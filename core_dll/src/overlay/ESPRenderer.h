/**
 * @file ESPRenderer.h
 * @brief ESP (Extra Sensory Perception) rendering for entities
 *
 * Phase 4 - DirectX Hook & Overlay
 *
 * Renders entity information overlay:
 * - Entity boxes
 * - Entity names
 * - HP bars
 * - Distance indicators
 */

#pragma once

#include <Windows.h>
#include <d3d9.h>

namespace JX1 {

/**
 * ESP Rendering Settings
 */
struct ESPSettings
{
    // Enable/Disable
    bool showMonsters = true;
    bool showPlayers = true;
    bool showNPCs = true;
    bool showItems = true;

    // Display options
    bool showBoxes = true;
    bool showNames = true;
    bool showHP = true;
    bool showDistance = true;
    bool showLines = false;  // Lines from player to entities

    // Filters
    float maxDistance = 500.0f;  // Max ESP distance
    bool onlyHostile = false;     // Only show hostile monsters
    bool onlyAlive = true;        // Skip dead entities

    // Colors (RGBA)
    struct {
        unsigned char r, g, b, a;
    } colorMonster = {255, 0, 0, 255};      // Red

    struct {
        unsigned char r, g, b, a;
    } colorPlayer = {0, 255, 255, 255};     // Cyan

    struct {
        unsigned char r, g, b, a;
    } colorNPC = {255, 255, 0, 255};        // Yellow

    struct {
        unsigned char r, g, b, a;
    } colorItem = {255, 255, 255, 255};     // White
};

/**
 * ESPRenderer - Renders ESP overlay for entities
 *
 * Usage:
 *   ESPRenderer* esp = ESPRenderer::Get();
 *   esp->Render(device);
 */
class ESPRenderer
{
public:
    /**
     * Get singleton instance
     */
    static ESPRenderer* Get();

    /**
     * Render ESP for all entities
     */
    void Render(IDirect3DDevice9* device);

    /**
     * Get/Set settings
     */
    ESPSettings& GetSettings() { return m_settings; }
    void SetSettings(const ESPSettings& settings) { m_settings = settings; }

private:
    ESPRenderer() = default;
    ~ESPRenderer() = default;
    ESPRenderer(const ESPRenderer&) = delete;
    ESPRenderer& operator=(const ESPRenderer&) = delete;

    /**
     * Render ESP for specific entity
     */
    void RenderEntity(class Entity& entity, unsigned int color);

    /**
     * Draw 2D box around entity
     */
    void DrawEntityBox(const float screenPos[2], unsigned int color);

    /**
     * Draw entity name
     */
    void DrawEntityName(const float screenPos[2], const char* name, unsigned int color);

    /**
     * Draw entity HP bar
     */
    void DrawEntityHP(const float screenPos[2], float hpPercent);

    /**
     * Draw entity distance
     */
    void DrawEntityDistance(const float screenPos[2], float distance);

    /**
     * Draw line from player to entity
     */
    void DrawLineToEntity(const float screenPos[2], unsigned int color);

    ESPSettings m_settings;
};

} // namespace JX1
