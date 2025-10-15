/**
 * @file ESPRenderer.cpp
 * @brief ESP rendering implementation
 *
 * Phase 4 - DirectX Hook & Overlay
 */

#include "ESPRenderer.h"
#include "OverlayRenderer.h"
#include "../game/Entity.h"
#include "../game/EntityManager.h"
#include "../game/Player.h"
#include <imgui.h>

namespace JX1 {

ESPRenderer* ESPRenderer::Get()
{
    static ESPRenderer instance;
    return &instance;
}

void ESPRenderer::Render(IDirect3DDevice9* device)
{
    EntityManager* mgr = EntityManager::Get();
    if (!mgr->IsValid()) return;

    // Get all entities within max distance
    std::vector<Entity> entities = mgr->GetNearbyEntities(m_settings.maxDistance);

    for (Entity& entity : entities) {
        if (!entity.IsValid()) continue;

        // Skip dead entities if filter is enabled
        if (m_settings.onlyAlive && entity.IsDead()) continue;

        // Determine color based on entity type
        unsigned int color = 0;

        if (entity.IsMonster()) {
            if (!m_settings.showMonsters) continue;

            // Skip non-hostile if filter is enabled
            if (m_settings.onlyHostile && !entity.IsHostile()) continue;

            color = IM_COL32(
                m_settings.colorMonster.r,
                m_settings.colorMonster.g,
                m_settings.colorMonster.b,
                m_settings.colorMonster.a
            );
        }
        else if (entity.IsPlayer()) {
            if (!m_settings.showPlayers) continue;

            color = IM_COL32(
                m_settings.colorPlayer.r,
                m_settings.colorPlayer.g,
                m_settings.colorPlayer.b,
                m_settings.colorPlayer.a
            );
        }
        else if (entity.IsNPC()) {
            if (!m_settings.showNPCs) continue;

            color = IM_COL32(
                m_settings.colorNPC.r,
                m_settings.colorNPC.g,
                m_settings.colorNPC.b,
                m_settings.colorNPC.a
            );
        }
        else if (entity.IsItem()) {
            if (!m_settings.showItems) continue;

            color = IM_COL32(
                m_settings.colorItem.r,
                m_settings.colorItem.g,
                m_settings.colorItem.b,
                m_settings.colorItem.a
            );
        }
        else {
            continue; // Unknown type
        }

        // Render this entity
        RenderEntity(entity, color);
    }
}

void ESPRenderer::RenderEntity(Entity& entity, unsigned int color)
{
    // Get entity world position
    float worldPos[3];
    entity.GetPosition(worldPos[0], worldPos[1], worldPos[2]);

    // Convert to screen position
    float screenPos[2];
    if (!OverlayRenderer::Get()->WorldToScreen(worldPos, screenPos)) {
        return; // Entity not on screen
    }

    // Draw box
    if (m_settings.showBoxes) {
        DrawEntityBox(screenPos, color);
    }

    // Draw name
    if (m_settings.showNames) {
        std::string name = entity.GetName();
        DrawEntityName(screenPos, name.c_str(), color);
    }

    // Draw HP bar (skip for items)
    if (m_settings.showHP && !entity.IsItem()) {
        float hpPercent = entity.GetHPPercent();
        DrawEntityHP(screenPos, hpPercent);
    }

    // Draw distance
    if (m_settings.showDistance) {
        float distance = entity.GetDistanceToPlayer();
        DrawEntityDistance(screenPos, distance);
    }

    // Draw line to entity
    if (m_settings.showLines) {
        DrawLineToEntity(screenPos, color);
    }
}

void ESPRenderer::DrawEntityBox(const float screenPos[2], unsigned int color)
{
    // Draw a simple box around entity
    float boxWidth = 40.0f;
    float boxHeight = 60.0f;

    ImVec2 topLeft(screenPos[0] - boxWidth / 2.0f, screenPos[1] - boxHeight);
    ImVec2 bottomRight(screenPos[0] + boxWidth / 2.0f, screenPos[1]);

    OverlayRenderer::Get()->DrawBox(topLeft, bottomRight, color, 2.0f);
}

void ESPRenderer::DrawEntityName(const float screenPos[2], const char* name, unsigned int color)
{
    float offsetY = -70.0f; // Above the box

    ImVec2 pos(screenPos[0], screenPos[1] + offsetY);

    OverlayRenderer::Get()->DrawText(pos, name, color, true);
}

void ESPRenderer::DrawEntityHP(const float screenPos[2], float hpPercent)
{
    float offsetY = -75.0f; // Above the name
    float barWidth = 50.0f;
    float barHeight = 5.0f;

    ImVec2 pos(screenPos[0] - barWidth / 2.0f, screenPos[1] + offsetY);

    OverlayRenderer::Get()->DrawHealthBar(pos, barWidth, barHeight, hpPercent);
}

void ESPRenderer::DrawEntityDistance(const float screenPos[2], float distance)
{
    float offsetY = 10.0f; // Below the entity

    ImVec2 pos(screenPos[0], screenPos[1] + offsetY);

    char distText[32];
    sprintf_s(distText, "%.1fm", distance);

    OverlayRenderer::Get()->DrawText(pos, distText, IM_COL32(255, 255, 255, 200), true);
}

void ESPRenderer::DrawLineToEntity(const float screenPos[2], unsigned int color)
{
    float screenWidth, screenHeight;
    OverlayRenderer::Get()->GetScreenSize(screenWidth, screenHeight);

    // Line from bottom-center of screen to entity
    ImVec2 from(screenWidth / 2.0f, screenHeight);
    ImVec2 to(screenPos[0], screenPos[1]);

    OverlayRenderer::Get()->DrawLine(from, to, color, 1.0f);
}

} // namespace JX1
