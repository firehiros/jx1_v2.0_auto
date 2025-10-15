/**
 * @file Inventory.h
 * @brief Inventory and Item management
 *
 * Phase 3 - Game State Reading
 *
 * Provides access to player inventory:
 * - Read items in inventory
 * - Check item availability
 * - Find empty slots
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include "../utils/Offsets.h"
#include "../core/MemoryManager.h"

namespace JX1 {

/**
 * Item struct - Represents an inventory item
 */
struct Item
{
    uint32_t itemID = 0;
    uint32_t itemType = 0;
    int quantity = 0;
    int quality = 0;  // 1=Grey, 2=White, 3=Blue, 4=Purple, 5=Gold
    std::string name;

    bool IsValid() const { return itemID != 0; }
    bool IsEmpty() const { return itemID == 0; }
};

/**
 * Inventory class - Manages player inventory
 *
 * Usage:
 *   Inventory* inv = Inventory::Get();
 *   if (inv->IsValid()) {
 *       int potions = inv->CountItem(POTION_ID);
 *       if (potions > 0) {
 *           // Use potion
 *       }
 *   }
 */
class Inventory
{
public:
    /**
     * Get singleton instance
     */
    static Inventory* Get();

    /**
     * Initialize inventory (find inventory base from player)
     */
    bool Initialize();

    /**
     * Refresh inventory
     */
    bool Refresh();

    /**
     * Check if inventory is valid
     */
    bool IsValid() const;

    // ========================================
    // Get Items
    // ========================================

    /**
     * Get item at specific slot
     * @param slot Slot index (0-based)
     */
    Item GetItem(int slot) const;

    /**
     * Get all items in inventory
     */
    std::vector<Item> GetAllItems() const;

    /**
     * Get item count
     */
    int GetItemCount() const;

    /**
     * Get max inventory capacity
     */
    int GetMaxCapacity() const;

    // ========================================
    // Check Items
    // ========================================

    /**
     * Check if inventory has specific item
     */
    bool HasItem(uint32_t itemID) const;

    /**
     * Count how many of specific item
     */
    int CountItem(uint32_t itemID) const;

    /**
     * Check if inventory is full
     */
    bool IsFull() const;

    /**
     * Check if inventory is empty
     */
    bool IsEmpty() const;

    // ========================================
    // Find Items
    // ========================================

    /**
     * Find first slot containing item ID
     * @return Slot index, or -1 if not found
     */
    int FindItem(uint32_t itemID) const;

    /**
     * Find all slots containing item ID
     */
    std::vector<int> FindAllItems(uint32_t itemID) const;

    /**
     * Find first empty slot
     * @return Slot index, or -1 if inventory full
     */
    int FindEmptySlot() const;

    /**
     * Find items by type
     */
    std::vector<Item> FindItemsByType(uint32_t itemType) const;

    // ========================================
    // Advanced
    // ========================================

    /**
     * Get raw inventory base address
     */
    uintptr_t GetBaseAddress() const { return m_baseAddress; }

    /**
     * Dump inventory contents to log
     */
    void DumpInventory() const;

private:
    Inventory() = default;
    ~Inventory() = default;
    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    uintptr_t m_baseAddress = 0;

    /**
     * Helper: Get item address at slot
     */
    uintptr_t GetItemAddress(int slot) const;
};

} // namespace JX1
