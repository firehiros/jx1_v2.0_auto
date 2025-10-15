/**
 * @file D3D9Hook.h
 * @brief DirectX 9 hooking for overlay rendering
 *
 * Phase 4 - DirectX Hook & Overlay
 *
 * Hooks DirectX 9 EndScene/Present to inject our overlay rendering
 */

#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <functional>

namespace JX1 {

/**
 * D3D9Hook - Hooks DirectX 9 for overlay rendering
 *
 * This class hooks the DirectX 9 Present or EndScene function
 * to inject our ImGui overlay rendering into the game's render pipeline.
 *
 * Usage:
 *   D3D9Hook* hook = D3D9Hook::Get();
 *   hook->Initialize();
 *   hook->RegisterRenderCallback([](IDirect3DDevice9* device) {
 *       // Render overlay here
 *       ImGui::Begin("My Overlay");
 *       ImGui::Text("Hello World");
 *       ImGui::End();
 *   });
 */
class D3D9Hook
{
public:
    /**
     * Render callback type
     * Called every frame with the D3D device
     */
    using RenderCallback = std::function<void(IDirect3DDevice9*)>;

    /**
     * Get singleton instance
     */
    static D3D9Hook* Get();

    /**
     * Initialize DirectX 9 hook
     * Finds the game's D3D9 device and hooks Present/EndScene
     */
    bool Initialize();

    /**
     * Shutdown and unhook
     */
    void Shutdown();

    /**
     * Check if hook is active
     */
    bool IsHooked() const { return m_isHooked; }

    /**
     * Register a render callback
     * Will be called every frame during Present/EndScene
     */
    void RegisterRenderCallback(RenderCallback callback);

    /**
     * Get the D3D9 device pointer
     */
    IDirect3DDevice9* GetDevice() const { return m_device; }

    /**
     * Get device parameters
     */
    D3DPRESENT_PARAMETERS GetPresentParams() const { return m_presentParams; }

    // ========================================
    // Hook Control
    // ========================================

    /**
     * Enable/disable rendering
     * Useful for temporarily hiding overlay
     */
    void SetRenderingEnabled(bool enabled) { m_renderingEnabled = enabled; }
    bool IsRenderingEnabled() const { return m_renderingEnabled; }

private:
    D3D9Hook() = default;
    ~D3D9Hook() = default;
    D3D9Hook(const D3D9Hook&) = delete;
    D3D9Hook& operator=(const D3D9Hook&) = delete;

    /**
     * Find the game's D3D9 device
     * Creates a dummy device to get the VTable
     */
    bool FindDevice();

    /**
     * Hook the Present function
     */
    bool HookPresent();

    /**
     * Hook the EndScene function
     */
    bool HookEndScene();

    /**
     * Hook the Reset function (for handling resolution changes)
     */
    bool HookReset();

    /**
     * Initialize ImGui
     */
    bool InitializeImGui();

    /**
     * Shutdown ImGui
     */
    void ShutdownImGui();

    // ========================================
    // Hook Functions (static for MinHook)
    // ========================================

    /**
     * Hooked Present function
     */
    static HRESULT WINAPI PresentHook(
        IDirect3DDevice9* device,
        CONST RECT* pSourceRect,
        CONST RECT* pDestRect,
        HWND hDestWindowOverride,
        CONST RGNDATA* pDirtyRegion
    );

    /**
     * Hooked EndScene function
     */
    static HRESULT WINAPI EndSceneHook(IDirect3DDevice9* device);

    /**
     * Hooked Reset function
     */
    static HRESULT WINAPI ResetHook(
        IDirect3DDevice9* device,
        D3DPRESENT_PARAMETERS* pPresentationParameters
    );

    // ========================================
    // Original Functions (trampolines)
    // ========================================

    using PresentFn = HRESULT(WINAPI*)(
        IDirect3DDevice9*,
        CONST RECT*,
        CONST RECT*,
        HWND,
        CONST RGNDATA*
    );

    using EndSceneFn = HRESULT(WINAPI*)(IDirect3DDevice9*);

    using ResetFn = HRESULT(WINAPI*)(
        IDirect3DDevice9*,
        D3DPRESENT_PARAMETERS*
    );

    static PresentFn m_originalPresent;
    static EndSceneFn m_originalEndScene;
    static ResetFn m_originalReset;

    // ========================================
    // Member Variables
    // ========================================

    IDirect3DDevice9* m_device = nullptr;
    D3DPRESENT_PARAMETERS m_presentParams = {};
    HWND m_gameWindow = nullptr;

    bool m_isHooked = false;
    bool m_imguiInitialized = false;
    bool m_renderingEnabled = true;

    std::vector<RenderCallback> m_renderCallbacks;

    // VTable indices for D3D9 methods
    static constexpr int PRESENT_INDEX = 17;
    static constexpr int ENDSCENE_INDEX = 42;
    static constexpr int RESET_INDEX = 16;
};

} // namespace JX1
