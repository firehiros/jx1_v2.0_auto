/**
 * @file OverlayRenderer.h
 * @brief Main overlay rendering coordinator
 *
 * Phase 4 - DirectX Hook & Overlay
 *
 * Coordinates all overlay rendering (ESP, Menu, Debug info, etc.)
 */

#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <imgui.h>

namespace JX1 {

/**
 * OverlayRenderer - Main coordinator for overlay rendering
 *
 * This class manages all overlay components:
 * - ESP (Entity markers, HP bars, names)
 * - Menu (Settings, configuration)
 * - Debug info (FPS, player stats)
 * - Crosshair
 * - Notifications
 *
 * Usage:
 *   OverlayRenderer* renderer = OverlayRenderer::Get();
 *   renderer->Initialize();
 *   renderer->Render(device); // Called every frame
 */
class OverlayRenderer
{
public:
    /**
     * Get singleton instance
     */
    static OverlayRenderer* Get();

    /**
     * Initialize overlay renderer
     */
    bool Initialize();

    /**
     * Shutdown renderer
     */
    void Shutdown();

    /**
     * Render all overlay components
     * Called every frame from D3D9Hook
     */
    void Render(IDirect3DDevice9* device);

    // ========================================
    // Component Enable/Disable
    // ========================================

    void SetESPEnabled(bool enabled) { m_espEnabled = enabled; }
    bool IsESPEnabled() const { return m_espEnabled; }

    void SetMenuEnabled(bool enabled) { m_menuEnabled = enabled; }
    bool IsMenuEnabled() const { return m_menuEnabled; }

    void SetDebugEnabled(bool enabled) { m_debugEnabled = enabled; }
    bool IsDebugEnabled() const { return m_debugEnabled; }

    void SetCrosshairEnabled(bool enabled) { m_crosshairEnabled = enabled; }
    bool IsCrosshairEnabled() const { return m_crosshairEnabled; }

    // ========================================
    // Overlay Control
    // ========================================

    /**
     * Toggle menu visibility (INSERT key by default)
     */
    void ToggleMenu() { m_menuEnabled = !m_menuEnabled; }

    /**
     * Show notification message
     */
    void ShowNotification(const char* message, float duration = 3.0f);

    // ========================================
    // Rendering Helpers
    // ========================================

    /**
     * Convert 3D world position to 2D screen position
     * @return true if position is on screen
     */
    bool WorldToScreen(const float worldPos[3], float screenPos[2]) const;

    /**
     * Get screen dimensions
     */
    void GetScreenSize(float& width, float& height) const;

    /**
     * Draw 2D line
     */
    void DrawLine(const ImVec2& from, const ImVec2& to, ImU32 color, float thickness = 1.0f);

    /**
     * Draw 2D box
     */
    void DrawBox(const ImVec2& topLeft, const ImVec2& bottomRight, ImU32 color, float thickness = 1.0f);

    /**
     * Draw filled box
     */
    void DrawFilledBox(const ImVec2& topLeft, const ImVec2& bottomRight, ImU32 color);

    /**
     * Draw text
     */
    void DrawText(const ImVec2& pos, const char* text, ImU32 color, bool centered = false);

    /**
     * Draw health bar
     */
    void DrawHealthBar(const ImVec2& pos, float width, float height, float hpPercent);

private:
    OverlayRenderer() = default;
    ~OverlayRenderer() = default;
    OverlayRenderer(const OverlayRenderer&) = delete;
    OverlayRenderer& operator=(const OverlayRenderer&) = delete;

    /**
     * Render individual components
     */
    void RenderESP();
    void RenderMenu();
    void RenderDebugInfo();
    void RenderCrosshair();
    void RenderNotifications();

    /**
     * Calculate view-projection matrix for WorldToScreen
     */
    void UpdateViewProjection();

    // ========================================
    // Member Variables
    // ========================================

    bool m_initialized = false;

    // Component enable flags
    bool m_espEnabled = true;
    bool m_menuEnabled = false;
    bool m_debugEnabled = true;
    bool m_crosshairEnabled = false;

    // Device info
    IDirect3DDevice9* m_device = nullptr;
    float m_screenWidth = 0.0f;
    float m_screenHeight = 0.0f;

    // View/Projection matrices for WorldToScreen
    float m_viewMatrix[16] = {};
    float m_projectionMatrix[16] = {};
    bool m_matricesValid = false;

    // Notifications
    struct Notification {
        std::string message;
        float timeRemaining;
    };
    std::vector<Notification> m_notifications;

    // ImGui draw list for custom rendering
    ImDrawList* m_drawList = nullptr;
};

} // namespace JX1
