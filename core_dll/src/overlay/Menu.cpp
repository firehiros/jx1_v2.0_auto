/**
 * @file Menu.cpp
 * @brief ImGui menu implementation
 *
 * Phase 4 - DirectX Hook & Overlay
 */

#include "Menu.h"
#include "ESPRenderer.h"
#include "OverlayRenderer.h"
#include "../core/Logger.h"
#include <imgui.h>

namespace JX1 {

Menu* Menu::Get()
{
    static Menu instance;
    return &instance;
}

void Menu::Render()
{
    if (!m_visible) return;

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("JX1 Auto v2.0 - Settings", &m_visible, ImGuiWindowFlags_NoCollapse)) {
        // Tab bar
        if (ImGui::BeginTabBar("MainTabBar")) {
            if (ImGui::BeginTabItem("ESP")) {
                RenderESPTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Bot")) {
                RenderBotTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Debug")) {
                RenderDebugTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("About")) {
                RenderAboutTab();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void Menu::RenderESPTab()
{
    ESPSettings& settings = ESPRenderer::Get()->GetSettings();

    ImGui::Text("ESP Settings");
    ImGui::Separator();

    // Enable/Disable options
    ImGui::Checkbox("Show Monsters", &settings.showMonsters);
    ImGui::Checkbox("Show Players", &settings.showPlayers);
    ImGui::Checkbox("Show NPCs", &settings.showNPCs);
    ImGui::Checkbox("Show Items", &settings.showItems);

    ImGui::Separator();

    // Display options
    ImGui::Text("Display Options");
    ImGui::Checkbox("Show Boxes", &settings.showBoxes);
    ImGui::Checkbox("Show Names", &settings.showNames);
    ImGui::Checkbox("Show HP Bars", &settings.showHP);
    ImGui::Checkbox("Show Distance", &settings.showDistance);
    ImGui::Checkbox("Show Lines to Entities", &settings.showLines);

    ImGui::Separator();

    // Filters
    ImGui::Text("Filters");
    ImGui::SliderFloat("Max Distance", &settings.maxDistance, 100.0f, 1000.0f, "%.0f");
    ImGui::Checkbox("Only Hostile Monsters", &settings.onlyHostile);
    ImGui::Checkbox("Only Alive Entities", &settings.onlyAlive);

    ImGui::Separator();

    // Colors
    ImGui::Text("Colors");

    float monsterColor[4] = {
        settings.colorMonster.r / 255.0f,
        settings.colorMonster.g / 255.0f,
        settings.colorMonster.b / 255.0f,
        settings.colorMonster.a / 255.0f
    };
    if (ImGui::ColorEdit4("Monster Color", monsterColor)) {
        settings.colorMonster.r = (unsigned char)(monsterColor[0] * 255);
        settings.colorMonster.g = (unsigned char)(monsterColor[1] * 255);
        settings.colorMonster.b = (unsigned char)(monsterColor[2] * 255);
        settings.colorMonster.a = (unsigned char)(monsterColor[3] * 255);
    }

    float playerColor[4] = {
        settings.colorPlayer.r / 255.0f,
        settings.colorPlayer.g / 255.0f,
        settings.colorPlayer.b / 255.0f,
        settings.colorPlayer.a / 255.0f
    };
    if (ImGui::ColorEdit4("Player Color", playerColor)) {
        settings.colorPlayer.r = (unsigned char)(playerColor[0] * 255);
        settings.colorPlayer.g = (unsigned char)(playerColor[1] * 255);
        settings.colorPlayer.b = (unsigned char)(playerColor[2] * 255);
        settings.colorPlayer.a = (unsigned char)(playerColor[3] * 255);
    }

    float npcColor[4] = {
        settings.colorNPC.r / 255.0f,
        settings.colorNPC.g / 255.0f,
        settings.colorNPC.b / 255.0f,
        settings.colorNPC.a / 255.0f
    };
    if (ImGui::ColorEdit4("NPC Color", npcColor)) {
        settings.colorNPC.r = (unsigned char)(npcColor[0] * 255);
        settings.colorNPC.g = (unsigned char)(npcColor[1] * 255);
        settings.colorNPC.b = (unsigned char)(npcColor[2] * 255);
        settings.colorNPC.a = (unsigned char)(npcColor[3] * 255);
    }

    float itemColor[4] = {
        settings.colorItem.r / 255.0f,
        settings.colorItem.g / 255.0f,
        settings.colorItem.b / 255.0f,
        settings.colorItem.a / 255.0f
    };
    if (ImGui::ColorEdit4("Item Color", itemColor)) {
        settings.colorItem.r = (unsigned char)(itemColor[0] * 255);
        settings.colorItem.g = (unsigned char)(itemColor[1] * 255);
        settings.colorItem.b = (unsigned char)(itemColor[2] * 255);
        settings.colorItem.a = (unsigned char)(itemColor[3] * 255);
    }
}

void Menu::RenderBotTab()
{
    ImGui::Text("Bot Settings");
    ImGui::Separator();

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Coming in Phase 5-7!");

    ImGui::Spacing();
    ImGui::Text("Future features:");
    ImGui::BulletText("Auto Combat");
    ImGui::BulletText("Auto Loot");
    ImGui::BulletText("Auto Skill Rotation");
    ImGui::BulletText("Auto Movement");
    ImGui::BulletText("Auto Quest");
    ImGui::BulletText("Auto Farming");
}

void Menu::RenderDebugTab()
{
    ImGui::Text("Debug Options");
    ImGui::Separator();

    OverlayRenderer* renderer = OverlayRenderer::Get();

    bool espEnabled = renderer->IsESPEnabled();
    if (ImGui::Checkbox("Enable ESP", &espEnabled)) {
        renderer->SetESPEnabled(espEnabled);
    }

    bool debugEnabled = renderer->IsDebugEnabled();
    if (ImGui::Checkbox("Show Debug Info", &debugEnabled)) {
        renderer->SetDebugEnabled(debugEnabled);
    }

    bool crosshairEnabled = renderer->IsCrosshairEnabled();
    if (ImGui::Checkbox("Show Crosshair", &crosshairEnabled)) {
        renderer->SetCrosshairEnabled(crosshairEnabled);
    }

    ImGui::Separator();

    if (ImGui::Button("Test Notification")) {
        renderer->ShowNotification("This is a test notification!", 3.0f);
    }

    ImGui::Separator();

    ImGui::Text("Hotkeys:");
    ImGui::BulletText("INSERT - Toggle this menu");
    ImGui::BulletText("END - Unload DLL (shutdown)");
}

void Menu::RenderAboutTab()
{
    ImGui::Text("JX1 Auto v2.0");
    ImGui::Separator();

    ImGui::Text("A comprehensive automation tool for JX1");
    ImGui::Text("Created with official publisher partnership");

    ImGui::Spacing();

    ImGui::Text("Technology:");
    ImGui::BulletText("C++17");
    ImGui::BulletText("DirectX 9 Hook");
    ImGui::BulletText("ImGui (docking branch)");
    ImGui::BulletText("MinHook");

    ImGui::Spacing();

    ImGui::Text("Current Phase: Phase 4 - Overlay");
    ImGui::Text("Status: DirectX Hook & ESP Complete");

    ImGui::Spacing();

    ImGui::Separator();

    ImGui::Text("Developed by: [Your Name]");
    ImGui::Text("Version: 2.0.0");
    ImGui::Text("Build Date: 2025-10-15");
}

} // namespace JX1
