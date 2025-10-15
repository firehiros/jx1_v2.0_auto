# Phase 2 Quick Reference Card - Cheat Engine

## 🎯 Primary Objectives

### 1. Player Structure Offsets
- [ ] HP (Current Health) - Type: `int` or `float`
- [ ] Max HP - Type: `int` or `float`
- [ ] MP (Current Mana) - Type: `int` or `float`
- [ ] Max MP - Type: `int` or `float`
- [ ] Position X - Type: `float`
- [ ] Position Y - Type: `float`
- [ ] Position Z - Type: `float`
- [ ] Direction - Type: `float` (0-360)
- [ ] Level - Type: `int`
- [ ] Experience - Type: `int` or `int64`
- [ ] Gold - Type: `int` or `int64`
- [ ] Character Name - Type: `string` (max 32 chars)

### 2. Entity Manager Structure
- [ ] Entity List Pointer - Type: `pointer`
- [ ] Entity Count - Type: `int`
- [ ] Entity Array - Type: `array of pointers`

### 3. Critical Function Addresses
- [ ] Attack Function - Look for: `push ebp, mov ebp,esp, sub esp,??`
- [ ] MoveTo Function
- [ ] UseSkill Function
- [ ] PickupItem Function
- [ ] UseItem Function
- [ ] SendChat Function

---

## 🔍 Cheat Engine Quick Commands

### Initial Scan (First Search)
```
1. Value Type: 4 Bytes (int) / Float / Double
2. Scan Type: Exact Value / Unknown Initial Value
3. Value: [Enter known value]
4. Click: [First Scan]
```

### Next Scan (After Value Changes)
```
1. New Value: [Enter new value]
2. Click: [Next Scan]
```

### Pointer Scan (Finding Base Pointer)
```
1. Right-click address → Pointer scan for this address
2. Wait for results
3. Change value in game
4. Pointer scanner → Rescan memory
5. Keep addresses that remain valid
```

### View Assembly at Address
```
1. Right-click address → Disassemble this memory region
2. Or: Click "Memory View" button → Go to address
```

### Find What Writes/Reads
```
1. Right-click address → Find out what writes to this address
2. Perform action in game
3. View instructions that modified the address
4. Note the instruction and surrounding bytes
```

---

## 📊 Memory Scanning Cheat Sheet

| Data Type | CE Type | Size | Example |
|-----------|---------|------|---------|
| HP/MP | 4 Bytes | 4 | 850 |
| Position | Float | 4 | 1024.5 |
| Gold/XP | 8 Bytes | 8 | 999999 |
| Name | String | var | "Player01" |
| Pointer | 4 Bytes Hex | 4 | 0x12345678 |

### Scan Strategies

**Known Value (HP, Level, Gold):**
```
1. First Scan: Exact Value → [Current Value]
2. Change value in game
3. Next Scan: Exact Value → [New Value]
4. Repeat until 1-10 addresses remain
```

**Unknown Initial Value (Entity Count):**
```
1. First Scan: Unknown Initial Value
2. Change value (kill/spawn entity)
3. Next Scan: Increased Value / Decreased Value
4. Repeat until narrow down
```

**Float Values (Position X/Y/Z):**
```
1. Value Type: Float
2. Move character slowly
3. Scan Type: Increased Value / Decreased Value
4. Move in opposite direction
5. Scan opposite condition
```

**String Search (Character Name):**
```
1. Value Type: String
2. String: [Your character name]
3. First Scan
4. Should find 1-3 addresses
```

---

## 🎯 Pattern Creation Guide

### Step 1: Find Function Address
```
Method A: Find what writes → View code
Method B: Memory View → Search for string/value
Method C: Follow pointer chain
```

### Step 2: View Assembly
```
Example Attack Function:
Address   | Bytes            | Assembly
----------|------------------|------------------
00401000  | 55               | push ebp
00401001  | 8B EC            | mov ebp,esp
00401003  | 83 EC 20         | sub esp,20
00401006  | 56               | push esi
00401007  | 8B F1            | mov esi,ecx
00401009  | 85 F6            | test esi,esi
0040100B  | 74 0A            | je short 00401017
```

### Step 3: Create Pattern (First 10-15 bytes)
```cpp
// Fixed bytes: 55 8B EC 83 EC ?? 56 8B F1
// The ?? represents dynamic value (0x20 might change)

constexpr const char* ATTACK_FUNCTION = "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1";
constexpr const char* ATTACK_FUNCTION_MASK = "xxxxx?xxx";
//                                                    ^ wildcard
```

### Pattern Wildcard Rules
- Use `\x00` in pattern for wildcard bytes
- Use `?` in mask for wildcard position
- Use `x` in mask for exact match position

---

## 🔧 Common Issues & Solutions

### Issue: Too Many Results (1000+)
**Solution:**
- Use more specific value type
- Wait for value to change more
- Scan additional related values (HP + MP together)

### Issue: No Results Found
**Solution:**
- Wrong value type (try Float if 4 Bytes fails)
- Value is calculated/displayed differently (try ±10% range)
- Try "Unknown Initial Value" scan

### Issue: Address Changes Every Restart
**Solution:**
- Need to find pointer chain
- Use Pointer Scan feature
- Find static base address

### Issue: Can't Find Function
**Solution:**
- Find what writes to a value (HP decrease = Attack function)
- Search for text strings ("You attacked", "Miss!")
- Look for DirectX/WinAPI imports

---

## 📝 Documentation Template

### When You Find an Offset
```cpp
// In Offsets.h
static constexpr int HP = 0x10;  // Found: 2025-10-15, CE scan, verified working
```

### When You Find a Pattern
```cpp
// In Patterns.h
constexpr const char* ATTACK_FUNCTION = "\x55\x8B\xEC\x83\xEC\x00\x56";
constexpr const char* ATTACK_FUNCTION_MASK = "xxxxx?xx";
// Found at: 0x00401000 (base game)
// Assembly: push ebp, mov ebp,esp, sub esp,20, push esi
```

### Create Memory Map Notes
```
Player Base: 0x12345678 (static)
  +0x00: VTable
  +0x04: Entity ID
  +0x10: HP (int)
  +0x14: Max HP (int)
  +0x18: MP (int)
  +0x1C: Max MP (int)
  +0x20: Position X (float)
  +0x24: Position Y (float)
  +0x28: Position Z (float)
```

---

## ⚡ Workflow Checklist

### Session Start
- [ ] Launch JX1 game
- [ ] Login to character
- [ ] Open Cheat Engine
- [ ] Attach to JX1.exe process
- [ ] Enable "Hex" display
- [ ] Set scan region to "All" or "Local"

### Finding Each Value
- [ ] Note current value in game
- [ ] First scan in Cheat Engine
- [ ] Change value in game
- [ ] Next scan with new value
- [ ] Repeat until <10 results
- [ ] Test each address (freeze/change)
- [ ] Find pointer chain
- [ ] Calculate offset from base
- [ ] Document in Offsets.h
- [ ] Create pattern for stability
- [ ] Document in Patterns.h

### Function Discovery
- [ ] Find what writes to HP/MP/Position
- [ ] View assembly code
- [ ] Identify function start (push ebp)
- [ ] Copy first 10-15 bytes
- [ ] Create pattern with wildcards
- [ ] Test pattern after game restart
- [ ] Document assembly code
- [ ] Add to Patterns.h

### Session End
- [ ] Save Cheat Engine table (.CT file)
- [ ] Document all findings in MEMORY_MAP.md
- [ ] Update Offsets.h with values
- [ ] Update Patterns.h with signatures
- [ ] Test patterns after restart
- [ ] Commit changes to Git

---

## 🎮 JX1-Specific Tips

### Common Static Addresses
Look for these patterns in JX1:
```
Player Base: Usually at "JX1.exe"+offset (0x400000 + offset)
Entity Manager: Often near player base
Inventory: Usually Player+offset
```

### Common Function Signatures
```
Attack: Often starts with ECX register (this pointer)
MoveTo: Takes float parameters (X, Y, Z)
UseSkill: Takes skill ID + target pointer
```

### Memory Regions to Focus On
```
1. .text section (code) - For functions
2. .data section (static data) - For base pointers
3. Heap memory - For dynamic entities
```

---

## 📚 Quick Reference Links

- [Cheat Engine Tutorial](https://wiki.cheatengine.org/index.php?title=Tutorials)
- [Pattern Scanning Explanation](https://guidedhacking.com/threads/how-to-make-game-hacks-pattern-scanning.14112/)
- [Pointer Scanning Guide](https://wiki.cheatengine.org/index.php?title=Pointer_scan)

---

## ✅ Success Criteria

You've successfully completed Phase 2 when:

1. ✅ Found Player HP, MP, Position, Level with stable pointers
2. ✅ Found Entity list and can read entity data
3. ✅ Found at least 3 critical function addresses
4. ✅ Created patterns that work after game restart
5. ✅ All values documented in Offsets.h
6. ✅ All patterns documented in Patterns.h
7. ✅ Created MEMORY_MAP.md with structure documentation
8. ✅ Saved Cheat Engine table (.CT) for reference

---

**Next Phase**: Phase 3 - Implement Player/Entity classes using these offsets

**Estimated Time**: 8-12 hours of active reverse engineering work
