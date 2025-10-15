/**
 * @file Menu.h
 * @brief ImGui overlay menu for settings and controls
 *
 * Phase 4 - DirectX Hook & Overlay
 *
 * Provides settings menu for:
 * - ESP configuration
 * - Bot settings
 * - Debug options
 */

#pragma once

namespace JX1 {

/**
 * Menu - ImGui settings menu
 *
 * Usage:
 *   Menu* menu = Menu::Get();
 *   menu->Render(); // Called every frame when menu is visible
 */
class Menu
{
public:
    /**
     * Get singleton instance
     */
    static Menu* Get();

    /**
     * Render menu
     */
    void Render();

    /**
     * Show/hide menu
     */
    void Show() { m_visible = true; }
    void Hide() { m_visible = false; }
    void Toggle() { m_visible = !m_visible; }
    bool IsVisible() const { return m_visible; }

private:
    Menu() = default;
    ~Menu() = default;
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    /**
     * Render individual tabs
     */
    void RenderESPTab();
    void RenderBotTab();
    void RenderDebugTab();
    void RenderAboutTab();

    bool m_visible = false;
    int m_selectedTab = 0;
};

} // namespace JX1
