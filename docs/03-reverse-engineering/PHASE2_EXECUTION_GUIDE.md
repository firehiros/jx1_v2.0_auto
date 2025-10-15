# Phase 2 Execution Guide - Step-by-Step

**Purpose**: This is your hands-on execution guide. Follow these steps in order. Check boxes as you complete each step.

---

## 🎬 Before You Start

### Prerequisites Checklist
- [ ] I have access to a Windows 10/11 machine
- [ ] JX1 game is installed and I can login successfully
- [ ] Cheat Engine 7.5+ is installed
- [ ] I have read `PHASE2_REVERSE_ENGINEERING.md` at least once
- [ ] I have `PHASE2_QUICK_REFERENCE.md` open for reference
- [ ] I have at least 4 hours of uninterrupted time
- [ ] I am ready to take detailed notes

### Time Estimate
- **Total**: 8-12 hours (can be split across multiple sessions)
- **Session 1**: Player structure (2-3 hours)
- **Session 2**: Entity manager (2-3 hours)
- **Session 3**: Functions and patterns (2-3 hours)
- **Session 4**: Verification and documentation (1-2 hours)

---

## 🚀 Session 1: Player Structure Discovery

### Setup (15 minutes)
- [ ] 1. Boot Windows machine
- [ ] 2. Launch JX1 game
- [ ] 3. Login to your character
- [ ] 4. Enter game world and idle in a safe area
- [ ] 5. Note your character's current stats:
  ```
  Current HP: _______
  Max HP: _______
  Current MP: _______
  Max MP: _______
  Level: _______
  Gold: _______
  Character Name: _______
  ```
- [ ] 6. Launch Cheat Engine
- [ ] 7. Click "Open Process" button (computer icon)
- [ ] 8. Find and select "JX1.exe" from the list
- [ ] 9. Click "Open"
- [ ] 10. Load template: File → Load → Select `tools/JX1_Template.CT`
- [ ] 11. Verify "Attached: JX1.exe" appears in CE title bar

**✅ Checkpoint**: Cheat Engine is attached to JX1.exe process

---

### Finding Player HP (30 minutes)

#### Step 1: First Scan
- [ ] 1. In Cheat Engine, click "New Scan" button (if needed)
- [ ] 2. Set "Value Type" to "4 Bytes"
- [ ] 3. Set "Scan Type" to "Exact Value"
- [ ] 4. Enter your current HP in the "Value" field
- [ ] 5. Click "First Scan" button
- [ ] 6. Wait for scan to complete (usually 1-2 seconds)
- [ ] 7. Note results count: _______ addresses found

#### Step 2: Next Scan (Change Value)
- [ ] 8. In game, take damage (let a monster hit you, or jump off height)
- [ ] 9. Note new HP value: _______
- [ ] 10. In CE, change "Value" to new HP
- [ ] 11. Click "Next Scan" button
- [ ] 12. Note results count: _______ addresses found

#### Step 3: Repeat Until Narrow
- [ ] 13. Repeat step 2 until results < 10 addresses
- [ ] 14. If stuck with many results, try:
  - [ ] Take more damage
  - [ ] Heal with potion/skill
  - [ ] Wait for natural HP regen

#### Step 4: Identify Correct Address
- [ ] 15. Double-click each address in results to add to bottom panel
- [ ] 16. In bottom panel, check the "Active" checkbox for first address
- [ ] 17. Change "Value" to a test value (e.g., 9999)
- [ ] 18. Check game - did HP change?
  - [ ] YES → You found it! Proceed to step 19
  - [ ] NO → Try next address, repeat step 17
- [ ] 19. Once found, change description to "Player HP"
- [ ] 20. Change value back to original HP
- [ ] 21. Uncheck "Active" box
- [ ] 22. Right-click address → "Change record" → "Type" → "4 Bytes"
- [ ] 23. Note address: _______________________

**✅ Checkpoint**: Player HP address found and labeled

---

### Finding Player Max HP (15 minutes)
- [ ] 1. Look at addresses near Player HP (±0x04, ±0x08 offsets)
- [ ] 2. Add nearby addresses to bottom panel
- [ ] 3. Look for value matching your max HP
- [ ] 4. If not found, new scan:
  - [ ] Value Type: 4 Bytes
  - [ ] Scan Type: Exact Value
  - [ ] Value: [Your Max HP]
- [ ] 5. Cross-reference with HP address (should be nearby)
- [ ] 6. Label as "Player Max HP"
- [ ] 7. Note address: _______________________

**✅ Checkpoint**: Player Max HP found

---

### Finding Player MP and Max MP (20 minutes)
Repeat the same process as HP:
- [ ] 1. Find Current MP (use skills to consume MP)
- [ ] 2. Find Max MP (should be near Current MP)
- [ ] 3. Label both addresses
- [ ] 4. Note addresses:
  ```
  Current MP: _______________________
  Max MP: _______________________
  ```

**✅ Checkpoint**: Player MP and Max MP found

---

### Finding Player Position (45 minutes)

#### Position X
- [ ] 1. New Scan: Value Type = "Float"
- [ ] 2. Scan Type: "Unknown initial value"
- [ ] 3. Click "First Scan"
- [ ] 4. Move character EAST (right) slowly
- [ ] 5. Click "Next Scan" → "Increased value"
- [ ] 6. Move character WEST (left) slowly
- [ ] 7. Click "Next Scan" → "Decreased value"
- [ ] 8. Repeat until < 50 results
- [ ] 9. Add all to list, watch which changes as you move
- [ ] 10. Identify X coordinate
- [ ] 11. Label as "Player Position X"
- [ ] 12. Note address: _______________________

#### Position Y
- [ ] 13. Similar to X, but move NORTH (up) = Increased
- [ ] 14. Move SOUTH (down) = Decreased
- [ ] 15. Label as "Player Position Y"
- [ ] 16. Note address: _______________________

#### Position Z (Height)
- [ ] 17. New Scan: Float, Unknown initial value
- [ ] 18. Jump or climb stairs
- [ ] 19. Click "Next Scan" → "Increased value"
- [ ] 20. Go downstairs or fall
- [ ] 21. Click "Next Scan" → "Decreased value"
- [ ] 22. Label as "Player Position Z"
- [ ] 23. Note address: _______________________

**✅ Checkpoint**: Player Position X, Y, Z found

---

### Finding Player Level, XP, Gold (30 minutes)

#### Level
- [ ] 1. Value Type: 4 Bytes
- [ ] 2. Scan Type: Exact Value
- [ ] 3. Value: [Your current level]
- [ ] 4. First Scan
- [ ] 5. Cross-reference with HP address (should be in same structure)
- [ ] 6. Label as "Player Level"
- [ ] 7. Note address: _______________________

#### Experience (if possible to change)
- [ ] 8. Value Type: Try "4 Bytes" first, then "8 Bytes" if needed
- [ ] 9. Scan for exact current XP
- [ ] 10. Kill a monster to gain XP
- [ ] 11. Scan for increased value
- [ ] 12. Label as "Player Experience"
- [ ] 13. Note address: _______________________

#### Gold
- [ ] 14. Value Type: Try "4 Bytes" first, then "8 Bytes" if needed
- [ ] 15. Scan for exact current gold
- [ ] 16. Buy/sell something to change gold
- [ ] 17. Scan for new value
- [ ] 18. Label as "Player Gold"
- [ ] 19. Note address: _______________________

**✅ Checkpoint**: Level, XP, Gold found

---

### Finding Character Name (15 minutes)
- [ ] 1. Value Type: "String"
- [ ] 2. String: [Your character name]
- [ ] 3. First Scan
- [ ] 4. Should find 1-3 results
- [ ] 5. Add to list
- [ ] 6. Check which is in the Player structure (near HP/MP)
- [ ] 7. Label as "Player Name"
- [ ] 8. Note address: _______________________

**✅ Checkpoint**: Character name found

---

### Finding Player Base Pointer (60 minutes) - CRITICAL

#### Why This Matters
Player HP, MP, etc. are at addresses that CHANGE every game restart.
We need to find a POINTER that always points to the player structure.

#### Steps
- [ ] 1. Right-click "Player HP" address
- [ ] 2. Select "Pointer scan for this address"
- [ ] 3. Click "OK" (use default settings for first try)
- [ ] 4. Wait for scan to complete (can take 1-5 minutes)
- [ ] 5. Note results count: _______ pointers found
- [ ] 6. Save pointer scan: File → Save → "player_hp_pointers.scandata"

#### Narrowing Down Pointers
- [ ] 7. In CE, close game (keep CE open)
- [ ] 8. Restart JX1 and login
- [ ] 9. Find Player HP again with normal scan (quick process now)
- [ ] 10. Note new HP address: _______________________
- [ ] 11. In pointer scan results window:
- [ ] 12. Click "Pointer scanner" → "Rescan memory"
- [ ] 13. Address: [New HP address]
- [ ] 14. Click "OK"
- [ ] 15. This filters to pointers that STILL work
- [ ] 16. Note remaining pointers: _______

#### Selecting Best Pointer
- [ ] 17. Look for pointers with:
  - [ ] Fewest offsets (Level 1-3 is best)
  - [ ] Static base address (green, not red)
  - [ ] Base module "JX1.exe" (not dynamic heap)
- [ ] 18. Double-click best pointer to add to main list
- [ ] 19. Test it:
  - [ ] Close game and CE
  - [ ] Restart game and CE
  - [ ] Load your CE table
  - [ ] Does pointer still show correct HP?
- [ ] 20. If YES → You have a stable pointer! 🎉
- [ ] 21. Document pointer chain:
  ```
  Base: JX1.exe + 0x____________
  Offsets: [0x____, 0x____, ...]
  Final offset to HP: 0x____
  ```

**✅ Checkpoint**: Player base pointer found and verified

---

### Calculating Offsets (30 minutes)

Now that you have the base pointer, calculate offsets for all player values:

- [ ] 1. Note base address: _______________________
- [ ] 2. For each player value, calculate offset:
  ```
  Offset = [Value Address] - [Base Address]
  ```
- [ ] 3. Document offsets:
  ```
  Player Base: [Base Address]
    +0x____: HP
    +0x____: Max HP
    +0x____: MP
    +0x____: Max MP
    +0x____: Position X
    +0x____: Position Y
    +0x____: Position Z
    +0x____: Level
    +0x____: Experience
    +0x____: Gold
    +0x____: Name
  ```

**✅ Checkpoint**: All player offsets calculated

---

### Save Session 1
- [ ] 1. File → Save As → "JX1_Session1_Player.CT"
- [ ] 2. Copy offsets to `docs/MEMORY_MAP.md`
- [ ] 3. Update `core_dll/src/utils/Offsets.h` with found values
- [ ] 4. Take a break! ☕

**✅ Session 1 Complete**: Player structure fully mapped

---

## 🚀 Session 2: Entity Manager Discovery

### Setup (10 minutes)
- [ ] 1. Launch JX1 and login
- [ ] 2. Launch Cheat Engine and attach to JX1.exe
- [ ] 3. Load "JX1_Session1_Player.CT"
- [ ] 4. Go to an area with NPCs/monsters
- [ ] 5. Count visible entities: _______ entities

---

### Finding Entity Count (30 minutes)

#### Method: Unknown Initial Value
- [ ] 1. Value Type: "4 Bytes"
- [ ] 2. Scan Type: "Unknown initial value"
- [ ] 3. Click "First Scan"
- [ ] 4. Note results: _______ addresses
- [ ] 5. Kill a monster or let one despawn
- [ ] 6. Click "Next Scan" → "Decreased value"
- [ ] 7. Wait for monster to respawn or move to see new one
- [ ] 8. Click "Next Scan" → "Increased value"
- [ ] 9. Repeat until < 20 results
- [ ] 10. Add all to list
- [ ] 11. Watch values as entities spawn/despawn
- [ ] 12. Identify entity count
- [ ] 13. Label as "Entity Count"
- [ ] 14. Note address: _______________________

**✅ Checkpoint**: Entity count found

---

### Finding Entity Array (45 minutes)

#### Method: Pointer Search Near Entity Count
- [ ] 1. Right-click "Entity Count" address
- [ ] 2. Select "Browse this memory region"
- [ ] 3. Look at nearby addresses (±0x10 bytes)
- [ ] 4. Look for addresses that look like pointers:
  - Large hex values (0x00400000 - 0x7FFFFFFF)
  - Values that don't change much
- [ ] 5. Add suspicious addresses to list as "4 Bytes Hex"
- [ ] 6. For each suspicious address:
  - [ ] Right-click → "Browse this memory region"
  - [ ] Does it point to a structure with entity data?
- [ ] 7. Once found, label as "Entity Array Pointer"
- [ ] 8. Note address: _______________________

**Alternative Method: Search for Known Entity**
- [ ] 9. If above fails, search for entity name (string scan)
- [ ] 10. Find entity name in memory
- [ ] 11. Look at structure around it
- [ ] 12. Find what points to this structure
- [ ] 13. Trace back to entity array

**✅ Checkpoint**: Entity array pointer found

---

### Mapping Entity Structure (60 minutes)

#### Get First Entity
- [ ] 1. Take "Entity Array Pointer" value
- [ ] 2. Add address manually: [Array Pointer Value]
- [ ] 3. This is Entity[0]
- [ ] 4. Examine memory around this address

#### Find Entity Fields
- [ ] 5. Look for entity name (should be visible as string)
- [ ] 6. Note name offset: +0x____
- [ ] 7. Look for entity HP (4 bytes near name)
- [ ] 8. Note HP offset: +0x____
- [ ] 9. Look for entity position (3 floats near HP)
- [ ] 10. Note X offset: +0x____
- [ ] 11. Note Y offset: +0x____
- [ ] 12. Note Z offset: +0x____
- [ ] 13. Look for entity type (1=Player, 2=Monster, etc.)
- [ ] 14. Note type offset: +0x____
- [ ] 15. Look for entity ID (unique number)
- [ ] 16. Note ID offset: +0x____

#### Verify Entity Structure Size
- [ ] 17. Get Entity[1] address = [Array] + [Size]
- [ ] 18. Try size = 0x100, 0x200, 0x400
- [ ] 19. Check if Entity[1] has similar structure
- [ ] 20. Confirm entity size: 0x____

**✅ Checkpoint**: Entity structure mapped

---

### Document Entity Manager (30 minutes)
- [ ] 1. Open `docs/MEMORY_MAP.md`
- [ ] 2. Fill in Entity Manager section:
  ```
  Entity Manager Base: [Address]
    +0x____: Entity Array Pointer
    +0x____: Entity Count

  Entity Structure (Size: 0x____):
    +0x____: Entity ID
    +0x____: Entity Type
    +0x____: HP
    +0x____: Max HP (if found)
    +0x____: Position X
    +0x____: Position Y
    +0x____: Position Z
    +0x____: Name (+0x____)
    +0x____: Is Dead (if found)
  ```

**✅ Checkpoint**: Entity manager documented

---

### Save Session 2
- [ ] 1. File → Save As → "JX1_Session2_Entities.CT"
- [ ] 2. Update `docs/MEMORY_MAP.md`
- [ ] 3. Update `core_dll/src/utils/Offsets.h`
- [ ] 4. Take a break! ☕

**✅ Session 2 Complete**: Entity manager fully mapped

---

## 🚀 Session 3: Function Discovery & Patterns

### Setup (10 minutes)
- [ ] 1. Launch JX1 and login
- [ ] 2. Launch Cheat Engine and attach
- [ ] 3. Load "JX1_Session2_Entities.CT"
- [ ] 4. Verify all addresses still work
- [ ] 5. Have notepad ready for assembly code

---

### Finding Attack Function (45 minutes)

#### Method: Find What Writes to Enemy HP
- [ ] 1. Find an enemy/monster
- [ ] 2. Scan for enemy HP (same process as player HP)
- [ ] 3. Once found, right-click enemy HP
- [ ] 4. Select "Find out what writes to this address"
- [ ] 5. Click "OK"
- [ ] 6. A new window opens (empty)
- [ ] 7. In game, attack the enemy
- [ ] 8. CE window should show instructions
- [ ] 9. Note instructions that appear: _______
- [ ] 10. Double-click an instruction to view assembly
- [ ] 11. This opens "Memory Viewer" at that address

#### Identify Function Start
- [ ] 12. Scroll up in Memory Viewer
- [ ] 13. Look for function prologue:
  ```
  push ebp
  mov ebp,esp
  sub esp,[number]
  ```
- [ ] 14. Note function start address: _______________________
- [ ] 15. Copy first 10-15 bytes of assembly:
  ```
  Address   | Hex Bytes           | Assembly
  ----------|---------------------|----------------
  ________ | __________________ | _______________
  ________ | __________________ | _______________
  ________ | __________________ | _______________
  ```

#### Document Function
- [ ] 16. Open `docs/MEMORY_MAP.md`
- [ ] 17. Add to Functions section:
  ```
  Attack Function
  Address: 0x________
  Assembly:
    [Paste assembly code]

  Calling Convention: __thiscall (ECX = this pointer)
  Parameters:
    ECX: Player* (this)
    Stack+0: Entity* target (if applicable)
  ```

**✅ Checkpoint**: Attack function found and documented

---

### Finding MoveTo Function (45 minutes)

#### Method: Find What Writes to Player Position
- [ ] 1. Right-click "Player Position X"
- [ ] 2. Select "Find out what writes to this address"
- [ ] 3. In game, move character (click to move, not WASD keys)
- [ ] 4. Note instructions that write to position
- [ ] 5. Look for instructions that write all 3 coordinates (X, Y, Z)
- [ ] 6. Find function prologue (scroll up)
- [ ] 7. Note function address: _______________________
- [ ] 8. Copy assembly code (first 10-15 bytes)
- [ ] 9. Document in `MEMORY_MAP.md`

**✅ Checkpoint**: MoveTo function found

---

### Finding UseSkill Function (45 minutes)

#### Method: Find What Writes When Using Skill
- [ ] 1. Right-click "Player MP" (MP decreases when using skill)
- [ ] 2. Select "Find out what writes to this address"
- [ ] 3. In game, use a skill
- [ ] 4. Note instructions
- [ ] 5. Or: Look for skill cooldown timer in memory
- [ ] 6. Find what writes to cooldown
- [ ] 7. Trace back to UseSkill function
- [ ] 8. Note function address: _______________________
- [ ] 9. Document in `MEMORY_MAP.md`

**✅ Checkpoint**: UseSkill function found

---

### Finding Additional Functions (Optional, 30 min each)

#### UseItem Function
- [ ] Use similar method: Find what writes when using potion
- [ ] Document address: _______________________

#### PickupItem Function
- [ ] Find what writes to inventory count/items
- [ ] Document address: _______________________

#### SendChat Function
- [ ] May require string search or packet analysis
- [ ] Document address: _______________________

**✅ Checkpoint**: Additional functions found (optional)

---

### Creating Patterns (90 minutes)

For EACH function found, create a pattern:

#### Attack Function Pattern
- [ ] 1. Open Memory Viewer at Attack function address
- [ ] 2. Copy first 10-15 bytes (hex display)
- [ ] 3. Example bytes: `55 8B EC 83 EC 20 56 8B F1 85 F6`
- [ ] 4. Identify which bytes might change:
  - [ ] Absolute addresses (4-byte pointers) → Wildcard
  - [ ] Immediate values that might vary → Wildcard
  - [ ] Stack sizes (sub esp,XX) → Might wildcard
- [ ] 5. Create pattern string:
  ```cpp
  constexpr const char* ATTACK_FUNCTION =
      "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1";
  constexpr const char* ATTACK_FUNCTION_MASK =
      "xxxxx?xxx";
  ```
  - [ ] `\x00` = wildcard in pattern
  - [ ] `?` = wildcard in mask
  - [ ] `x` = exact match in mask
- [ ] 6. Update `core_dll/src/utils/Patterns.h`

#### Repeat for Each Function
- [ ] 7. Create pattern for MoveTo function
- [ ] 8. Create pattern for UseSkill function
- [ ] 9. Create pattern for UseItem function (if found)
- [ ] 10. Create pattern for PickupItem function (if found)
- [ ] 11. Create pattern for SendChat function (if found)

#### Create Pattern for Player Base
- [ ] 12. View memory at Player Base Pointer
- [ ] 13. Find instruction that references it:
  ```
  Example: mov ecx,[JX1.exe+XXXXXX]
  Bytes: 8B 0D XX XX XX XX
  ```
- [ ] 14. Create pattern for this instruction
- [ ] 15. Update `Patterns.h`:
  ```cpp
  constexpr const char* PLAYER_BASE = "\x8B\x0D\x00\x00\x00\x00";
  constexpr const char* PLAYER_BASE_MASK = "xx????";
  ```

#### Create Pattern for Entity Manager
- [ ] 16. Same process as Player Base
- [ ] 17. Update `Patterns.h`

**✅ Checkpoint**: All patterns created

---

### Save Session 3
- [ ] 1. File → Save As → "JX1_Session3_Functions.CT"
- [ ] 2. Update `docs/MEMORY_MAP.md` with all functions
- [ ] 3. Update `core_dll/src/utils/Patterns.h` with all patterns
- [ ] 4. Take a break! ☕

**✅ Session 3 Complete**: Functions and patterns documented

---

## 🚀 Session 4: Verification & Finalization

### Setup (5 minutes)
- [ ] 1. Close JX1 game
- [ ] 2. Close Cheat Engine
- [ ] 3. Restart computer (clean restart)
- [ ] 4. Launch JX1 and login

---

### Verify Static Addresses (15 minutes)
- [ ] 1. Launch Cheat Engine
- [ ] 2. Attach to JX1.exe
- [ ] 3. Load "JX1_Session3_Functions.CT"
- [ ] 4. Check each value:
  - [ ] Player HP shows correct value
  - [ ] Player MP shows correct value
  - [ ] Position X, Y, Z show correct values
  - [ ] Level, Gold show correct values
  - [ ] Entity Count shows correct value
- [ ] 5. If any fail:
  - [ ] Mark as "UNSTABLE" in MEMORY_MAP.md
  - [ ] Need to use pattern scan for this value

**✅ Checkpoint**: Static addresses verified

---

### Test Patterns (30 minutes)

For this, you'll need to implement basic pattern scanning in C++:

#### Create Test Program
- [ ] 1. On Windows machine, create simple C++ test file:
  ```cpp
  // test_patterns.cpp
  #include <Windows.h>
  #include <iostream>

  uintptr_t FindPattern(HMODULE module, const char* pattern, const char* mask) {
      MODULEINFO modInfo;
      GetModuleInformation(GetCurrentProcess(), module, &modInfo, sizeof(MODULEINFO));

      uintptr_t base = (uintptr_t)module;
      uintptr_t size = modInfo.SizeOfImage;

      size_t patternLength = strlen(mask);

      for (uintptr_t i = 0; i < size - patternLength; i++) {
          bool found = true;
          for (size_t j = 0; j < patternLength; j++) {
              if (mask[j] != '?' && pattern[j] != *(char*)(base + i + j)) {
                  found = false;
                  break;
              }
          }
          if (found) {
              return base + i;
          }
      }
      return 0;
  }

  int main() {
      HMODULE hModule = GetModuleHandleA("JX1.exe");
      if (!hModule) {
          std::cout << "JX1.exe not found!" << std::endl;
          return 1;
      }

      // Test Attack Function pattern
      const char* pattern = "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1";
      const char* mask = "xxxxx?xxx";

      uintptr_t address = FindPattern(hModule, pattern, mask);

      if (address) {
          std::cout << "Attack Function found at: 0x" << std::hex << address << std::endl;
      } else {
          std::cout << "Pattern not found!" << std::endl;
      }

      return 0;
  }
  ```

- [ ] 2. Compile: `cl test_patterns.cpp /EHsc Psapi.lib`
- [ ] 3. Inject into JX1 process or attach debugger
- [ ] 4. Run and verify patterns work

**Note**: This can also be tested in Phase 3 when implementing MemoryManager

**✅ Checkpoint**: Patterns tested (or marked for Phase 3 testing)

---

### Complete Documentation (60 minutes)

#### Finalize MEMORY_MAP.md
- [ ] 1. Open `docs/MEMORY_MAP.md`
- [ ] 2. Fill in all TODO sections
- [ ] 3. Add summary section at top:
  ```markdown
  ## Summary

  **Status**: ✅ Complete
  **Date Completed**: 2025-10-15
  **Game Version**: [Version]

  ### Key Findings:
  - Player Base: JX1.exe + 0x______
  - Entity Manager: JX1.exe + 0x______
  - Functions Found: 6/6 (Attack, MoveTo, UseSkill, UseItem, Pickup, Chat)
  - Patterns Created: 8/8
  - All patterns verified: ✅
  ```
- [ ] 4. Add verification status for each item
- [ ] 5. Add notes about any quirks or special cases
- [ ] 6. Add screenshots (optional but helpful):
  - [ ] Screenshot of CE with Player structure
  - [ ] Screenshot of Entity structure
  - [ ] Screenshot of function assembly

#### Update Offsets.h
- [ ] 7. Open `core_dll/src/utils/Offsets.h`
- [ ] 8. Replace all `0x0` with actual offsets
- [ ] 9. Add comments with verification status:
  ```cpp
  static constexpr int HP = 0x10;  // ✅ Verified 2025-10-15
  ```

#### Update Patterns.h
- [ ] 10. Open `core_dll/src/utils/Patterns.h`
- [ ] 11. Replace all empty patterns with actual patterns
- [ ] 12. Add comments with assembly code:
  ```cpp
  // Attack Function Pattern
  // Assembly: push ebp, mov ebp,esp, sub esp,20, push esi, mov esi,ecx
  // Found at: 0x00401000 (base game, will vary)
  constexpr const char* ATTACK_FUNCTION = "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1";
  constexpr const char* ATTACK_FUNCTION_MASK = "xxxxx?xxx";
  ```

#### Update PHASE2_STATUS.md
- [ ] 13. Open `docs/PHASE2_STATUS.md`
- [ ] 14. Update status to "✅ Complete"
- [ ] 15. Check all completion boxes
- [ ] 16. Add completion date
- [ ] 17. Add summary of findings

**✅ Checkpoint**: All documentation complete

---

### Create Reference Materials (30 minutes)

#### Final Cheat Engine Table
- [ ] 1. Clean up CE table (remove test addresses)
- [ ] 2. Organize into groups (Player, Entities, Functions)
- [ ] 3. Add descriptions to all addresses
- [ ] 4. Save as "JX1_V2.0_Final.CT"
- [ ] 5. Copy to `tools/` directory

#### Create Offset Summary
- [ ] 6. Create quick reference file:
  ```
  File: docs/OFFSET_QUICK_REFERENCE.txt

  === PLAYER STRUCTURE ===
  Base: JX1.exe + 0x________

  +0x____: HP (int)
  +0x____: Max HP (int)
  +0x____: MP (int)
  +0x____: Max MP (int)
  +0x____: Position X (float)
  +0x____: Position Y (float)
  +0x____: Position Z (float)
  +0x____: Level (int)
  +0x____: Experience (int/int64)
  +0x____: Gold (int/int64)
  +0x____: Name (string)

  === ENTITY MANAGER ===
  Base: JX1.exe + 0x________

  +0x____: Entity Count (int)
  +0x____: Entity Array (pointer)

  === ENTITY STRUCTURE ===
  Size: 0x____ bytes

  +0x____: Entity ID (uint32)
  +0x____: Entity Type (uint32)
  +0x____: HP (int)
  +0x____: Position X (float)
  +0x____: Position Y (float)
  +0x____: Position Z (float)
  +0x____: Name (string)

  === FUNCTIONS ===
  Attack: Pattern in Patterns.h
  MoveTo: Pattern in Patterns.h
  UseSkill: Pattern in Patterns.h
  ```

**✅ Checkpoint**: Reference materials created

---

### Final Verification (30 minutes)

#### Checklist Review
- [ ] 1. All player offsets found and documented ✅
- [ ] 2. Entity manager structure found and documented ✅
- [ ] 3. At least 3 critical functions found ✅
- [ ] 4. Patterns created for all critical addresses ✅
- [ ] 5. Patterns verified after game restart ✅
- [ ] 6. Offsets.h updated with actual values ✅
- [ ] 7. Patterns.h updated with actual patterns ✅
- [ ] 8. MEMORY_MAP.md completed ✅
- [ ] 9. Cheat Engine table saved ✅
- [ ] 10. Ready for Phase 3 implementation ✅

#### Quality Check
- [ ] 11. Can you read player HP in real-time? ✅
- [ ] 12. Can you read player position? ✅
- [ ] 13. Can you read entity list? ✅
- [ ] 14. Can you identify Attack function location? ✅
- [ ] 15. Do patterns work after game restart? ✅

**✅ Checkpoint**: Phase 2 fully verified

---

### Commit Changes (If using Git)

- [ ] 1. Add all updated files:
  ```bash
  git add core_dll/src/utils/Offsets.h
  git add core_dll/src/utils/Patterns.h
  git add docs/MEMORY_MAP.md
  git add docs/PHASE2_STATUS.md
  git add tools/JX1_V2.0_Final.CT
  git add docs/OFFSET_QUICK_REFERENCE.txt
  ```
- [ ] 2. Commit:
  ```bash
  git commit -m "Phase 2 Complete: Reverse engineering finished

  - Found all player structure offsets
  - Mapped entity manager and entity structure
  - Discovered 6 critical game functions
  - Created patterns for stability
  - All findings documented and verified
  "
  ```

**✅ Session 4 Complete**: Phase 2 finalized and verified

---

## 🎉 Phase 2 Complete!

### What You've Accomplished

You have successfully:

1. ✅ **Mapped the entire Player structure**
   - HP, MP, Position, Level, Gold, and more
   - Found stable base pointer
   - Calculated all offsets

2. ✅ **Mapped the Entity Manager**
   - Entity list location
   - Entity structure layout
   - Entity properties (HP, Position, Name)

3. ✅ **Discovered Critical Functions**
   - Attack function
   - MoveTo function
   - UseSkill function
   - And more...

4. ✅ **Created Stable Patterns**
   - Patterns for player base
   - Patterns for entity manager
   - Patterns for all functions
   - Verified to work after restart

5. ✅ **Documented Everything**
   - Complete memory map
   - Updated code files (Offsets.h, Patterns.h)
   - Created reference materials

### Time Invested
Approximately: _______ hours

### Celebration
You deserve a break! This was intensive work. 🎊

---

## 🚀 Next Steps: Phase 3

You are now ready to proceed to **Phase 3: Implementation**

In Phase 3, you will:

1. **Implement Pattern Scanning**
   - Add pattern scan code to MemoryManager
   - Scan for all patterns on DLL load
   - Store found addresses

2. **Implement Player Class**
   - Create `Player.h` and `Player.cpp`
   - Use offsets from Offsets.h
   - Read player HP, MP, Position in real-time
   - Test with logging

3. **Implement Entity Class**
   - Create `Entity.h` and `Entity.cpp`
   - Read entity list
   - Iterate entities
   - Get entity properties

4. **Test Everything**
   - Inject DLL into JX1
   - Verify real-time data reading
   - Log player stats every second
   - Log nearby entities

### Estimated Time for Phase 3
- 2-3 weeks (10-15 hours)

---

## 📚 Resources Created

You now have:

- ✅ Complete memory map documentation
- ✅ Working Cheat Engine table
- ✅ All offsets in code
- ✅ All patterns in code
- ✅ Quick reference guide
- ✅ Verification checklist

Everything you need to build the automation system!

---

**Phase 2 Execution Guide Complete**
**Status**: 🎉 Ready to Execute or ✅ Completed
**Last Updated**: 2025-10-15

Good luck with your reverse engineering! 🚀
