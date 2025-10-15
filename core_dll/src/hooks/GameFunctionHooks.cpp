/**
 * @file GameFunctionHooks.cpp
 * @brief Game function hooks implementation
 *
 * Phase 5 - Game Function Hooks
 */

#include "GameFunctionHooks.h"
#include "../game/Entity.h"
#include "../game/Player.h"
#include "../game/EntityManager.h"
#include "../game/Inventory.h"
#include "../core/Logger.h"
#include "../core/MemoryManager.h"
#include "../utils/Patterns.h"
#include "../utils/Offsets.h"
#include <cstring>

namespace JX1 {

GameFunctionHooks* GameFunctionHooks::Get()
{
    static GameFunctionHooks instance;
    return &instance;
}

bool GameFunctionHooks::Initialize()
{
    Logger::Info("[GameFunctionHooks] Initializing game function hooks...");

    if (!FindFunctions()) {
        Logger::Error("[GameFunctionHooks] Failed to find game functions!");
        return false;
    }

    m_initialized = true;

    Logger::Success("[GameFunctionHooks] Game function hooks initialized!");

    return true;
}

void GameFunctionHooks::Shutdown()
{
    Logger::Info("[GameFunctionHooks] Shutting down game function hooks...");

    m_initialized = false;

    Logger::Success("[GameFunctionHooks] Game function hooks shut down.");
}

// ========================================
// Private Methods
// ========================================

bool GameFunctionHooks::FindFunctions()
{
    Logger::Info("[GameFunctionHooks] Finding game functions...");

    MemoryManager& mem = MemoryManager::Get();

    // Find Attack function
    uintptr_t attackAddr = mem.FindPattern(Patterns::ATTACK_FUNCTION, Patterns::ATTACK_FUNCTION_MASK);
    if (ValidateFunction(attackAddr, "Attack")) {
        m_attackFn = reinterpret_cast<AttackFn>(attackAddr);
        Offsets::Functions::Attack = attackAddr;
    }

    // Find UseSkill function
    uintptr_t useSkillAddr = mem.FindPattern(Patterns::USESKILL_FUNCTION, Patterns::USESKILL_FUNCTION_MASK);
    if (ValidateFunction(useSkillAddr, "UseSkill")) {
        m_useSkillFn = reinterpret_cast<UseSkillFn>(useSkillAddr);
        Offsets::Functions::UseSkill = useSkillAddr;
    }

    // Find StopAttack function
    uintptr_t stopAttackAddr = mem.FindPattern(Patterns::STOPATTACK_FUNCTION, Patterns::STOPATTACK_FUNCTION_MASK);
    if (ValidateFunction(stopAttackAddr, "StopAttack")) {
        m_stopAttackFn = reinterpret_cast<StopAttackFn>(stopAttackAddr);
        Offsets::Functions::StopAttack = stopAttackAddr;
    }

    // Find MoveTo function
    uintptr_t moveToAddr = mem.FindPattern(Patterns::MOVETO_FUNCTION, Patterns::MOVETO_FUNCTION_MASK);
    if (ValidateFunction(moveToAddr, "MoveTo")) {
        m_moveToFn = reinterpret_cast<MoveToFn>(moveToAddr);
        Offsets::Functions::MoveTo = moveToAddr;
    }

    // Find Teleport function
    uintptr_t teleportAddr = mem.FindPattern(Patterns::TELEPORT_FUNCTION, Patterns::TELEPORT_FUNCTION_MASK);
    if (ValidateFunction(teleportAddr, "Teleport")) {
        m_teleportFn = reinterpret_cast<TeleportFn>(teleportAddr);
        Offsets::Functions::Teleport = teleportAddr;
    }

    // Find UseItem function
    uintptr_t useItemAddr = mem.FindPattern(Patterns::USEITEM_FUNCTION, Patterns::USEITEM_FUNCTION_MASK);
    if (ValidateFunction(useItemAddr, "UseItem")) {
        m_useItemFn = reinterpret_cast<UseItemFn>(useItemAddr);
        Offsets::Functions::UseItem = useItemAddr;
    }

    // Find PickupItem function
    uintptr_t pickupAddr = mem.FindPattern(Patterns::PICKUP_FUNCTION, Patterns::PICKUP_FUNCTION_MASK);
    if (ValidateFunction(pickupAddr, "PickupItem")) {
        m_pickupItemFn = reinterpret_cast<PickupItemFn>(pickupAddr);
        Offsets::Functions::PickupItem = pickupAddr;
    }

    // Find DropItem function
    uintptr_t dropAddr = mem.FindPattern(Patterns::DROPITEM_FUNCTION, Patterns::DROPITEM_FUNCTION_MASK);
    if (ValidateFunction(dropAddr, "DropItem")) {
        m_dropItemFn = reinterpret_cast<DropItemFn>(dropAddr);
        Offsets::Functions::DropItem = dropAddr;
    }

    // Find SendChat function
    uintptr_t sendChatAddr = mem.FindPattern(Patterns::SENDCHAT_FUNCTION, Patterns::SENDCHAT_FUNCTION_MASK);
    if (ValidateFunction(sendChatAddr, "SendChat")) {
        m_sendChatFn = reinterpret_cast<SendChatFn>(sendChatAddr);
        Offsets::Functions::SendChat = sendChatAddr;
    }

    Logger::Success("[GameFunctionHooks] All game functions found!");

    return true;
}

bool GameFunctionHooks::ValidateFunction(uintptr_t address, const char* name)
{
    if (address == 0) {
        Logger::Warning("[GameFunctionHooks] %s function not found (pattern failed)", name);
        return false;
    }

    Logger::Success("[GameFunctionHooks] Found %s at 0x%p", name, (void*)address);
    return true;
}

void GameFunctionHooks::SetError(const char* error)
{
    strncpy_s(m_lastError, error, sizeof(m_lastError) - 1);
    Logger::Error("[GameFunctionHooks] %s", error);
}

// ========================================
// Combat Functions
// ========================================

bool GameFunctionHooks::Attack(Entity* target)
{
    if (!m_initialized) {
        SetError("GameFunctionHooks not initialized!");
        return false;
    }

    if (!m_attackFn) {
        SetError("Attack function not found!");
        return false;
    }

    if (!CanPerformAction()) {
        SetError("Cannot perform action (player dead/stunned)");
        return false;
    }

    if (!target || !target->IsValid()) {
        SetError("Invalid target entity!");
        return false;
    }

    if (target->IsDead()) {
        SetError("Target is already dead!");
        return false;
    }

    try {
        // Get player base address (this pointer)
        void* playerPtr = reinterpret_cast<void*>(Player::Get()->GetBaseAddress());

        // Get target address
        void* targetPtr = reinterpret_cast<void*>(target->GetAddress());

        // Call game's Attack function
        // __thiscall: ECX = this, parameters on stack
        m_attackFn(playerPtr, targetPtr);

        Logger::Info("[GameFunctionHooks] Attack called on entity ID %u", target->GetID());
        return true;
    }
    catch (...) {
        SetError("Exception occurred during Attack call!");
        return false;
    }
}

bool GameFunctionHooks::AttackByID(uint32_t entityID)
{
    Entity entity = EntityManager::Get()->GetEntityByID(entityID);

    if (!entity.IsValid()) {
        SetError("Entity not found by ID!");
        return false;
    }

    return Attack(&entity);
}

bool GameFunctionHooks::UseSkill(uint32_t skillID, Entity* target)
{
    if (!m_initialized) {
        SetError("GameFunctionHooks not initialized!");
        return false;
    }

    if (!m_useSkillFn) {
        SetError("UseSkill function not found!");
        return false;
    }

    if (!CanPerformAction()) {
        SetError("Cannot perform action");
        return false;
    }

    try {
        void* playerPtr = reinterpret_cast<void*>(Player::Get()->GetBaseAddress());
        void* targetPtr = target ? reinterpret_cast<void*>(target->GetAddress()) : nullptr;

        // Call game's UseSkill function
        m_useSkillFn(playerPtr, skillID, targetPtr);

        Logger::Info("[GameFunctionHooks] UseSkill called (skillID: %u)", skillID);
        return true;
    }
    catch (...) {
        SetError("Exception occurred during UseSkill call!");
        return false;
    }
}

bool GameFunctionHooks::StopAttack()
{
    if (!m_initialized) {
        SetError("GameFunctionHooks not initialized!");
        return false;
    }

    if (!m_stopAttackFn) {
        SetError("StopAttack function not found!");
        return false;
    }

    try {
        void* playerPtr = reinterpret_cast<void*>(Player::Get()->GetBaseAddress());
        m_stopAttackFn(playerPtr);

        Logger::Info("[GameFunctionHooks] StopAttack called");
        return true;
    }
    catch (...) {
        SetError("Exception occurred during StopAttack call!");
        return false;
    }
}

// ========================================
// Movement Functions
// ========================================

bool GameFunctionHooks::MoveTo(float x, float y, float z)
{
    if (!m_initialized) {
        SetError("GameFunctionHooks not initialized!");
        return false;
    }

    if (!m_moveToFn) {
        SetError("MoveTo function not found!");
        return false;
    }

    if (!CanPerformAction()) {
        SetError("Cannot perform action");
        return false;
    }

    try {
        void* playerPtr = reinterpret_cast<void*>(Player::Get()->GetBaseAddress());
        m_moveToFn(playerPtr, x, y, z);

        Logger::Info("[GameFunctionHooks] MoveTo called (%.1f, %.1f, %.1f)", x, y, z);
        return true;
    }
    catch (...) {
        SetError("Exception occurred during MoveTo call!");
        return false;
    }
}

bool GameFunctionHooks::MoveToEntity(Entity* target)
{
    if (!target || !target->IsValid()) {
        SetError("Invalid target entity!");
        return false;
    }

    float x, y, z;
    target->GetPosition(x, y, z);

    return MoveTo(x, y, z);
}

bool GameFunctionHooks::Teleport(int mapID, float x, float y, float z)
{
    if (!m_initialized) {
        SetError("GameFunctionHooks not initialized!");
        return false;
    }

    if (!m_teleportFn) {
        SetError("Teleport function not found!");
        return false;
    }

    try {
        void* playerPtr = reinterpret_cast<void*>(Player::Get()->GetBaseAddress());
        m_teleportFn(playerPtr, mapID, x, y, z);

        Logger::Info("[GameFunctionHooks] Teleport called (map: %d, pos: %.1f, %.1f, %.1f)", mapID, x, y, z);
        return true;
    }
    catch (...) {
        SetError("Exception occurred during Teleport call!");
        return false;
    }
}

// ========================================
// Item Functions
// ========================================

bool GameFunctionHooks::UseItem(int slot)
{
    if (!m_initialized) {
        SetError("GameFunctionHooks not initialized!");
        return false;
    }

    if (!m_useItemFn) {
        SetError("UseItem function not found!");
        return false;
    }

    if (!CanPerformAction()) {
        SetError("Cannot perform action");
        return false;
    }

    if (slot < 0 || slot >= 40) {
        SetError("Invalid inventory slot!");
        return false;
    }

    try {
        void* playerPtr = reinterpret_cast<void*>(Player::Get()->GetBaseAddress());
        m_useItemFn(playerPtr, slot);

        Logger::Info("[GameFunctionHooks] UseItem called (slot: %d)", slot);
        return true;
    }
    catch (...) {
        SetError("Exception occurred during UseItem call!");
        return false;
    }
}

bool GameFunctionHooks::UseItemByID(uint32_t itemID)
{
    // Find item in inventory
    int slot = Inventory::Get()->FindItem(itemID);

    if (slot == -1) {
        SetError("Item not found in inventory!");
        return false;
    }

    return UseItem(slot);
}

bool GameFunctionHooks::PickupItem(uint32_t itemID)
{
    if (!m_initialized) {
        SetError("GameFunctionHooks not initialized!");
        return false;
    }

    if (!m_pickupItemFn) {
        SetError("PickupItem function not found!");
        return false;
    }

    if (!CanPerformAction()) {
        SetError("Cannot perform action");
        return false;
    }

    try {
        void* playerPtr = reinterpret_cast<void*>(Player::Get()->GetBaseAddress());
        m_pickupItemFn(playerPtr, itemID);

        Logger::Info("[GameFunctionHooks] PickupItem called (itemID: %u)", itemID);
        return true;
    }
    catch (...) {
        SetError("Exception occurred during PickupItem call!");
        return false;
    }
}

bool GameFunctionHooks::DropItem(int slot, int quantity)
{
    if (!m_initialized) {
        SetError("GameFunctionHooks not initialized!");
        return false;
    }

    if (!m_dropItemFn) {
        SetError("DropItem function not found!");
        return false;
    }

    if (!CanPerformAction()) {
        SetError("Cannot perform action");
        return false;
    }

    if (slot < 0 || slot >= 40) {
        SetError("Invalid inventory slot!");
        return false;
    }

    if (quantity <= 0) {
        SetError("Invalid quantity!");
        return false;
    }

    try {
        void* playerPtr = reinterpret_cast<void*>(Player::Get()->GetBaseAddress());
        m_dropItemFn(playerPtr, slot, quantity);

        Logger::Info("[GameFunctionHooks] DropItem called (slot: %d, qty: %d)", slot, quantity);
        return true;
    }
    catch (...) {
        SetError("Exception occurred during DropItem call!");
        return false;
    }
}

// ========================================
// Chat Functions
// ========================================

bool GameFunctionHooks::SendChat(const char* message)
{
    if (!m_initialized) {
        SetError("GameFunctionHooks not initialized!");
        return false;
    }

    if (!m_sendChatFn) {
        SetError("SendChat function not found!");
        return false;
    }

    if (!message || strlen(message) == 0) {
        SetError("Empty message!");
        return false;
    }

    try {
        // SendChat is typically __cdecl, not __thiscall
        m_sendChatFn(message);

        Logger::Info("[GameFunctionHooks] SendChat called: %s", message);
        return true;
    }
    catch (...) {
        SetError("Exception occurred during SendChat call!");
        return false;
    }
}

bool GameFunctionHooks::SendWhisper(const char* targetName, const char* message)
{
    if (!targetName || !message) {
        SetError("Invalid parameters!");
        return false;
    }

    // Format whisper message
    char whisperMsg[256];
    sprintf_s(whisperMsg, "/w %s %s", targetName, message);

    return SendChat(whisperMsg);
}

// ========================================
// Utility Functions
// ========================================

bool GameFunctionHooks::CanPerformAction() const
{
    Player* player = Player::Get();

    if (!player->IsValid()) {
        return false;
    }

    // Cannot perform actions if dead
    if (player->IsDead()) {
        return false;
    }

    // TODO: Add more checks in Phase 2
    // - Not stunned
    // - Not silenced (for skills)
    // - Not rooted (for movement)

    return true;
}

} // namespace JX1
