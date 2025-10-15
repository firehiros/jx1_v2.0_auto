/**
 * @file OverlayRenderer.cpp
 * @brief Main overlay rendering implementation
 *
 * Phase 4 - DirectX Hook & Overlay
 */

#include "OverlayRenderer.h"
#include "ESPRenderer.h"
#include "Menu.h"
#include "../game/Player.h"
#include "../game/EntityManager.h"
#include "../core/Logger.h"
#include <cmath>

namespace JX1 {

OverlayRenderer* OverlayRenderer::Get()
{
    static OverlayRenderer instance;
    return &instance;
}

bool OverlayRenderer::Initialize()
{
    Logger::Info("[OverlayRenderer] Initializing overlay renderer...");

    m_initialized = true;

    Logger::Success("[OverlayRenderer] Overlay renderer initialized!");

    return true;
}

void OverlayRenderer::Shutdown()
{
    Logger::Info("[OverlayRenderer] Shutting down overlay renderer...");

    m_initialized = false;

    Logger::Success("[OverlayRenderer] Overlay renderer shut down.");
}

void OverlayRenderer::Render(IDirect3DDevice9* device)
{
    if (!m_initialized || !device) return;

    m_device = device;

    // Get screen dimensions
    D3DVIEWPORT9 viewport;
    device->GetViewport(&viewport);
    m_screenWidth = (float)viewport.Width;
    m_screenHeight = (float)viewport.Height;

    // Get ImGui draw list for custom drawing
    m_drawList = ImGui::GetBackgroundDrawList();

    // Update view/projection matrices for WorldToScreen
    UpdateViewProjection();

    // Render components
    if (m_espEnabled) {
        RenderESP();
    }

    if (m_menuEnabled) {
        RenderMenu();
    }

    if (m_debugEnabled) {
        RenderDebugInfo();
    }

    if (m_crosshairEnabled) {
        RenderCrosshair();
    }

    // Always render notifications
    RenderNotifications();
}

// ========================================
// Component Rendering
// ========================================

void OverlayRenderer::RenderESP()
{
    // Use ESPRenderer for entity rendering
    ESPRenderer::Get()->Render(m_device);
}

void OverlayRenderer::RenderMenu()
{
    // Use Menu for settings UI
    Menu::Get()->Render();
}

void OverlayRenderer::RenderDebugInfo()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(0.35f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoFocusOnAppearing |
                             ImGuiWindowFlags_NoNav;

    if (ImGui::Begin("Debug Info", nullptr, flags)) {
        ImGui::Text("JX1 Auto v2.0");
        ImGui::Separator();

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Screen: %.0fx%.0f", m_screenWidth, m_screenHeight);

        ImGui::Separator();

        // Player info
        Player* player = Player::Get();
        if (player->IsValid()) {
            ImGui::Text("=== Player ===");
            ImGui::Text("Name: %s", player->GetName().c_str());
            ImGui::Text("Level: %d", player->GetLevel());
            ImGui::Text("HP: %d / %d (%.1f%%)",
                player->GetHP(),
                player->GetMaxHP(),
                player->GetHPPercent()
            );
            ImGui::Text("MP: %d / %d (%.1f%%)",
                player->GetMP(),
                player->GetMaxMP(),
                player->GetMPPercent()
            );

            float x, y, z;
            player->GetPosition(x, y, z);
            ImGui::Text("Pos: (%.1f, %.1f, %.1f)", x, y, z);

            ImGui::Text("Dead: %s", player->IsDead() ? "Yes" : "No");
            ImGui::Text("Combat: %s", player->IsInCombat() ? "Yes" : "No");
            ImGui::Text("Moving: %s", player->IsMoving() ? "Yes" : "No");
        } else {
            ImGui::Text("Player: Not found");
        }

        ImGui::Separator();

        // Entity info
        EntityManager* mgr = EntityManager::Get();
        if (mgr->IsValid()) {
            ImGui::Text("=== Entities ===");
            ImGui::Text("Total: %d", mgr->GetEntityCount());

            auto monsters = mgr->GetAllMonsters();
            auto players = mgr->GetAllPlayers();
            auto npcs = mgr->GetAllNPCs();
            auto items = mgr->GetAllItems();

            ImGui::Text("Monsters: %zu", monsters.size());
            ImGui::Text("Players: %zu", players.size());
            ImGui::Text("NPCs: %zu", npcs.size());
            ImGui::Text("Items: %zu", items.size());

            auto nearbyEnemies = mgr->GetNearbyEnemies(500.0f);
            ImGui::Text("Enemies (500u): %zu", nearbyEnemies.size());
        } else {
            ImGui::Text("EntityManager: Not initialized");
        }

        ImGui::Separator();
        ImGui::Text("Press INSERT for menu");
    }
    ImGui::End();
}

void OverlayRenderer::RenderCrosshair()
{
    float centerX = m_screenWidth / 2.0f;
    float centerY = m_screenHeight / 2.0f;
    float size = 10.0f;
    float thickness = 2.0f;
    ImU32 color = IM_COL32(0, 255, 0, 255);

    // Draw crosshair
    m_drawList->AddLine(
        ImVec2(centerX - size, centerY),
        ImVec2(centerX + size, centerY),
        color,
        thickness
    );

    m_drawList->AddLine(
        ImVec2(centerX, centerY - size),
        ImVec2(centerX, centerY + size),
        color,
        thickness
    );
}

void OverlayRenderer::RenderNotifications()
{
    float startY = m_screenHeight - 150.0f;
    float x = m_screenWidth / 2.0f;

    // Update and render notifications
    for (auto it = m_notifications.begin(); it != m_notifications.end();) {
        it->timeRemaining -= ImGui::GetIO().DeltaTime;

        if (it->timeRemaining <= 0.0f) {
            it = m_notifications.erase(it);
            continue;
        }

        // Fade out in last second
        float alpha = (it->timeRemaining < 1.0f) ? it->timeRemaining : 1.0f;
        ImU32 color = IM_COL32(255, 255, 255, (int)(alpha * 255));

        // Draw notification
        ImVec2 textSize = ImGui::CalcTextSize(it->message.c_str());
        ImVec2 pos(x - textSize.x / 2.0f, startY);

        // Background
        ImVec2 bgMin(pos.x - 10, pos.y - 5);
        ImVec2 bgMax(pos.x + textSize.x + 10, pos.y + textSize.y + 5);
        m_drawList->AddRectFilled(bgMin, bgMax, IM_COL32(0, 0, 0, (int)(alpha * 150)));

        // Text
        m_drawList->AddText(pos, color, it->message.c_str());

        startY += textSize.y + 10.0f;
        ++it;
    }
}

// ========================================
// Notification System
// ========================================

void OverlayRenderer::ShowNotification(const char* message, float duration)
{
    Notification notif;
    notif.message = message;
    notif.timeRemaining = duration;
    m_notifications.push_back(notif);

    Logger::Info("[Notification] %s", message);
}

// ========================================
// WorldToScreen Conversion
// ========================================

void OverlayRenderer::UpdateViewProjection()
{
    if (!m_device) {
        m_matricesValid = false;
        return;
    }

    // Get view and projection matrices from D3D9 device
    D3DMATRIX view, projection;

    HRESULT hr1 = m_device->GetTransform(D3DTS_VIEW, &view);
    HRESULT hr2 = m_device->GetTransform(D3DTS_PROJECTION, &projection);

    if (FAILED(hr1) || FAILED(hr2)) {
        m_matricesValid = false;
        return;
    }

    // Copy matrices
    memcpy(m_viewMatrix, &view, sizeof(D3DMATRIX));
    memcpy(m_projectionMatrix, &projection, sizeof(D3DMATRIX));

    m_matricesValid = true;
}

bool OverlayRenderer::WorldToScreen(const float worldPos[3], float screenPos[2]) const
{
    if (!m_matricesValid) {
        return false;
    }

    // Convert D3DMATRIX to float arrays for easier access
    const float* view = m_viewMatrix;
    const float* proj = m_projectionMatrix;

    // Transform world position to view space
    float viewX = worldPos[0] * view[0] + worldPos[1] * view[4] + worldPos[2] * view[8] + view[12];
    float viewY = worldPos[0] * view[1] + worldPos[1] * view[5] + worldPos[2] * view[9] + view[13];
    float viewZ = worldPos[0] * view[2] + worldPos[1] * view[6] + worldPos[2] * view[10] + view[14];
    float viewW = worldPos[0] * view[3] + worldPos[1] * view[7] + worldPos[2] * view[11] + view[15];

    // Transform to clip space
    float clipX = viewX * proj[0] + viewY * proj[4] + viewZ * proj[8] + viewW * proj[12];
    float clipY = viewX * proj[1] + viewY * proj[5] + viewZ * proj[9] + viewW * proj[13];
    float clipW = viewX * proj[3] + viewY * proj[7] + viewZ * proj[11] + viewW * proj[15];

    if (clipW < 0.1f) {
        return false; // Behind camera
    }

    // Perspective divide
    float ndcX = clipX / clipW;
    float ndcY = clipY / clipW;

    // Check if in screen bounds
    if (ndcX < -1.0f || ndcX > 1.0f || ndcY < -1.0f || ndcY > 1.0f) {
        return false;
    }

    // Convert to screen coordinates
    screenPos[0] = (ndcX + 1.0f) * 0.5f * m_screenWidth;
    screenPos[1] = (1.0f - ndcY) * 0.5f * m_screenHeight;

    return true;
}

// ========================================
// Drawing Helpers
// ========================================

void OverlayRenderer::GetScreenSize(float& width, float& height) const
{
    width = m_screenWidth;
    height = m_screenHeight;
}

void OverlayRenderer::DrawLine(const ImVec2& from, const ImVec2& to, ImU32 color, float thickness)
{
    if (!m_drawList) return;
    m_drawList->AddLine(from, to, color, thickness);
}

void OverlayRenderer::DrawBox(const ImVec2& topLeft, const ImVec2& bottomRight, ImU32 color, float thickness)
{
    if (!m_drawList) return;
    m_drawList->AddRect(topLeft, bottomRight, color, 0.0f, 0, thickness);
}

void OverlayRenderer::DrawFilledBox(const ImVec2& topLeft, const ImVec2& bottomRight, ImU32 color)
{
    if (!m_drawList) return;
    m_drawList->AddRectFilled(topLeft, bottomRight, color);
}

void OverlayRenderer::DrawText(const ImVec2& pos, const char* text, ImU32 color, bool centered)
{
    if (!m_drawList) return;

    ImVec2 textPos = pos;

    if (centered) {
        ImVec2 textSize = ImGui::CalcTextSize(text);
        textPos.x -= textSize.x / 2.0f;
        textPos.y -= textSize.y / 2.0f;
    }

    // Draw text with black outline for visibility
    m_drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), IM_COL32(0, 0, 0, 255), text);
    m_drawList->AddText(textPos, color, text);
}

void OverlayRenderer::DrawHealthBar(const ImVec2& pos, float width, float height, float hpPercent)
{
    if (!m_drawList) return;

    // Clamp percent
    if (hpPercent < 0.0f) hpPercent = 0.0f;
    if (hpPercent > 100.0f) hpPercent = 100.0f;

    // Background (dark gray)
    ImVec2 bgMin = pos;
    ImVec2 bgMax(pos.x + width, pos.y + height);
    m_drawList->AddRectFilled(bgMin, bgMax, IM_COL32(50, 50, 50, 200));

    // Health bar (color based on HP)
    float hpWidth = width * (hpPercent / 100.0f);
    ImVec2 hpMin = pos;
    ImVec2 hpMax(pos.x + hpWidth, pos.y + height);

    ImU32 hpColor;
    if (hpPercent > 50.0f) {
        hpColor = IM_COL32(0, 255, 0, 255); // Green
    } else if (hpPercent > 25.0f) {
        hpColor = IM_COL32(255, 255, 0, 255); // Yellow
    } else {
        hpColor = IM_COL32(255, 0, 0, 255); // Red
    }

    m_drawList->AddRectFilled(hpMin, hpMax, hpColor);

    // Border
    m_drawList->AddRect(bgMin, bgMax, IM_COL32(255, 255, 255, 255), 0.0f, 0, 1.0f);
}

} // namespace JX1
