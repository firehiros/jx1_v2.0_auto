# Phase 2 Documentation Index

**Quick Navigation Guide for Phase 2 - Reverse Engineering**

---

## 🎯 Start Here

**New to Phase 2? Read in this order:**

1. **[📋 PHASE2_READY.md](../PHASE2_READY.md)** ← Start here!
   - Overview of Phase 2
   - What's been prepared
   - Success criteria
   - Quick start checklist

2. **[📘 PHASE2_REVERSE_ENGINEERING.md](PHASE2_REVERSE_ENGINEERING.md)**
   - Complete tutorial on reverse engineering JX1
   - Cheat Engine usage guide
   - Theory and concepts
   - Read once before starting

3. **[✅ PHASE2_EXECUTION_GUIDE.md](PHASE2_EXECUTION_GUIDE.md)** ← Your main work guide
   - Step-by-step execution plan
   - Checkbox-based workflow
   - **Use this while working**
   - 4 sessions with detailed steps

---

## 📚 Reference Materials

**Keep these open while working:**

### During Active Work
- **[📋 PHASE2_QUICK_REFERENCE.md](PHASE2_QUICK_REFERENCE.md)**
  - Cheat Engine command cheatsheet
  - Memory scanning strategies
  - Pattern creation guide
  - Common issues & solutions
  - **Keep open in second monitor or print it**

### For Documentation
- **[📝 MEMORY_MAP.md](MEMORY_MAP.md)**
  - Template for documenting findings
  - Fill out as you discover offsets
  - Will be your reference for Phase 3+

### For Progress Tracking
- **[📊 PHASE2_STATUS.md](PHASE2_STATUS.md)**
  - Detailed task breakdown
  - Progress tracking
  - Success criteria
  - Update as you complete tasks

---

## 🛠️ Tools & Code

### Cheat Engine Template
- **[tools/JX1_Template.CT](../tools/JX1_Template.CT)**
  - Pre-organized CE table
  - Load this in Cheat Engine
  - Fill out as you find addresses

### Code Templates to Update
- **[core_dll/src/utils/Offsets.h](../core_dll/src/utils/Offsets.h)**
  - Replace `0x0` with actual offsets
  - Add verification comments

- **[core_dll/src/utils/Patterns.h](../core_dll/src/utils/Patterns.h)**
  - Replace empty patterns with actual patterns
  - Add assembly code comments

---

## 📖 Document Purpose Quick Reference

| Document | Purpose | When to Use |
|----------|---------|-------------|
| **PHASE2_READY.md** | Overview & quick start | Before starting Phase 2 |
| **PHASE2_REVERSE_ENGINEERING.md** | Concepts & theory | First-time read, reference |
| **PHASE2_EXECUTION_GUIDE.md** | Step-by-step work plan | **During all work sessions** |
| **PHASE2_QUICK_REFERENCE.md** | Command cheatsheet | **During work, quick lookup** |
| **MEMORY_MAP.md** | Document findings | **During work, fill out** |
| **PHASE2_STATUS.md** | Progress tracking | Track completion status |
| **JX1_Template.CT** | Cheat Engine table | Load in CE, fill out |
| **Offsets.h** | Code template | Update with found offsets |
| **Patterns.h** | Code template | Update with found patterns |

---

## 🎯 Recommended Workflow

### First Time Setup (Day 0)
1. Read `PHASE2_READY.md` (5 min)
2. Read `PHASE2_REVERSE_ENGINEERING.md` (30 min)
3. Skim `PHASE2_EXECUTION_GUIDE.md` (10 min)
4. Install Cheat Engine on Windows (5 min)

### Session 1: Player Structure (2-3 hours)
1. Open `PHASE2_EXECUTION_GUIDE.md` → Session 1
2. Open `PHASE2_QUICK_REFERENCE.md` for reference
3. Open `MEMORY_MAP.md` to document findings
4. Load `JX1_Template.CT` in Cheat Engine
5. Follow Session 1 checkboxes step by step
6. Update `Offsets.h` with found player offsets
7. Update `MEMORY_MAP.md` with player structure
8. Save CE table as `JX1_Session1_Player.CT`

### Session 2: Entity Manager (2-3 hours)
1. Open `PHASE2_EXECUTION_GUIDE.md` → Session 2
2. Keep `PHASE2_QUICK_REFERENCE.md` open
3. Keep `MEMORY_MAP.md` open for notes
4. Load previous session's CE table
5. Follow Session 2 checkboxes
6. Update `Offsets.h` with entity offsets
7. Update `MEMORY_MAP.md` with entity structure
8. Save CE table as `JX1_Session2_Entities.CT`

### Session 3: Functions & Patterns (2-3 hours)
1. Open `PHASE2_EXECUTION_GUIDE.md` → Session 3
2. Keep `PHASE2_QUICK_REFERENCE.md` open
3. Keep `MEMORY_MAP.md` open
4. Follow Session 3 checkboxes
5. Update `Patterns.h` with all patterns
6. Update `MEMORY_MAP.md` with function documentation
7. Save CE table as `JX1_Session3_Functions.CT`

### Session 4: Verification (1-2 hours)
1. Open `PHASE2_EXECUTION_GUIDE.md` → Session 4
2. Restart game and CE (clean test)
3. Verify all findings
4. Complete `MEMORY_MAP.md`
5. Finalize `Offsets.h` and `Patterns.h`
6. Save final CE table as `JX1_V2.0_Final.CT`
7. Update `PHASE2_STATUS.md` to "Complete"

---

## 🔍 Quick Search Guide

### Looking for...

**"How do I find Player HP?"**
→ `PHASE2_EXECUTION_GUIDE.md` → Session 1 → Finding Player HP

**"How do I create a pattern?"**
→ `PHASE2_QUICK_REFERENCE.md` → Pattern Creation Guide
→ `PHASE2_REVERSE_ENGINEERING.md` → Pattern Creation Tutorial

**"What's the Cheat Engine command for...?"**
→ `PHASE2_QUICK_REFERENCE.md` → Cheat Engine Quick Commands

**"I have too many scan results, what do I do?"**
→ `PHASE2_QUICK_REFERENCE.md` → Common Issues & Solutions

**"How do I find a function address?"**
→ `PHASE2_EXECUTION_GUIDE.md` → Session 3 → Finding Attack Function
→ `PHASE2_REVERSE_ENGINEERING.md` → Finding Critical Functions

**"Where do I document my findings?"**
→ `MEMORY_MAP.md` ← Fill this out
→ `Offsets.h` ← Update with offsets
→ `Patterns.h` ← Update with patterns

**"How do I verify my patterns work?"**
→ `PHASE2_EXECUTION_GUIDE.md` → Session 4 → Verify Static Addresses

---

## 📱 Mobile/Print-Friendly Version

For quick reference while working:

### Print These (Optional):
1. `PHASE2_QUICK_REFERENCE.md` (10 pages)
   - Cheatsheet for CE commands
   - Keep next to keyboard

2. `PHASE2_EXECUTION_GUIDE.md` Session Checklists
   - Print current session
   - Check boxes with pen as you work

---

## 🎓 Learning Path

### Complete Beginner (Never used Cheat Engine)
1. Read `PHASE2_REVERSE_ENGINEERING.md` - Full tutorial
2. Watch Cheat Engine YouTube tutorials (external)
3. Practice on a simple game first (optional)
4. Then use `PHASE2_EXECUTION_GUIDE.md`

### Intermediate (Used CE before, new to game hacking)
1. Skim `PHASE2_REVERSE_ENGINEERING.md` - Focus on patterns section
2. Use `PHASE2_EXECUTION_GUIDE.md` as main guide
3. Reference `PHASE2_QUICK_REFERENCE.md` when stuck

### Advanced (Experienced with game hacking)
1. Use `PHASE2_QUICK_REFERENCE.md` as main reference
2. Use `PHASE2_EXECUTION_GUIDE.md` as checklist only
3. Quickly fill out `MEMORY_MAP.md`, `Offsets.h`, `Patterns.h`

---

## 🚨 Troubleshooting Index

### Common Issues:

**"Too many scan results"**
→ `PHASE2_QUICK_REFERENCE.md` → Common Issues → Too Many Results

**"Address changes after restart"**
→ `PHASE2_EXECUTION_GUIDE.md` → Session 1 → Finding Player Base Pointer
→ `PHASE2_REVERSE_ENGINEERING.md` → Pointer Scanning

**"Can't find function address"**
→ `PHASE2_QUICK_REFERENCE.md` → Common Issues → Can't Find Function
→ `PHASE2_REVERSE_ENGINEERING.md` → Finding Critical Functions

**"Pattern doesn't work after restart"**
→ `PHASE2_QUICK_REFERENCE.md` → Common Issues → Pattern Fails
→ `PHASE2_REVERSE_ENGINEERING.md` → Pattern Creation → Tips

**"Game crashes when I attach CE"**
→ Contact publisher (you have partnership)
→ Try CE with anti-anti-debug options

---

## 📊 Progress Tracking

### Check Your Progress:

**Session 1 Complete?**
- [ ] Player HP, MP found
- [ ] Player Position X, Y, Z found
- [ ] Player Level, Gold found
- [ ] Player base pointer found
- [ ] All player offsets calculated
- [ ] `Offsets.h` updated
- [ ] `MEMORY_MAP.md` Player section complete

**Session 2 Complete?**
- [ ] Entity count found
- [ ] Entity array pointer found
- [ ] Entity structure mapped
- [ ] Entity size determined
- [ ] `Offsets.h` updated with entity offsets
- [ ] `MEMORY_MAP.md` Entity section complete

**Session 3 Complete?**
- [ ] Attack function found
- [ ] MoveTo function found
- [ ] UseSkill function found
- [ ] Patterns created for all functions
- [ ] Patterns created for base pointers
- [ ] `Patterns.h` updated
- [ ] `MEMORY_MAP.md` Functions section complete

**Session 4 Complete?**
- [ ] All addresses verified after restart
- [ ] All patterns tested
- [ ] `MEMORY_MAP.md` 100% complete
- [ ] `Offsets.h` 100% complete
- [ ] `Patterns.h` 100% complete
- [ ] Final CE table saved
- [ ] `PHASE2_STATUS.md` marked complete

---

## 🎉 Completion Checklist

Phase 2 is complete when ALL of these are ✅:

- [ ] ✅ Read all documentation
- [ ] ✅ Player structure fully mapped
- [ ] ✅ Entity manager fully mapped
- [ ] ✅ At least 3 functions found
- [ ] ✅ All patterns created and tested
- [ ] ✅ `MEMORY_MAP.md` complete
- [ ] ✅ `Offsets.h` updated with real values
- [ ] ✅ `Patterns.h` updated with real patterns
- [ ] ✅ Final CE table saved
- [ ] ✅ All findings verified after restart
- [ ] ✅ Ready to proceed to Phase 3

---

## 🚀 After Phase 2

**Next Phase**: Phase 3 - Implementation

You'll use your findings to:
1. Implement pattern scanning in code
2. Create Player class that reads real-time data
3. Create Entity class that reads entity list
4. Test everything with DLL injection

---

## 📞 Quick Links

| Resource | Location |
|----------|----------|
| **Main Guide** | [PHASE2_EXECUTION_GUIDE.md](PHASE2_EXECUTION_GUIDE.md) |
| **Cheatsheet** | [PHASE2_QUICK_REFERENCE.md](PHASE2_QUICK_REFERENCE.md) |
| **Findings Template** | [MEMORY_MAP.md](MEMORY_MAP.md) |
| **CE Table** | [../tools/JX1_Template.CT](../tools/JX1_Template.CT) |
| **Offsets** | [../core_dll/src/utils/Offsets.h](../core_dll/src/utils/Offsets.h) |
| **Patterns** | [../core_dll/src/utils/Patterns.h](../core_dll/src/utils/Patterns.h) |
| **Status** | [PHASE2_STATUS.md](PHASE2_STATUS.md) |
| **Overview** | [../PHASE2_READY.md](../PHASE2_READY.md) |

---

**Phase 2 Documentation Index**
**Last Updated**: 2025-10-15
**Total Documents**: 9 files
**Status**: 📚 Complete Reference Library

Happy reverse engineering! 🔍🎮
