/**
 * @file D3D9Hook.cpp
 * @brief DirectX 9 hooking implementation
 *
 * Phase 4 - DirectX Hook & Overlay
 */

#include "D3D9Hook.h"
#include "../core/Logger.h"
#include "../core/HookManager.h"
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

// Forward declare ImGui WndProc handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace JX1 {

// Static member initialization
D3D9Hook::PresentFn D3D9Hook::m_originalPresent = nullptr;
D3D9Hook::EndSceneFn D3D9Hook::m_originalEndScene = nullptr;
D3D9Hook::ResetFn D3D9Hook::m_originalReset = nullptr;

D3D9Hook* D3D9Hook::Get()
{
    static D3D9Hook instance;
    return &instance;
}

bool D3D9Hook::Initialize()
{
    Logger::Info("[D3D9Hook] Initializing DirectX 9 hook...");

    // Find the D3D9 device
    if (!FindDevice()) {
        Logger::Error("[D3D9Hook] Failed to find D3D9 device!");
        return false;
    }

    // Hook Present
    if (!HookPresent()) {
        Logger::Error("[D3D9Hook] Failed to hook Present!");
        return false;
    }

    // Hook EndScene (backup method)
    if (!HookEndScene()) {
        Logger::Warning("[D3D9Hook] Failed to hook EndScene (non-critical)");
    }

    // Hook Reset (for resolution changes)
    if (!HookReset()) {
        Logger::Warning("[D3D9Hook] Failed to hook Reset (non-critical)");
    }

    m_isHooked = true;

    Logger::Success("[D3D9Hook] DirectX 9 hook initialized successfully!");

    return true;
}

void D3D9Hook::Shutdown()
{
    Logger::Info("[D3D9Hook] Shutting down DirectX 9 hook...");

    // Shutdown ImGui
    if (m_imguiInitialized) {
        ShutdownImGui();
    }

    // Unhook functions
    // MinHook will handle this when HookManager::Shutdown() is called

    m_isHooked = false;
    m_device = nullptr;

    Logger::Success("[D3D9Hook] DirectX 9 hook shut down.");
}

void D3D9Hook::RegisterRenderCallback(RenderCallback callback)
{
    m_renderCallbacks.push_back(callback);
    Logger::Info("[D3D9Hook] Registered render callback (total: %zu)", m_renderCallbacks.size());
}

// ========================================
// Private Methods
// ========================================

bool D3D9Hook::FindDevice()
{
    Logger::Info("[D3D9Hook] Finding D3D9 device...");

    // Create a dummy D3D9 object to get the VTable
    IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d9) {
        Logger::Error("[D3D9Hook] Failed to create D3D9 object!");
        return false;
    }

    // Get adapter display mode
    D3DDISPLAYMODE displayMode;
    HRESULT hr = d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
    if (FAILED(hr)) {
        Logger::Error("[D3D9Hook] Failed to get display mode!");
        d3d9->Release();
        return false;
    }

    // Setup present parameters for dummy device
    D3DPRESENT_PARAMETERS pp = {};
    pp.Windowed = TRUE;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.BackBufferFormat = displayMode.Format;
    pp.BackBufferWidth = 1;
    pp.BackBufferHeight = 1;
    pp.hDeviceWindow = GetDesktopWindow();

    // Create dummy device
    IDirect3DDevice9* dummyDevice = nullptr;
    hr = d3d9->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        pp.hDeviceWindow,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &pp,
        &dummyDevice
    );

    if (FAILED(hr) || !dummyDevice) {
        Logger::Error("[D3D9Hook] Failed to create dummy D3D9 device!");
        d3d9->Release();
        return false;
    }

    // Get VTable from dummy device
    void** vTable = *reinterpret_cast<void***>(dummyDevice);

    // Store function pointers
    m_originalPresent = reinterpret_cast<PresentFn>(vTable[PRESENT_INDEX]);
    m_originalEndScene = reinterpret_cast<EndSceneFn>(vTable[ENDSCENE_INDEX]);
    m_originalReset = reinterpret_cast<ResetFn>(vTable[RESET_INDEX]);

    Logger::Info("[D3D9Hook] Found D3D9 VTable:");
    Logger::Info("  Present:  0x%p", m_originalPresent);
    Logger::Info("  EndScene: 0x%p", m_originalEndScene);
    Logger::Info("  Reset:    0x%p", m_originalReset);

    // Cleanup dummy device
    dummyDevice->Release();
    d3d9->Release();

    return true;
}

bool D3D9Hook::HookPresent()
{
    Logger::Info("[D3D9Hook] Hooking Present...");

    return HookManager::Get().CreateHook(
        reinterpret_cast<void*>(m_originalPresent),
        reinterpret_cast<void*>(&PresentHook),
        reinterpret_cast<void**>(&m_originalPresent),
        "D3D9::Present"
    );
}

bool D3D9Hook::HookEndScene()
{
    Logger::Info("[D3D9Hook] Hooking EndScene...");

    return HookManager::Get().CreateHook(
        reinterpret_cast<void*>(m_originalEndScene),
        reinterpret_cast<void*>(&EndSceneHook),
        reinterpret_cast<void**>(&m_originalEndScene),
        "D3D9::EndScene"
    );
}

bool D3D9Hook::HookReset()
{
    Logger::Info("[D3D9Hook] Hooking Reset...");

    return HookManager::Get().CreateHook(
        reinterpret_cast<void*>(m_originalReset),
        reinterpret_cast<void*>(&ResetHook),
        reinterpret_cast<void**>(&m_originalReset),
        "D3D9::Reset"
    );
}

bool D3D9Hook::InitializeImGui()
{
    if (m_imguiInitialized) return true;

    Logger::Info("[D3D9Hook] Initializing ImGui...");

    // Get game window
    D3DDEVICE_CREATION_PARAMETERS creationParams;
    m_device->GetCreationParameters(&creationParams);
    m_gameWindow = creationParams.hFocusWindow;

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Configure ImGui
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_gameWindow);
    ImGui_ImplDX9_Init(m_device);

    m_imguiInitialized = true;

    Logger::Success("[D3D9Hook] ImGui initialized!");

    return true;
}

void D3D9Hook::ShutdownImGui()
{
    if (!m_imguiInitialized) return;

    Logger::Info("[D3D9Hook] Shutting down ImGui...");

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    m_imguiInitialized = false;

    Logger::Success("[D3D9Hook] ImGui shut down.");
}

// ========================================
// Hook Functions
// ========================================

HRESULT WINAPI D3D9Hook::PresentHook(
    IDirect3DDevice9* device,
    CONST RECT* pSourceRect,
    CONST RECT* pDestRect,
    HWND hDestWindowOverride,
    CONST RGNDATA* pDirtyRegion)
{
    D3D9Hook* instance = D3D9Hook::Get();

    // Store device on first call
    if (!instance->m_device) {
        instance->m_device = device;
        Logger::Info("[D3D9Hook] Captured D3D9 device: 0x%p", device);
    }

    // Initialize ImGui on first frame
    if (!instance->m_imguiInitialized) {
        instance->InitializeImGui();
    }

    // Render overlay if enabled
    if (instance->m_renderingEnabled && instance->m_imguiInitialized) {
        // Start ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Call all registered render callbacks
        for (const auto& callback : instance->m_renderCallbacks) {
            callback(device);
        }

        // Render ImGui
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }

    // Call original Present
    return instance->m_originalPresent(
        device,
        pSourceRect,
        pDestRect,
        hDestWindowOverride,
        pDirtyRegion
    );
}

HRESULT WINAPI D3D9Hook::EndSceneHook(IDirect3DDevice9* device)
{
    D3D9Hook* instance = D3D9Hook::Get();

    // Alternative rendering point (some games use EndScene instead of Present)
    // For now, we primarily use Present, but this is here as backup

    // Call original EndScene
    return instance->m_originalEndScene(device);
}

HRESULT WINAPI D3D9Hook::ResetHook(
    IDirect3DDevice9* device,
    D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    D3D9Hook* instance = D3D9Hook::Get();

    Logger::Info("[D3D9Hook] Device Reset called (resolution change?)");

    // Invalidate ImGui resources before reset
    if (instance->m_imguiInitialized) {
        ImGui_ImplDX9_InvalidateDeviceObjects();
    }

    // Call original Reset
    HRESULT result = instance->m_originalReset(device, pPresentationParameters);

    // Recreate ImGui resources after reset
    if (instance->m_imguiInitialized && SUCCEEDED(result)) {
        ImGui_ImplDX9_CreateDeviceObjects();
    }

    return result;
}

} // namespace JX1
