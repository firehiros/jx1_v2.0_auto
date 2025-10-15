# JX1 Memory Map - Reverse Engineering Findings

**Game Version**: [TODO: Add JX1 version]
**Date Started**: [TODO: Add date]
**Date Completed**: [TODO: Add date]
**Tools Used**: Cheat Engine 7.5, x64dbg (if used)

---

## 📋 Summary

This document contains all memory structures, offsets, and patterns discovered during Phase 2 reverse engineering.

**Status**: 🚧 In Progress

- [ ] Player structure fully mapped
- [ ] Entity manager fully mapped
- [ ] Inventory structure fully mapped
- [ ] Skill system fully mapped
- [ ] Critical functions discovered
- [ ] Patterns created and tested
- [ ] All findings verified after restart

---

## 🎮 Player Structure

### Base Pointer

**Static Address**: `"JX1.exe" + 0x????????`
**Pattern**: [TODO: Add pattern from Patterns.h]

**How Found**:
```
1. [Describe the process]
2. [Steps taken in Cheat Engine]
3. [Pointer chain found]
```

**Pointer Chain**:
```
[JX1.exe + 0x????????] → 0x???????? → Player Object
```

### Player Object Layout

```cpp
// Base: [JX1.exe + 0x????????]
struct Player {
    // VTable (if exists)
    +0x00: void* vtable;                    // [TODO: Find] Virtual function table

    // Identity
    +0x04: uint32_t entityID;               // [TODO: Find] Unique player ID
    +0x??: char name[32];                   // [TODO: Find] Character name

    // Health & Mana
    +0x??: int currentHP;                   // [TODO: Find] Current health points
    +0x??: int maxHP;                       // [TODO: Find] Maximum health points
    +0x??: int currentMP;                   // [TODO: Find] Current mana points
    +0x??: int maxMP;                       // [TODO: Find] Maximum mana points

    // Position (World coordinates)
    +0x??: float posX;                      // [TODO: Find] X coordinate
    +0x??: float posY;                      // [TODO: Find] Y coordinate
    +0x??: float posZ;                      // [TODO: Find] Z coordinate (height)
    +0x??: float direction;                 // [TODO: Find] Facing direction (0-360)

    // Character stats
    +0x??: int level;                       // [TODO: Find] Character level
    +0x??: int64_t experience;              // [TODO: Find] Current XP
    +0x??: int64_t gold;                    // [TODO: Find] Gold amount

    // State flags
    +0x??: bool isDead;                     // [TODO: Find] Death state
    +0x??: bool inCombat;                   // [TODO: Find] Combat state
    +0x??: bool isMoving;                   // [TODO: Find] Movement state

    // Pointers to other structures
    +0x??: Inventory* pInventory;           // [TODO: Find] Pointer to inventory
    +0x??: SkillManager* pSkills;           // [TODO: Find] Pointer to skills
    +0x??: EquipmentManager* pEquipment;    // [TODO: Find] Pointer to equipment
};

// Total Size: 0x??? bytes
```

**Verification Steps**:
- [x] HP changes when taking damage - **Result**: [TODO: Add result]
- [x] MP changes when using skills - **Result**: [TODO: Add result]
- [x] Position changes when moving - **Result**: [TODO: Add result]
- [x] Values persist after game restart (using pattern) - **Result**: [TODO: Add result]

**Notes**:
```
[Add any special observations, quirks, or important details]
```

---

## 👾 Entity Manager Structure

### Base Pointer

**Static Address**: `"JX1.exe" + 0x????????`
**Pattern**: [TODO: Add pattern from Patterns.h]

**How Found**:
```
1. [Describe the process]
2. [Method used to discover entity list]
```

### Entity Manager Layout

```cpp
// Base: [JX1.exe + 0x????????]
struct EntityManager {
    +0x??: Entity** entityArray;           // [TODO: Find] Array of entity pointers
    +0x??: int entityCount;                // [TODO: Find] Current number of entities
    +0x??: int maxEntities;                // [TODO: Find] Maximum capacity
};
```

### Entity Object Layout

```cpp
// Each entity in the array
struct Entity {
    // Identity
    +0x00: void* vtable;                    // [TODO: Find] Virtual function table
    +0x??: uint32_t entityID;               // [TODO: Find] Unique entity ID
    +0x??: uint32_t entityType;             // [TODO: Find] Type: 1=Player, 2=Monster, 3=NPC, 4=Item
    +0x??: char name[64];                   // [TODO: Find] Entity name

    // Health
    +0x??: int currentHP;                   // [TODO: Find] Current HP
    +0x??: int maxHP;                       // [TODO: Find] Max HP

    // Position
    +0x??: float posX;                      // [TODO: Find] X coordinate
    +0x??: float posY;                      // [TODO: Find] Y coordinate
    +0x??: float posZ;                      // [TODO: Find] Z coordinate

    // State
    +0x??: bool isDead;                     // [TODO: Find] Is entity dead
    +0x??: bool isHostile;                  // [TODO: Find] Is hostile (for monsters)
    +0x??: int level;                       // [TODO: Find] Entity level
};

// Total Size: 0x??? bytes
```

**Verification Steps**:
- [ ] Entity count increases when NPC/monster spawns
- [ ] Entity count decreases when entity dies/despawns
- [ ] Can read entity names correctly
- [ ] Can read entity positions correctly

**Notes**:
```
[Add observations about entity lifecycle, spawn/despawn behavior]
```

---

## 🎒 Inventory Structure

### Inventory Manager

```cpp
// Pointer from Player object
struct Inventory {
    +0x??: int itemCount;                   // [TODO: Find] Current number of items
    +0x??: int maxSlots;                    // [TODO: Find] Maximum inventory slots
    +0x??: Item* items[MAX_SLOTS];          // [TODO: Find] Array of item pointers
};
```

### Item Object Layout

```cpp
struct Item {
    +0x??: uint32_t itemID;                 // [TODO: Find] Unique item ID
    +0x??: uint32_t itemType;               // [TODO: Find] Type: weapon, armor, potion, etc.
    +0x??: int quantity;                    // [TODO: Find] Stack quantity
    +0x??: int quality;                     // [TODO: Find] Quality: 1=Grey, 2=White, 3=Blue, 4=Purple, 5=Gold
    +0x??: int durability;                  // [TODO: Find] Current durability
    +0x??: int maxDurability;               // [TODO: Find] Maximum durability
    +0x??: char name[64];                   // [TODO: Find] Item name
};
```

**Notes**:
```
[Add notes about inventory behavior, slot indexing, special cases]
```

---

## 🔮 Skill System Structure

### Skill Manager

```cpp
// Pointer from Player object
struct SkillManager {
    +0x??: int skillCount;                  // [TODO: Find] Number of learned skills
    +0x??: Skill* skills[MAX_SKILLS];       // [TODO: Find] Array of skill pointers
};
```

### Skill Object Layout

```cpp
struct Skill {
    +0x??: uint32_t skillID;                // [TODO: Find] Unique skill ID
    +0x??: int skillLevel;                  // [TODO: Find] Skill level
    +0x??: float cooldown;                  // [TODO: Find] Remaining cooldown (seconds)
    +0x??: int manaCost;                    // [TODO: Find] Mana cost to cast
    +0x??: char name[64];                   // [TODO: Find] Skill name
    +0x??: int castTime;                    // [TODO: Find] Cast time (ms)
};
```

**Notes**:
```
[Add notes about skill casting, cooldown behavior, skill effects]
```

---

## 🎯 Function Addresses

### Combat Functions

#### Attack Function
**Address**: `0x????????` (found via pattern)
**Pattern**: [TODO: Add from Patterns.h]
**Signature**: `void __thiscall Player::Attack(Entity* target)`

**Assembly**:
```asm
00401000  55              push ebp
00401001  8B EC           mov ebp,esp
00401003  83 EC 20        sub esp,20
00401006  56              push esi
00401007  8B F1           mov esi,ecx        ; this pointer
...
```

**How Found**:
```
1. [Describe method: Find what writes when attacking]
2. [Steps taken]
```

**Parameters**:
- ECX: `Player*` (this pointer)
- Stack+0: `Entity*` target

**Notes**:
```
[Add notes about behavior, preconditions, side effects]
```

---

#### UseSkill Function
**Address**: `0x????????`
**Pattern**: [TODO: Add from Patterns.h]
**Signature**: `void __thiscall Player::UseSkill(int skillID, Entity* target)`

**Assembly**:
```asm
[TODO: Add assembly code]
```

**Parameters**:
- ECX: `Player*` (this pointer)
- Stack+0: `int` skillID
- Stack+4: `Entity*` target

---

### Movement Functions

#### MoveTo Function
**Address**: `0x????????`
**Pattern**: [TODO: Add from Patterns.h]
**Signature**: `void __thiscall Player::MoveTo(float x, float y, float z)`

**Assembly**:
```asm
[TODO: Add assembly code]
```

**Parameters**:
- ECX: `Player*` (this pointer)
- Stack+0: `float` x
- Stack+4: `float` y
- Stack+8: `float` z

---

### Item Functions

#### UseItem Function
**Address**: `0x????????`
**Pattern**: [TODO: Add from Patterns.h]
**Signature**: `void __thiscall Player::UseItem(int slotIndex)`

**Assembly**:
```asm
[TODO: Add assembly code]
```

---

#### PickupItem Function
**Address**: `0x????????`
**Pattern**: [TODO: Add from Patterns.h]
**Signature**: `void __thiscall Player::PickupItem(int itemID)`

**Assembly**:
```asm
[TODO: Add assembly code]
```

---

### Chat Functions

#### SendChat Function
**Address**: `0x????????`
**Pattern**: [TODO: Add from Patterns.h]
**Signature**: `void __cdecl Chat::SendMessage(const char* message)`

**Assembly**:
```asm
[TODO: Add assembly code]
```

---

## 🔍 Pattern Signatures Summary

All patterns are documented in `core_dll/src/utils/Patterns.h`

| Name | Pattern | Status | Verified |
|------|---------|--------|----------|
| PLAYER_BASE | [TODO] | ❌ Not Found | ❌ |
| ENTITY_MANAGER | [TODO] | ❌ Not Found | ❌ |
| ATTACK_FUNCTION | [TODO] | ❌ Not Found | ❌ |
| MOVETO_FUNCTION | [TODO] | ❌ Not Found | ❌ |
| USESKILL_FUNCTION | [TODO] | ❌ Not Found | ❌ |
| PICKUP_FUNCTION | [TODO] | ❌ Not Found | ❌ |
| USEITEM_FUNCTION | [TODO] | ❌ Not Found | ❌ |
| SENDCHAT_FUNCTION | [TODO] | ❌ Not Found | ❌ |

---

## 📊 Offset Summary Table

Quick reference for all offsets found:

| Structure | Field | Offset | Type | Verified |
|-----------|-------|--------|------|----------|
| Player | HP | +0x?? | int | ❌ |
| Player | Max HP | +0x?? | int | ❌ |
| Player | MP | +0x?? | int | ❌ |
| Player | Max MP | +0x?? | int | ❌ |
| Player | Position X | +0x?? | float | ❌ |
| Player | Position Y | +0x?? | float | ❌ |
| Player | Position Z | +0x?? | float | ❌ |
| Player | Level | +0x?? | int | ❌ |
| Player | Experience | +0x?? | int64 | ❌ |
| Player | Gold | +0x?? | int64 | ❌ |
| Entity | Entity ID | +0x?? | uint32 | ❌ |
| Entity | Entity Type | +0x?? | uint32 | ❌ |
| Entity | HP | +0x?? | int | ❌ |
| Entity | Position X | +0x?? | float | ❌ |

---

## 🧪 Testing & Verification

### Test Cases

1. **Player HP Test**
   - [ ] Read HP value
   - [ ] Take damage in game
   - [ ] Verify HP decreases
   - [ ] Heal character
   - [ ] Verify HP increases
   - [ ] Restart game, re-scan with pattern
   - [ ] Verify pattern finds correct address

2. **Position Test**
   - [ ] Read X, Y, Z coordinates
   - [ ] Move character north
   - [ ] Verify Y coordinate changes
   - [ ] Move character east
   - [ ] Verify X coordinate changes
   - [ ] Jump/climb
   - [ ] Verify Z coordinate changes

3. **Entity Test**
   - [ ] Read entity count
   - [ ] Spawn NPC/monster
   - [ ] Verify entity count increases
   - [ ] Kill entity
   - [ ] Verify entity count decreases
   - [ ] Read entity name
   - [ ] Verify name matches in-game

4. **Function Hook Test**
   - [ ] Find Attack function
   - [ ] Hook function with logging
   - [ ] Trigger attack in game
   - [ ] Verify hook is called
   - [ ] Verify parameters are correct

---

## 🐛 Known Issues & Quirks

### Issue 1: [TODO: Add issue title]
**Description**: [Describe the issue]
**Workaround**: [If any]
**Impact**: [Low/Medium/High]

### Issue 2: [TODO: Add issue title]
**Description**: [Describe the issue]
**Workaround**: [If any]
**Impact**: [Low/Medium/High]

---

## 📝 Additional Notes

### Memory Protection
```
[Note any memory regions that are protected or require special handling]
```

### Anti-Cheat Considerations
```
[Note any anti-cheat mechanisms observed, if any]
[Publisher partnership should minimize this concern]
```

### Performance Notes
```
[Note any performance considerations, hot paths, frequently-called functions]
```

### Version Differences
```
[If testing multiple game versions, note any differences]
```

---

## 📚 Resources & References

- Cheat Engine Table: `[path/to/JX1_v2.0_auto.CT]`
- x64dbg Database: `[path/to/JX1.dd32]` (if used)
- Screenshots: `[path/to/screenshots/]`
- Video Recordings: `[path/to/recordings/]`

---

## ✅ Phase 2 Completion Checklist

- [ ] Player base pointer found and pattern created
- [ ] All Player offsets documented
- [ ] Entity manager found and pattern created
- [ ] Entity structure documented
- [ ] Inventory structure documented
- [ ] Skill system documented
- [ ] Attack function found and pattern created
- [ ] MoveTo function found and pattern created
- [ ] UseSkill function found and pattern created
- [ ] UseItem function found and pattern created
- [ ] PickupItem function found and pattern created
- [ ] All patterns tested after game restart
- [ ] All offsets updated in Offsets.h
- [ ] All patterns updated in Patterns.h
- [ ] Cheat Engine table saved
- [ ] This document completed

**Completion Date**: [TODO: Add date when all checkboxes are complete]

---

## 🚀 Next Steps (Phase 3)

Once Phase 2 is complete:

1. Implement `Player` class using found offsets
2. Implement `Entity` class using found offsets
3. Implement pattern scanning in `MemoryManager::Initialize()`
4. Test reading player HP/MP/Position in real-time
5. Test reading entity list
6. Prepare for DirectX hook implementation (Phase 4)

---

**Last Updated**: [TODO: Update date]
**Status**: 🚧 Phase 2 In Progress
