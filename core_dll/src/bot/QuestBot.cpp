/**
 * @file QuestBot.cpp
 * @brief QuestBot implementation (stub for Phase 7)
 */

#include "QuestBot.h"
#include "../core/Logger.h"

namespace JX1 {

QuestBot* QuestBot::Get()
{
    static QuestBot instance;
    return &instance;
}

void QuestBot::Start()
{
    if (m_running) {
        Logger::Warning("[QuestBot] Already running");
        return;
    }

    Logger::Info("[QuestBot] Starting...");
    m_running = true;
}

void QuestBot::Stop()
{
    if (!m_running) {
        return;
    }

    Logger::Info("[QuestBot] Stopping...");
    m_running = false;
}

void QuestBot::Update()
{
    if (!m_running) return;

    // TODO: Implement in Phase 7
    // For now, this is a stub
}

} // namespace JX1
