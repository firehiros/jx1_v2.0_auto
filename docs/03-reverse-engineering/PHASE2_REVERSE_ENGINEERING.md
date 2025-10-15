# Phase 2: Reverse Engineering Guide

**Timeline:** Week 4-5 (2 weeks)
**Tools Required:** Cheat Engine, x64dbg (optional)
**Prerequisites:** Windows + JX1 game installed

---

## 📋 Overview

Phase 2 mục tiêu:
1. Find Player structure (HP, MP, Position, Level...)
2. Find Entity list (Monsters, NPCs, Players nearby)
3. Find critical game functions (Attack, Move, UseSkill...)
4. Create pattern signatures for stability
5. Document everything

**Deliverables:**
- `core_dll/src/utils/Offsets.h` - All memory offsets
- `core_dll/src/utils/Patterns.h` - Pattern signatures
- `docs/MEMORY_MAP.md` - Complete memory documentation

---

## 🛠️ Tools Setup

### 1. Cheat Engine

**Download:** https://www.cheatengine.org/

**Installation:**
```
1. Download latest version (7.5+)
2. Install with default settings
3. Launch Cheat Engine
4. Familiarize with interface
```

**Key Features We'll Use:**
- Memory Scanner
- Pointer Scanner
- Dissect Data Structures
- Code Injection
- Save/Load Cheat Tables

### 2. x64dbg (Optional but Recommended)

**Download:** https://x64dbg.com/

**Usage:**
- Advanced debugging
- Finding function signatures
- Analyzing assembly code

### 3. Process Hacker (Optional)

**Download:** https://processhacker.sourceforge.io/

**Usage:**
- Better than Task Manager
- View loaded modules
- Memory viewer

---

## 📚 Cheat Engine Tutorial

### Basic Workflow

1. **Attach to Process**
   ```
   - Launch JX1 game
   - Cheat Engine → File → Open Process
   - Select JX1.exe
   ```

2. **Scan for Value**
   ```
   - See current HP: 1500
   - Scan Type: Exact Value
   - Value: 1500
   - Click "First Scan"
   ```

3. **Filter Results**
   ```
   - Take damage → HP: 1350
   - Value: 1350
   - Click "Next Scan"
   - Repeat until 1-10 addresses remain
   ```

4. **Find Pointer**
   ```
   - Right-click address → Pointer scan for this address
   - Save pointer scan
   - Restart game
   - Load pointer scan
   - Rescan to find static pointer
   ```

---

## 🎯 Task 1: Find Player Structure

### 1.1 Find Player HP

**Steps:**

```
1. Launch JX1 and login to a character
2. Note current HP (e.g., 1500/2000)
3. Cheat Engine → Attach to JX1.exe
4. Scan for current HP value (1500)
   - Value type: 4 Bytes
   - Scan type: Exact Value
5. Take damage or heal
6. Scan again for new value
7. Repeat until you have 1-5 addresses
```

**Verify:**
```
- Double-click address to add to address list
- Change value (e.g., set to 9999)
- Check if HP in game changes
- If yes, you found it! ✅
```

**Document:**
```cpp
// In your notes:
// HP Address: 0x12345678 (will change each restart)
// Need to find pointer!
```

### 1.2 Find Static Pointer to HP

**Why?** Memory addresses change when game restarts. We need static pointer.

**Steps:**

```
1. Right-click HP address → "Pointer scan for this address"
2. Save pointer scan (e.g., hp_pointers.PTR)
3. Wait for scan to complete (might take 5-10 minutes)
4. Restart game
5. Load pointer scan file
6. Find new HP address (using same method)
7. Rescan pointer scan with new address
8. Addresses that remain are static pointers!
```

**Result:**
```
Static Pointer Chain Example:
"JX1.exe"+0x123456 → +0x10 → +0x8 → +0x4 = HP

Base: JX1.exe + 0x123456
Offsets: 0x10, 0x8, 0x4
```

**Document:**
```cpp
// Offsets.h
constexpr uintptr_t OFFSET_PLAYER_BASE = 0x123456;
constexpr uintptr_t OFFSET_HP = 0x4;  // Final offset in chain
```

### 1.3 Find Other Player Stats

Use same method for:

**HP/MP:**
- Current HP
- Max HP
- Current MP
- Max MP

**Position:**
- Position X (Float)
- Position Y (Float)
- Position Z (Float)
- Direction/Rotation (Float)

**Character Info:**
- Level (4 bytes)
- Experience (4 bytes)
- Gold (4 bytes)
- Character Name (String)

**Tips:**
- Float values: Use "Float" type in Cheat Engine
- Position: Try scanning when standing still, then move
- Name: Use "String" type, search for your character name

### 1.4 Analyze Player Structure

**Use "Dissect Data Structures":**

```
1. Find Player base address (from pointer)
2. Right-click → "Browse this memory region"
3. Tools → "Dissect data structures"
4. Define structure size (e.g., 0x500 bytes)
5. Analyze offsets
```

**Common Pattern:**
```cpp
struct Player {
    // Offset 0x00
    void* vtable;

    // Offset 0x10-0x20 (Stats)
    int currentHP;
    int maxHP;
    int currentMP;
    int maxMP;

    // Offset 0x30-0x40 (Position)
    float posX;
    float posY;
    float posZ;
    float direction;

    // Offset 0x50-0x60 (Character info)
    int level;
    int experience;
    int gold;

    // Offset 0x100+ (Name, etc)
    char name[32];

    // ... more fields
};
```

---

## 🎯 Task 2: Find Entity List

### 2.1 Find Monster HP

**Steps:**

```
1. Find a monster
2. Attack it to see HP bar
3. Estimate HP (e.g., 500)
4. Scan for monster HP
   - Scan type: Unknown initial value
   - Value type: 4 bytes
5. Attack again → HP decreased
6. Next Scan: Decreased value
7. Repeat until found
```

### 2.2 Find Entity List Structure

**Method 1: From Monster Address**

```
1. Find monster address (e.g., 0x23456789)
2. Search nearby memory for this address
   - Scan type: Exact value
   - Value type: 4 bytes
   - Value: 0x23456789
3. This might be pointer in entity list
```

**Method 2: Find Entity Count**

```
1. Count monsters nearby (e.g., 5)
2. Scan for value: 5
3. Kill one monster
4. Scan for value: 4
5. Find entity count address
6. Nearby should be entity list pointer
```

**Common Structure:**
```cpp
struct EntityManager {
    Entity** entityList;      // Array of pointers
    int entityCount;          // Number of entities
    int entityCapacity;       // Max capacity
    // ...
};
```

### 2.3 Analyze Entity Structure

```cpp
struct Entity {
    void* vtable;
    int entityID;
    int entityType;          // 1=Player, 2=Monster, 3=NPC, 4=Item
    float posX;
    float posY;
    float posZ;
    int currentHP;
    int maxHP;
    char name[64];
    // ...
};
```

**Verify:**
- Change entity HP → See if monster HP changes in game
- Change entity position → See if monster moves
- Check entity type value

---

## 🎯 Task 3: Find Critical Functions

### 3.1 Find Attack Function

**Method: Code Injection**

```
1. Find Player HP address
2. Right-click → "Find out what writes to this address"
3. Attack a monster
4. Cheat Engine will show the instruction
5. Click "Show disassembler"
6. This is near/inside the attack function!
```

**Example:**
```asm
; Found instruction at 0x00401234
00401234: mov [eax+10],edx    ; Writing HP
00401237: call 0x00402000     ; Might be attack function
```

**Document:**
```cpp
// Function signatures
// Attack function: 0x00402000
// Signature: 55 8B EC 83 EC ?? 56 8B F1 (example)
```

### 3.2 Find Other Functions

**Functions to Find:**
- Attack function
- MoveTo function
- UseSkill function
- PickupItem function
- UseItem function

**Method:**
```
1. Use "Find what writes to X"
2. Use "Find what accesses X"
3. Analyze nearby calls
4. Test by replacing instructions
```

---

## 🎯 Task 4: Create Pattern Signatures

### 4.1 Why Patterns?

Absolute addresses change when:
- Game updates
- Game restarts (ASLR)

Pattern signatures are stable.

### 4.2 How to Create Pattern

**Example:**

```asm
; Function at 0x00401234
00401234: 55              push ebp
00401235: 8B EC           mov ebp,esp
00401237: 83 EC 20        sub esp,20
0040123A: 56              push esi
0040123B: 8B F1           mov esi,ecx
```

**Pattern:**
```
Bytes:  55 8B EC 83 EC 20 56 8B F1
Mask:   xxxxxxxxx

// Or with wildcards for dynamic values:
// If 0x20 might change:
Bytes:  55 8B EC 83 EC ?? 56 8B F1
Mask:   xxxxx?xxx
```

**In Code:**
```cpp
// Patterns.h
namespace Patterns {
    constexpr const char* PLAYER_BASE = "\x8B\x0D\x00\x00\x00\x00\x85\xC9";
    constexpr const char* PLAYER_BASE_MASK = "xx????xx";

    constexpr const char* ATTACK_FUNC = "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1";
    constexpr const char* ATTACK_FUNC_MASK = "xxxxx?xxx";
}
```

### 4.3 Extract Pattern from Address

**In Cheat Engine:**

```
1. Memory View → Go to address
2. Select 10-20 bytes
3. Copy to clipboard
4. Convert to pattern format
```

**Tools:**
- IDA Pro / Ghidra (advanced)
- Online pattern generators

---

## 📝 Documentation Template

### Offsets.h

```cpp
#pragma once

namespace JX1 {
namespace Offsets {

    // ========================================
    // Player Offsets
    // ========================================

    // Player Base Pointer
    // Pattern: "JX1.exe"+0x123456
    constexpr uintptr_t PLAYER_BASE = 0x123456;

    // Player Structure Offsets
    struct Player {
        static constexpr int HP = 0x10;
        static constexpr int MAX_HP = 0x14;
        static constexpr int MP = 0x18;
        static constexpr int MAX_MP = 0x1C;

        static constexpr int POS_X = 0x30;
        static constexpr int POS_Y = 0x34;
        static constexpr int POS_Z = 0x38;
        static constexpr int DIRECTION = 0x3C;

        static constexpr int LEVEL = 0x50;
        static constexpr int EXPERIENCE = 0x54;
        static constexpr int GOLD = 0x58;

        static constexpr int NAME = 0x100;
    };

    // ========================================
    // Entity Manager Offsets
    // ========================================

    constexpr uintptr_t ENTITY_MANAGER_BASE = 0x234567;

    struct EntityManager {
        static constexpr int ENTITY_LIST = 0x0;
        static constexpr int ENTITY_COUNT = 0x4;
        static constexpr int ENTITY_CAPACITY = 0x8;
    };

    // ========================================
    // Entity Structure Offsets
    // ========================================

    struct Entity {
        static constexpr int ENTITY_ID = 0x4;
        static constexpr int ENTITY_TYPE = 0x8;

        static constexpr int HP = 0x10;
        static constexpr int MAX_HP = 0x14;

        static constexpr int POS_X = 0x20;
        static constexpr int POS_Y = 0x24;
        static constexpr int POS_Z = 0x28;

        static constexpr int NAME = 0x40;
    };

    // ========================================
    // Function Addresses (will use patterns)
    // ========================================

    struct Functions {
        // These will be found at runtime using patterns
        static uintptr_t Attack;
        static uintptr_t MoveTo;
        static uintptr_t UseSkill;
        static uintptr_t PickupItem;
    };

} // namespace Offsets
} // namespace JX1
```

### Patterns.h

```cpp
#pragma once

namespace JX1 {
namespace Patterns {

    // ========================================
    // Static Pointers
    // ========================================

    // Player Base Pointer
    // Signature: mov ecx,[JX1.exe+XXXXXX]
    constexpr const char* PLAYER_BASE = "\x8B\x0D\x00\x00\x00\x00\x85\xC9\x74";
    constexpr const char* PLAYER_BASE_MASK = "xx????xxx";

    // Entity Manager Pointer
    constexpr const char* ENTITY_MANAGER = "\xA1\x00\x00\x00\x00\x85\xC0\x74\x00\x8B\x48";
    constexpr const char* ENTITY_MANAGER_MASK = "x????xxx?xx";

    // ========================================
    // Function Signatures
    // ========================================

    // Attack Function
    // Pattern: 55 8B EC 83 EC ?? 56 8B F1 89 75 ?? E8
    constexpr const char* ATTACK_FUNCTION = "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1\x89\x75\x00\xE8";
    constexpr const char* ATTACK_FUNCTION_MASK = "xxxxx?xxxxx?x";

    // MoveTo Function
    constexpr const char* MOVETO_FUNCTION = "\x55\x8B\xEC\x83\xEC\x00\x8B\x45\x00\x53\x56\x57";
    constexpr const char* MOVETO_FUNCTION_MASK = "xxxxx?xx?xxx";

    // UseSkill Function
    constexpr const char* USESKILL_FUNCTION = "\x55\x8B\xEC\x83\xEC\x00\x8B\x4D\x00\x89\x4D";
    constexpr const char* USESKILL_FUNCTION_MASK = "xxxxx?xx?xx";

} // namespace Patterns
} // namespace JX1
```

---

## 📊 Progress Tracking

### Phase 2 Checklist

```
Week 4:
[ ] Setup Cheat Engine
[ ] Find Player HP address
[ ] Find Player HP pointer chain
[ ] Find Player MP
[ ] Find Player Position (X, Y, Z)
[ ] Find Player Level, XP, Gold
[ ] Document Player structure
[ ] Create pattern for Player base

Week 5:
[ ] Find Entity list
[ ] Find Entity count
[ ] Analyze Entity structure
[ ] Find Monster HP
[ ] Find NPC detection
[ ] Find Attack function signature
[ ] Find MoveTo function signature
[ ] Find UseSkill function signature
[ ] Document all findings
[ ] Update Offsets.h
[ ] Update Patterns.h
[ ] Test patterns after game restart
```

---

## 🎓 Tips & Best Practices

### General Tips:

1. **Save Often**
   - Save Cheat Engine tables (.CT files)
   - Document as you go
   - Take screenshots

2. **Verify Everything**
   - Test address by changing value
   - Restart game and retest pointers
   - Verify patterns work

3. **Use Comments**
   ```cpp
   // Found on 2025-10-15
   // Method: Scanned HP value
   // Verified: Working after restart
   constexpr int HP = 0x10;
   ```

4. **Test Stability**
   - Restart game multiple times
   - Test on different characters
   - Test on different maps

### Common Pitfalls:

1. **Wrong Value Type**
   - HP might be 2 bytes, not 4 bytes
   - Position is usually Float
   - Try different types if not found

2. **Dynamic Allocation**
   - Some structures are dynamically allocated
   - Pointers might change
   - Need multiple pointer chains

3. **Packed Structures**
   - Offsets might not be aligned
   - Use "dissect data structures" carefully
   - Check actual memory layout

---

## 🔍 Advanced Techniques

### 1. Finding Hidden Values

**Health might be encrypted:**
```
Displayed HP: 1500
Real HP in memory: 0x5DC (same value)
OR
Real HP in memory: 0x12345 (encrypted)

Try:
- Scan for displayed value
- Scan for displayed value * 10
- Scan for displayed value + offset
```

### 2. Code Injection for Testing

```
1. Find instruction that uses value
2. Right-click → "Replace with code that does nothing"
3. Test if feature stops working
4. If yes, you found the right place!
```

### 3. Comparing Memory Regions

```
1. Save memory snapshot
2. Perform action (e.g., level up)
3. Compare memory regions
4. Find what changed
```

---

## 📚 Learning Resources

### Cheat Engine Tutorials:

1. **Official Tutorial**: Help → Cheat Engine Tutorial
2. **Video Tutorials**: YouTube "Cheat Engine Tutorial"
3. **Forums**: https://fearlessrevolution.com/

### Reverse Engineering:

1. **Guided Hacking**: https://guidedhacking.com/
2. **UnknownCheats**: https://www.unknowncheats.me/
3. **OldSchool Hacking**: Various forums

### Assembly Reference:

1. **x86 Assembly**: https://www.felixcloutier.com/x86/
2. **Compiler Explorer**: https://godbolt.org/

---

## 🎯 Deliverables

At end of Phase 2, you should have:

1. **Complete Offsets.h**
   - All Player offsets
   - All Entity offsets
   - All function addresses

2. **Complete Patterns.h**
   - Pattern signatures for all offsets
   - Tested and verified

3. **MEMORY_MAP.md**
   - Complete documentation
   - Structures documented
   - Screenshots included

4. **Cheat Engine Table**
   - Saved .CT file with all findings
   - For future reference

---

## ✅ Success Criteria

Phase 2 is complete when:

- [ ] Can read Player HP in real-time
- [ ] Can read Player position
- [ ] Can list all entities nearby
- [ ] Patterns work after game restart
- [ ] All offsets documented
- [ ] Code compiles with new offsets
- [ ] Ready for Phase 3 (Game State Reading)

---

**Next Phase:** Phase 3 - Implement Player/Entity classes using found offsets

**Estimated Time:** 2 weeks (10-20 hours)

**Last Updated:** 2025-10-15
