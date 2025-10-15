/**
 * @file Inventory.cpp
 * @brief Inventory implementation
 *
 * Phase 3 - Game State Reading
 */

#include "Inventory.h"
#include "Player.h"
#include "../core/Logger.h"

namespace JX1 {

Inventory* Inventory::Get()
{
    static Inventory instance;
    return &instance;
}

bool Inventory::Initialize()
{
    Logger::Info("[Inventory] Initializing inventory...");

    // Inventory is typically accessed through the Player object
    // Player + InventoryOffset = Inventory pointer
    Player* player = Player::Get();

    if (!player->IsValid()) {
        Logger::Error("[Inventory] Cannot initialize inventory - player not valid!");
        return false;
    }

    // Read inventory pointer from player
    // Note: Offsets::Inventory offsets are relative to PLAYER base, not inventory base
    // We need to read the inventory pointer first
    // This might be: Player + SOME_OFFSET = InventoryPointer
    // For now, assume inventory is embedded in player structure

    m_baseAddress = player->GetBaseAddress();

    if (!m_baseAddress) {
        Logger::Error("[Inventory] Inventory base address is null!");
        return false;
    }

    Logger::Success("[Inventory] Inventory initialized at 0x%p", (void*)m_baseAddress);

    return true;
}

bool Inventory::Refresh()
{
    return Initialize();
}

bool Inventory::IsValid() const
{
    return m_baseAddress != 0;
}

// ========================================
// Get Items
// ========================================

Item Inventory::GetItem(int slot) const
{
    Item item;

    if (!IsValid()) return item;
    if (slot < 0 || slot >= GetMaxCapacity()) return item;

    uintptr_t itemAddress = GetItemAddress(slot);
    if (!itemAddress) return item;

    item.itemID = MemoryManager::Get().Read<uint32_t>(itemAddress + Offsets::Item::ITEM_ID);
    item.itemType = MemoryManager::Get().Read<uint32_t>(itemAddress + Offsets::Item::ITEM_TYPE);
    item.quantity = MemoryManager::Get().Read<int>(itemAddress + Offsets::Item::QUANTITY);
    item.quality = MemoryManager::Get().Read<int>(itemAddress + Offsets::Item::QUALITY);

    // TODO: Read item name if offset is found
    // For now, use itemID as name
    char nameBuf[32];
    sprintf_s(nameBuf, "Item_%u", item.itemID);
    item.name = nameBuf;

    return item;
}

std::vector<Item> Inventory::GetAllItems() const
{
    std::vector<Item> items;

    if (!IsValid()) return items;

    int maxCapacity = GetMaxCapacity();
    items.reserve(maxCapacity);

    for (int i = 0; i < maxCapacity; ++i) {
        Item item = GetItem(i);
        items.push_back(item);
    }

    return items;
}

int Inventory::GetItemCount() const
{
    if (!IsValid()) return 0;

    // Count non-empty items
    int count = 0;
    int maxCapacity = GetMaxCapacity();

    for (int i = 0; i < maxCapacity; ++i) {
        Item item = GetItem(i);
        if (item.IsValid()) {
            count++;
        }
    }

    return count;

    // Alternative: If game has item count field:
    // return MemoryManager::Get().Read<int>(m_baseAddress + Offsets::Inventory::ITEM_COUNT);
}

int Inventory::GetMaxCapacity() const
{
    if (!IsValid()) return 0;

    // Read from memory if offset is known
    // return MemoryManager::Get().Read<int>(m_baseAddress + Offsets::Inventory::MAX_ITEMS);

    // Default: assume 40 slots (common in MMORPGs)
    // This should be updated in Phase 2 with actual value
    return 40;
}

// ========================================
// Check Items
// ========================================

bool Inventory::HasItem(uint32_t itemID) const
{
    return FindItem(itemID) != -1;
}

int Inventory::CountItem(uint32_t itemID) const
{
    if (!IsValid()) return 0;

    int totalCount = 0;
    int maxCapacity = GetMaxCapacity();

    for (int i = 0; i < maxCapacity; ++i) {
        Item item = GetItem(i);
        if (item.itemID == itemID) {
            totalCount += item.quantity;
        }
    }

    return totalCount;
}

bool Inventory::IsFull() const
{
    return GetItemCount() >= GetMaxCapacity();
}

bool Inventory::IsEmpty() const
{
    return GetItemCount() == 0;
}

// ========================================
// Find Items
// ========================================

int Inventory::FindItem(uint32_t itemID) const
{
    if (!IsValid()) return -1;

    int maxCapacity = GetMaxCapacity();

    for (int i = 0; i < maxCapacity; ++i) {
        Item item = GetItem(i);
        if (item.itemID == itemID) {
            return i;
        }
    }

    return -1;
}

std::vector<int> Inventory::FindAllItems(uint32_t itemID) const
{
    std::vector<int> slots;

    if (!IsValid()) return slots;

    int maxCapacity = GetMaxCapacity();

    for (int i = 0; i < maxCapacity; ++i) {
        Item item = GetItem(i);
        if (item.itemID == itemID) {
            slots.push_back(i);
        }
    }

    return slots;
}

int Inventory::FindEmptySlot() const
{
    if (!IsValid()) return -1;

    int maxCapacity = GetMaxCapacity();

    for (int i = 0; i < maxCapacity; ++i) {
        Item item = GetItem(i);
        if (item.IsEmpty()) {
            return i;
        }
    }

    return -1;
}

std::vector<Item> Inventory::FindItemsByType(uint32_t itemType) const
{
    std::vector<Item> result;

    if (!IsValid()) return result;

    int maxCapacity = GetMaxCapacity();

    for (int i = 0; i < maxCapacity; ++i) {
        Item item = GetItem(i);
        if (item.IsValid() && item.itemType == itemType) {
            result.push_back(item);
        }
    }

    return result;
}

// ========================================
// Advanced
// ========================================

void Inventory::DumpInventory() const
{
    if (!IsValid()) {
        Logger::Warning("[Inventory] Cannot dump inventory - not valid!");
        return;
    }

    Logger::Info("========== INVENTORY ==========");
    Logger::Info("Base Address: 0x%p", (void*)m_baseAddress);
    Logger::Info("Item Count: %d / %d", GetItemCount(), GetMaxCapacity());
    Logger::Info("Is Full: %s", IsFull() ? "Yes" : "No");
    Logger::Info("Is Empty: %s", IsEmpty() ? "Yes" : "No");

    Logger::Info("Items:");

    int maxCapacity = GetMaxCapacity();
    for (int i = 0; i < maxCapacity; ++i) {
        Item item = GetItem(i);
        if (item.IsValid()) {
            const char* qualityStr = "Unknown";
            switch (item.quality) {
                case 1: qualityStr = "Grey"; break;
                case 2: qualityStr = "White"; break;
                case 3: qualityStr = "Blue"; break;
                case 4: qualityStr = "Purple"; break;
                case 5: qualityStr = "Gold"; break;
                default: break;
            }

            Logger::Info("  [%d] %s (ID: %u, Type: %u, Qty: %d, Quality: %s)",
                i,
                item.name.c_str(),
                item.itemID,
                item.itemType,
                item.quantity,
                qualityStr
            );
        }
    }

    Logger::Info("===============================");
}

// ========================================
// Private Helpers
// ========================================

uintptr_t Inventory::GetItemAddress(int slot) const
{
    if (!IsValid()) return 0;
    if (slot < 0 || slot >= GetMaxCapacity()) return 0;

    // Inventory items array is typically:
    // InventoryBase + ITEMS_ARRAY + (slot * ItemStructSize)

    // Item structure size - should be determined in Phase 2
    // For now, assume 0x20 (32 bytes) per item
    const int ITEM_STRUCT_SIZE = 0x20;

    uintptr_t itemsArrayBase = m_baseAddress + Offsets::Inventory::ITEMS_ARRAY;
    uintptr_t itemAddress = itemsArrayBase + (slot * ITEM_STRUCT_SIZE);

    return itemAddress;
}

} // namespace JX1
