# Reverse Engineering Documentation

Tài liệu về reverse engineering game JX1 với Cheat Engine.

---

## 📚 Documents

### [CHEAT_ENGINE_GUIDE.md](CHEAT_ENGINE_GUIDE.md) ⭐ HƯỚNG DẪN CHÍNH
- Complete Cheat Engine tutorial
- Tutorial 1: Find Player HP
- Tutorial 2: Find Pointer Chain
- Tutorial 3: Find Player Struct
- Tutorial 4: Find Functions (Pattern Scanning)
- Tutorial 5: Find Entity List
- Template code examples

**Dành cho:** Phase 2 - Reverse Engineering, tìm offsets

**Thời gian:** 2-4 giờ để hoàn thành

---

### [CE_QUICK_REFERENCE.md](CE_QUICK_REFERENCE.md) ⭐ THAM KHẢO NHANH
- Cheat Engine hotkeys
- Scan types reference
- Value types
- Quick workflow
- Pattern syntax

**Dành cho:** Quick reference khi đang làm việc với CE

---

### [MEMORY_MAP.md](MEMORY_MAP.md)
- Game memory layout
- Memory regions
- Important addresses
- Documentation of findings

**Dành cho:** Document kết quả reverse engineering

---

### Phase 2 Documents

#### [PHASE2_INDEX.md](PHASE2_INDEX.md)
- Phase 2 overview
- Task list
- Progress tracking

#### [PHASE2_EXECUTION_GUIDE.md](PHASE2_EXECUTION_GUIDE.md)
- Step-by-step execution plan
- Detailed instructions

#### [PHASE2_QUICK_REFERENCE.md](PHASE2_QUICK_REFERENCE.md)
- Quick tips for Phase 2

#### [PHASE2_REVERSE_ENGINEERING.md](PHASE2_REVERSE_ENGINEERING.md)
- Detailed RE methodology

#### [PHASE2_STATUS.md](PHASE2_STATUS.md)
- Current progress
- Findings status

---

## 🎯 Phase 2 Workflow

```
1. Install Cheat Engine
   ↓
2. Read CHEAT_ENGINE_GUIDE.md
   ↓
3. Follow Tutorial 1-5
   ↓
4. Document findings in:
   - core_dll/src/utils/Offsets.h
   - core_dll/src/utils/Patterns.h
   ↓
5. Update PHASE2_STATUS.md
```

---

## ✅ Phase 2 Checklist

### Player Data
- [ ] HP Current/Max
- [ ] MP Current/Max
- [ ] Position (X, Y, Z)
- [ ] Level
- [ ] Name
- [ ] Gold

### Entity Data
- [ ] Entity List Base
- [ ] Entity Count
- [ ] Entity HP
- [ ] Entity Position
- [ ] Entity Type/ID

### Functions
- [ ] Attack Function Pattern
- [ ] Move Function Pattern
- [ ] UseItem Function Pattern

---

## 📖 Recommended Reading Order

```
First Time:
1. CHEAT_ENGINE_GUIDE.md    ← Complete tutorial
2. CE_QUICK_REFERENCE.md    ← Bookmark for reference

When Working:
- CE_QUICK_REFERENCE.md     ← Keep open
- PHASE2_STATUS.md          ← Update progress

When Documenting:
- MEMORY_MAP.md             ← Document findings
```

---

## 🛠️ Tools

```
✅ Cheat Engine 7.5+           (Required)
✅ JX1 Game                    (Required)
□ x64dbg                       (Optional - advanced)
□ Process Hacker               (Optional - monitoring)
```

---

## 💡 Quick Tips

### Tìm HP
```
1. Scan: 4 Bytes, Exact Value
2. Change HP in game
3. Next Scan
4. Repeat until < 10 results
```

### Tìm Pointer
```
1. Right-click address
2. Pointer scan
3. Restart game
4. Rescan 2-3 times
```

### Tìm Pattern
```
1. Right-click → "Find what writes"
2. View disassembler
3. Copy unique bytes
4. Use wildcards for dynamic values
```

---

## 📊 Expected Results

After Phase 2, you will have:

**Offsets.h:**
```cpp
constexpr uintptr_t PLAYER_BASE = 0x00A2B4C0;
struct Player {
    static constexpr int HP = 0x10;
    static constexpr int MP = 0x14;
    // ...
};
```

**Patterns.h:**
```cpp
constexpr const char* ATTACK_FUNCTION =
    "55 8B EC 83 EC ? 56 8B F1";
```

---

## 🔗 Related

- **Implementation**: [../04-development/](../04-development/)
- **Architecture**: [../02-architecture/](../02-architecture/)

---

## 🆘 Need Help?

### Common Issues

**Can't find address:**
→ Try Unknown Initial scan → Changed/Unchanged

**Pointer doesn't work:**
→ Increase max level to 7, rescan more times

**Pattern doesn't match:**
→ Use wildcards (?), try shorter pattern

**Full troubleshooting:** See [CHEAT_ENGINE_GUIDE.md](CHEAT_ENGINE_GUIDE.md)

---

**Back to:** [Main Documentation](../)
