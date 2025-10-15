# Phase 2 Status - Reverse Engineering

**Phase**: Phase 2 - Reverse Engineering
**Start Date**: 2025-10-15
**Expected Duration**: 8-12 hours of active work
**Status**: 🎯 Ready to Execute (Preparation Complete)

---

## ✅ Preparation Complete

All documentation, templates, and tools are ready for Phase 2 execution:

### Documentation Created
- ✅ **PHASE2_REVERSE_ENGINEERING.md** - Comprehensive 400+ line guide
  - Cheat Engine tutorial
  - Step-by-step instructions for finding Player structure
  - Entity list discovery methods
  - Function signature finding techniques
  - Pattern creation tutorial

- ✅ **PHASE2_QUICK_REFERENCE.md** - Quick reference card
  - Cheat Engine commands cheatsheet
  - Memory scanning strategies
  - Pattern creation guide
  - Common issues & solutions
  - Workflow checklist

- ✅ **MEMORY_MAP.md** - Template for documenting findings
  - Player structure template
  - Entity manager template
  - Inventory structure template
  - Skill system template
  - Function addresses template
  - Verification checklists

### Code Templates Created
- ✅ **Offsets.h** - Updated with comprehensive structure
  - Player offsets (HP, MP, Position, Level, etc.)
  - Entity offsets (ID, Type, HP, Position, etc.)
  - Inventory offsets
  - Skill system offsets
  - Function address placeholders
  - All marked with TODO for Phase 2

- ✅ **Patterns.h** - Updated with pattern templates
  - Player base pointer pattern
  - Entity manager pattern
  - Function signature patterns
  - Pattern creation tutorial in comments
  - Assembly-to-pattern conversion examples

### Tools Created
- ✅ **JX1_Template.CT** - Cheat Engine table template
  - Pre-organized structure for all values
  - Player stats group
  - Position group
  - Entity manager group
  - Inventory group
  - Skills group
  - Function addresses group
  - Instructions in comments

---

## 🎯 Phase 2 Objectives

### Primary Goals
1. ✅ Documentation prepared
2. ✅ Templates created
3. ⏳ Find Player base pointer and offsets
4. ⏳ Find Entity manager and entity structure
5. ⏳ Find critical game functions
6. ⏳ Create stable patterns for all findings
7. ⏳ Verify patterns after game restart
8. ⏳ Document all findings

### Success Criteria
- [ ] Player HP, MP, Position readable with stable pointer
- [ ] Entity list accessible and can read entity data
- [ ] At least 3 critical functions found (Attack, MoveTo, UseSkill)
- [ ] All patterns work after game restart
- [ ] All values documented in Offsets.h
- [ ] All patterns documented in Patterns.h
- [ ] MEMORY_MAP.md completed with findings
- [ ] Cheat Engine table saved with working addresses

---

## 📋 Task Breakdown

### Task 1: Setup (30 minutes)
- [ ] Access Windows environment
- [ ] Install Cheat Engine 7.5+
- [ ] Launch JX1 game
- [ ] Login to character
- [ ] Load JX1_Template.CT in Cheat Engine
- [ ] Attach to JX1.exe process

### Task 2: Find Player Structure (2-3 hours)
- [ ] Find Player HP (scan for exact value)
- [ ] Find Player Max HP
- [ ] Find Player MP
- [ ] Find Player Max MP
- [ ] Find Position X (float scan while moving)
- [ ] Find Position Y (float scan while moving)
- [ ] Find Position Z (float scan while jumping)
- [ ] Find Level (4 bytes exact value)
- [ ] Find Experience (8 bytes or 4 bytes)
- [ ] Find Gold (8 bytes or 4 bytes)
- [ ] Find Character Name (string search)
- [ ] Perform pointer scan for HP to find base pointer
- [ ] Calculate all offsets from base pointer
- [ ] Document in MEMORY_MAP.md

### Task 3: Find Entity Manager (2-3 hours)
- [ ] Find entity count (unknown initial value scan)
- [ ] Spawn/kill entities to narrow down
- [ ] Find entity array pointer
- [ ] Examine first entity structure
- [ ] Find entity ID offset
- [ ] Find entity type offset
- [ ] Find entity HP offset
- [ ] Find entity position offsets
- [ ] Find entity name offset
- [ ] Document entity structure size
- [ ] Document in MEMORY_MAP.md

### Task 4: Find Functions (2-3 hours)
- [ ] Find Attack function (find what writes when attacking)
- [ ] Find MoveTo function (find what writes to position)
- [ ] Find UseSkill function (find what writes when using skill)
- [ ] Find UseItem function (find what writes to inventory)
- [ ] Find PickupItem function (find what writes when picking up)
- [ ] Find SendChat function (find what writes when chatting)
- [ ] Document assembly code for each function
- [ ] Identify function signatures
- [ ] Document in MEMORY_MAP.md

### Task 5: Create Patterns (1-2 hours)
- [ ] Create pattern for Player base pointer
- [ ] Create pattern for Entity manager
- [ ] Create pattern for Attack function
- [ ] Create pattern for MoveTo function
- [ ] Create pattern for UseSkill function
- [ ] Create pattern for UseItem function
- [ ] Create pattern for PickupItem function
- [ ] Create pattern for SendChat function
- [ ] Update Patterns.h with all patterns
- [ ] Document pattern creation process in MEMORY_MAP.md

### Task 6: Verification (1 hour)
- [ ] Save Cheat Engine table
- [ ] Close game and Cheat Engine
- [ ] Restart game
- [ ] Load Cheat Engine table
- [ ] Verify all static addresses still work
- [ ] Test pattern scanning for dynamic addresses
- [ ] Update MEMORY_MAP.md with verification results
- [ ] Mark any addresses that fail as unstable

### Task 7: Documentation (1 hour)
- [ ] Complete MEMORY_MAP.md with all findings
- [ ] Update Offsets.h with actual values
- [ ] Update Patterns.h with actual patterns
- [ ] Add comments to explain any quirks or special cases
- [ ] Create memory structure diagrams (optional)
- [ ] Take screenshots of key findings
- [ ] Update PHASE2_STATUS.md (this file) as complete

---

## 🛠️ Required Tools

### Software
- ✅ Windows OS (10/11)
- ✅ JX1 Game installed and working
- ✅ Cheat Engine 7.5+ installed
- ⏳ x64dbg (optional, for advanced debugging)
- ⏳ IDA Free/Ghidra (optional, for static analysis)

### Files
- ✅ docs/PHASE2_REVERSE_ENGINEERING.md (Main guide)
- ✅ docs/PHASE2_QUICK_REFERENCE.md (Quick reference)
- ✅ docs/MEMORY_MAP.md (Documentation template)
- ✅ tools/JX1_Template.CT (Cheat Engine template)
- ✅ core_dll/src/utils/Offsets.h (Code template)
- ✅ core_dll/src/utils/Patterns.h (Code template)

---

## 📊 Progress Tracking

### Player Structure (0/12 complete)
- [ ] HP
- [ ] Max HP
- [ ] MP
- [ ] Max MP
- [ ] Position X
- [ ] Position Y
- [ ] Position Z
- [ ] Level
- [ ] Experience
- [ ] Gold
- [ ] Name
- [ ] Base Pointer

### Entity Manager (0/8 complete)
- [ ] Entity Count
- [ ] Entity Array Pointer
- [ ] Entity ID offset
- [ ] Entity Type offset
- [ ] Entity HP offset
- [ ] Entity Position offsets
- [ ] Entity Name offset
- [ ] Entity structure size

### Functions (0/6 complete)
- [ ] Attack Function
- [ ] MoveTo Function
- [ ] UseSkill Function
- [ ] UseItem Function
- [ ] PickupItem Function
- [ ] SendChat Function

### Patterns (0/8 complete)
- [ ] Player Base Pattern
- [ ] Entity Manager Pattern
- [ ] Attack Function Pattern
- [ ] MoveTo Function Pattern
- [ ] UseSkill Function Pattern
- [ ] UseItem Function Pattern
- [ ] PickupItem Function Pattern
- [ ] SendChat Function Pattern

---

## 🚨 Known Challenges

### Challenge 1: Dynamic Addresses
**Issue**: Addresses change every game restart
**Solution**: Use pointer scanning to find stable base pointers
**Mitigation**: Create patterns for critical addresses

### Challenge 2: Multiple Player Instances
**Issue**: May find multiple instances of player data
**Solution**: Use pointer chains to find the correct instance
**Mitigation**: Verify which instance updates when you perform actions

### Challenge 3: Obfuscated Code
**Issue**: Some functions may be obfuscated or use non-standard calling conventions
**Solution**: Use "Find what accesses/writes" to trace execution
**Mitigation**: Document unusual calling conventions in MEMORY_MAP.md

### Challenge 4: Anti-Debug Protection
**Issue**: Game might detect Cheat Engine
**Solution**: Publisher partnership should allow debugging
**Mitigation**: Contact publisher if issues arise

---

## 💡 Tips & Best Practices

### General Tips
1. **Save frequently** - Save Cheat Engine table after every discovery
2. **Document everything** - Write notes as you go, not at the end
3. **Test thoroughly** - Verify each finding multiple times
4. **Use meaningful names** - Label addresses clearly in CE table
5. **Take breaks** - Reverse engineering is mentally intensive

### Cheat Engine Tips
1. **Start with known values** - HP, Level, Gold are easiest to find
2. **Use float scans for positions** - Movement values are usually floats
3. **Use unknown initial value** - For counts and dynamic values
4. **Use pointer scan** - For finding stable pointers
5. **Use "Find what writes"** - For discovering functions

### Pattern Creation Tips
1. **Use unique bytes** - Avoid common instructions (push/pop/mov)
2. **Use enough bytes** - 10-15 bytes minimum for uniqueness
3. **Use wildcards sparingly** - Only for values that truly change
4. **Test patterns** - Verify they work after game restart
5. **Document assembly** - Keep original assembly code in comments

---

## 🎓 Learning Resources

### Recommended Reading
- [Cheat Engine Tutorial](https://wiki.cheatengine.org/index.php?title=Tutorials)
- [x86 Assembly Guide](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)
- [Pattern Scanning Explained](https://guidedhacking.com/threads/how-to-make-game-hacks-pattern-scanning.14112/)
- [Pointer Scanning Guide](https://wiki.cheatengine.org/index.php?title=Pointer_scan)

### Video Tutorials
- Search YouTube: "Cheat Engine Tutorial"
- Search YouTube: "Game Hacking Tutorial"
- Search YouTube: "Reverse Engineering Games"

---

## 📞 Support

### If You Get Stuck
1. **Review documentation** - Check PHASE2_REVERSE_ENGINEERING.md
2. **Check quick reference** - See PHASE2_QUICK_REFERENCE.md
3. **Review examples** - Look at pattern examples in Patterns.h
4. **Search online** - Many game hacking tutorials available
5. **Take a break** - Fresh eyes often solve problems

### Common Questions
**Q: I found too many results (1000+)**
A: Be more specific with scan type, wait for more value changes

**Q: Address changes every restart**
A: Use pointer scanning to find stable base pointer

**Q: Can't find function**
A: Use "Find what writes" on a value that the function modifies

**Q: Pattern doesn't work after restart**
A: Pattern might be too specific or include dynamic values, add more wildcards

---

## ✅ Definition of Done

Phase 2 is complete when:

1. ✅ All player offsets found and documented
2. ✅ Entity manager structure found and documented
3. ✅ At least 3 critical functions found and documented
4. ✅ Patterns created for all critical addresses
5. ✅ All patterns verified after game restart
6. ✅ Offsets.h updated with actual values
7. ✅ Patterns.h updated with actual patterns
8. ✅ MEMORY_MAP.md completed with full documentation
9. ✅ Cheat Engine table saved and working
10. ✅ Ready to proceed to Phase 3 (Implementation)

---

## 🚀 Next Phase

**Phase 3: Game State & Basic Hooks (Week 6-7)**

After Phase 2 completion, you will:
1. Implement `Player` class using found offsets
2. Implement `Entity` class using found offsets
3. Implement pattern scanning in `MemoryManager::Initialize()`
4. Test reading player HP/MP/Position in real-time
5. Test reading entity list in real-time
6. Begin DirectX hook preparation

---

**Status Updated**: 2025-10-15
**Next Update**: [When Phase 2 execution begins]
