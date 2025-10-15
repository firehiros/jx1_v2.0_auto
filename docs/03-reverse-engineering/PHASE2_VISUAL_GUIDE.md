# Phase 2 Visual Guide - Hướng Dẫn Bằng Hình Ảnh

**Guide đơn giản hóa với visual examples**

---

## 🎯 Overview: Bạn Sẽ Làm Gì?

```
Session 1: Tìm Player HP
    ↓
Found HP at: 0x12345678
    ↓
Tìm Player Base Pointer
    ↓
Found: JX1.exe + 0x00ABCDEF
    ↓
Calculate offsets cho tất cả values
    ↓
Player HP = Base + 0x10
Player MP = Base + 0x14
Player Position X = Base + 0x20
etc.
    ↓
Update Offsets.h
    ↓
✅ Done Session 1!
```

---

## 📸 Visual Examples

### Example 1: Finding Player HP

**Step 1: Cheat Engine Setup**
```
┌─────────────────────────────────────────┐
│ Cheat Engine 7.5                   [_][□][X]│
├─────────────────────────────────────────┤
│ ▼ Value Type: [4 Bytes ▼]              │
│ ▼ Scan Type:  [Exact Value ▼]          │
│ Value: [850        ]                    │← Nhập HP hiện tại
│                                         │
│ [First Scan] [Next Scan] [New Scan]    │
├─────────────────────────────────────────┤
│ Address         | Value                 │
│ (empty - chưa scan)                     │
└─────────────────────────────────────────┘
```

**Step 2: After First Scan**
```
┌─────────────────────────────────────────┐
│ Found: 15,234 addresses                 │
├─────────────────────────────────────────┤
│ Address         | Value                 │
│ 0x00123456     | 850                    │
│ 0x00234567     | 850                    │
│ 0x00345678     | 850                    │
│ ... (15,234 more)                       │
└─────────────────────────────────────────┘

Quá nhiều! Cần narrow down →
```

**Step 3: Take Damage & Next Scan**
```
Trong game: Take damage → HP: 850 → 720

Cheat Engine:
Value: [720        ]
Click [Next Scan]

Found: 234 addresses ← Tốt hơn!
```

**Step 4: Repeat**
```
Heal → 820
Next Scan → 45 addresses

Take damage → 650
Next Scan → 8 addresses ← Gần xong!

Heal → 900
Next Scan → 3 addresses ← Perfect!
```

**Step 5: Test Addresses**
```
Double-click address 1 → Xuất hiện ở bảng dưới

┌─────────────────────────────────────────┐
│ [✓] Active | Description  | Value       │
│ [ ] 0x1234 | Player HP    | 900         │← Check box này
│ [ ] 0x5678 | Unknown      | 900         │
│ [ ] 0x9ABC | Unknown      | 900         │
└─────────────────────────────────────────┘

Change value to 9999:
┌─────────────────────────────────────────┐
│ [✓] Active | Description  | Value       │
│ [✓] 0x1234 | Player HP    | [9999  ]    │← Type 9999
└─────────────────────────────────────────┘

Check game: HP = 9999? → YES! ✅ Found it!
```

---

### Example 2: Finding Player Position

**Movement Test**:
```
Step 1: Scan Type "Unknown initial value"
Step 2: Move EAST → "Increased value"
Step 3: Move WEST → "Decreased value"

Results:
┌─────────────────────────────────────────┐
│ Found 3 addresses (likely X, Y, Z)      │
│ 0x1234 → Changes when move E/W ← X     │
│ 0x1238 → Changes when move N/S ← Y     │
│ 0x123C → Changes when jump     ← Z     │
└─────────────────────────────────────────┘
```

---

### Example 3: Pointer Scan

**Why Pointers?**
```
Without Pointer:
  Game Restart → Address thay đổi
  0x12345678 → 0x87654321 ← Khác!
  Must find lại mỗi lần ❌

With Pointer:
  Game Restart → Pointer không đổi
  JX1.exe+0xABCDEF → Always same ✅
  Auto-find mỗi lần!
```

**Pointer Scan Steps**:
```
1. Right-click HP address
   ┌──────────────────────┐
   │ Copy                 │
   │ Change value         │
   │ Browse memory region │
   │ ► Pointer scan...   │← Click this
   │ Find what writes     │
   └──────────────────────┘

2. Pointer Scanner opens
   ┌─────────────────────────────────┐
   │ Scanning for pointers to        │
   │ 0x12345678                      │
   │                                 │
   │ [OK] [Cancel]                   │
   └─────────────────────────────────┘
   Click OK → Wait 1-5 minutes

3. Results (maybe 1000+ pointers)
   ┌─────────────────────────────────┐
   │ Pointers found: 1,234           │
   │                                 │
   │ Level | Address        | Offset │
   │   1   | JX1.exe+ABCDEF | 10    │← Good!
   │   2   | JX1.exe+123456 | 10,8  │
   │   3   | Heap+...       | ...   │← Bad
   └─────────────────────────────────┘

4. Close game & CE
5. Restart game & CE
6. Find HP lại (quick)
7. Pointer scanner → Rescan memory
8. Enter new HP address
9. Pointers that still work = STABLE! ✅
```

---

### Example 4: Finding Functions

**"Find What Writes" Method**:
```
1. Find monster HP (same as player HP)

2. Right-click monster HP
   ┌──────────────────────────────┐
   │ Change value                 │
   │ ► Find out what writes...   │← Click
   │ Find out what accesses       │
   └──────────────────────────────┘

3. New window opens (empty):
   ┌─────────────────────────────────┐
   │ The following opcodes write to  │
   │ this address:                   │
   │                                 │
   │ (empty - waiting for action)    │
   └─────────────────────────────────┘

4. In game: ATTACK monster

5. Instructions appear:
   ┌─────────────────────────────────┐
   │ 0x00401234 - sub [eax+10],edx   │← This!
   │ 0x00405678 - mov [ecx+10],eax   │
   └─────────────────────────────────┘

6. Double-click instruction → Memory Viewer

7. Assembly code:
   ┌─────────────────────────────────┐
   │ 00401000  55        push ebp    │← Function start!
   │ 00401001  8B EC     mov ebp,esp│
   │ 00401003  83 EC 20  sub esp,20 │
   │ 00401006  56        push esi   │
   │ 00401007  8B F1     mov esi,ecx│
   │ ...                             │
   │ 00401234  29 50 10  sub [eax+10],edx │← Our instruction
   └─────────────────────────────────┘

Function found: 0x00401000
```

---

### Example 5: Creating Patterns

**From Assembly to Pattern**:
```
Assembly (first 9 bytes):
┌──────────────────────────────────────┐
│ Address  | Hex        | Assembly    │
├──────────┼────────────┼─────────────┤
│ 00401000 | 55         | push ebp    │
│ 00401001 | 8B EC      | mov ebp,esp │
│ 00401003 | 83 EC 20   | sub esp,20  │← 0x20 might change!
│ 00401006 | 56         | push esi    │
│ 00401007 | 8B F1      | mov esi,ecx │
└──────────────────────────────────────┘

Pattern (with wildcard for 0x20):
  "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1"
                         ↑
                    Wildcard (0x00)

Mask:
  "xxxxx?xxx"
        ↑
   Wildcard position
```

**In Patterns.h**:
```cpp
constexpr const char* ATTACK_FUNCTION =
    "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1";
constexpr const char* ATTACK_FUNCTION_MASK =
    "xxxxx?xxx";
```

---

## 🎯 Visual Workflow

### Complete Phase 2 Flow

```
START
  │
  ├─ Session 1: Player Structure
  │    ├─ Find HP ──────────────────→ Note offset: 0x10
  │    ├─ Find MP ──────────────────→ Note offset: 0x14
  │    ├─ Find Position ────────────→ Note offsets: 0x20,0x24,0x28
  │    ├─ Find Level, Gold, Name ───→ Note offsets
  │    └─ Find Base Pointer ────────→ JX1.exe + 0xABCDEF ✅
  │         │
  │         └─ Update Offsets.h
  │
  ├─ Session 2: Entity Manager
  │    ├─ Find Entity Count ────────→ Note address
  │    ├─ Find Entity Array ────────→ Note address
  │    └─ Find Entity Structure ────→ Note offsets
  │         │
  │         └─ Update Offsets.h
  │
  ├─ Session 3: Functions
  │    ├─ Find Attack function ─────→ Address: 0x401000
  │    ├─ Find MoveTo function ─────→ Address: 0x402000
  │    ├─ Find UseSkill ────────────→ Address: 0x403000
  │    └─ Find other functions ─────→ Note addresses
  │         │
  │         └─ Copy assembly code
  │
  └─ Session 4: Patterns
       ├─ Create patterns ──────────→ From assembly
       ├─ Test patterns ────────────→ Restart game test
       └─ Update Patterns.h ────────→ Add all patterns
            │
            └─ COMPLETE! ✅
```

---

## 📊 Progress Tracking

### Use This Checklist

```
PLAYER STRUCTURE:
[_] HP offset: 0x____
[_] Max HP offset: 0x____
[_] MP offset: 0x____
[_] Max MP offset: 0x____
[_] Position X: 0x____
[_] Position Y: 0x____
[_] Position Z: 0x____
[_] Level: 0x____
[_] Gold: 0x____
[_] Name: 0x____
[_] Base Pointer: JX1.exe + 0x________

ENTITY MANAGER:
[_] Entity Count: 0x________
[_] Entity Array: 0x________
[_] Entity ID offset: 0x____
[_] Entity Type offset: 0x____
[_] Entity HP offset: 0x____
[_] Entity Position offsets: 0x____, 0x____, 0x____

FUNCTIONS:
[_] Attack: 0x________
[_] MoveTo: 0x________
[_] UseSkill: 0x________
[_] UseItem: 0x________
[_] PickupItem: 0x________
[_] SendChat: 0x________

PATTERNS:
[_] Player Base pattern created
[_] Entity Manager pattern created
[_] Attack function pattern created
[_] MoveTo function pattern created
[_] All patterns tested ✅
```

---

## 🎓 Cheat Engine Interface Guide

### Main Window Layout

```
┌──────────────────────────────────────────────┐
│ Cheat Engine 7.5                        [_][□][X]│
├──────────────────────────────────────────────┤
│ File Edit ... Help                           │
├──────────────────────────────────────────────┤
│ [PC Icon] Select Process: JX1.exe ▼          │
├──────────────────────────────────────────────┤
│                                              │
│ ▼ Value Type: [4 Bytes      ▼]              │
│ ▼ Scan Type:  [Exact Value  ▼]              │
│ Value:        [850          ]                │
│                                              │
│ Hex: [ ]  From: [ ] To: [ ]                 │
│                                              │
│ [First Scan] [Next Scan] [New Scan]         │
│ [Undo Scan] [Cancel]                        │
├──────────────────────────────────────────────┤
│ SCAN RESULTS:                                │
│ Address      | Value                         │
│ 0x00123456  | 850                           │
│ 0x00234567  | 850                           │
│ ... (15,234 more)                           │
└──────────────────────────────────────────────┘
```

### Bottom Panel (After Double-Click)

```
┌──────────────────────────────────────────────┐
│ ADDRESS LIST:                                │
├──────────────────────────────────────────────┤
│ [✓]│ Description  │ Address    │ Value      │
├────┼──────────────┼────────────┼────────────┤
│ [ ]│ Player HP    │ 0x00123456 │ 850        │
│ [ ]│ Player MP    │ 0x00123460 │ 500        │
│ [ ]│ Position X   │ 0x00123470 │ 1234.5     │
└──────────────────────────────────────────────┘
      ↑
  Check để freeze/modify value
```

---

## 🔢 Common Value Types

| Value Type | Use For | Example |
|------------|---------|---------|
| **4 Bytes** | HP, MP, Level, Count | 850 |
| **Float** | Position X/Y/Z | 1234.5 |
| **8 Bytes** | Gold, Experience | 999999 |
| **String** | Name | "YourChar" |
| **2 Bytes** | Small numbers | 50 |

---

## 📝 Quick Reference Card

### Scan Strategies

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
KNOWN VALUE (HP, Level):
  1. First Scan: Exact Value
  2. Change in game
  3. Next Scan: Exact Value (new)
  4. Repeat until < 10 results
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UNKNOWN VALUE (Entity Count):
  1. First Scan: Unknown initial value
  2. Change in game (kill/spawn)
  3. Next Scan: Increased/Decreased
  4. Repeat
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
FLOAT VALUE (Position):
  1. Value Type: Float
  2. First Scan: Unknown initial value
  3. Move character
  4. Next Scan: Increased/Decreased
  5. Repeat
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

---

## ⏱️ Session Breakdown

### Session 1: Player (2-3h) ⭐⭐

```
00:00 - Setup (15 min)
00:15 - Find HP (30 min)
00:45 - Find MP (20 min)
01:05 - Find Position (45 min)
01:50 - Find Level/Gold (30 min)
02:20 - Find Base Pointer (60 min) ← Most important!
03:20 - Document & Verify (30 min)
──────────────────────────────
Total: ~3 hours
```

### Session 2: Entities (2-3h) ⭐⭐⭐

```
00:00 - Find Entity Count (30 min)
00:30 - Find Entity Array (45 min)
01:15 - Map Entity Structure (60 min)
02:15 - Document (30 min)
02:45 - Verify (15 min)
──────────────────────────────
Total: ~3 hours
```

### Session 3: Functions (2-3h) ⭐⭐⭐⭐

```
00:00 - Find Attack (45 min)
00:45 - Find MoveTo (45 min)
01:30 - Find UseSkill (45 min)
02:15 - Find Others (30 min)
02:45 - Document (15 min)
──────────────────────────────
Total: ~3 hours
```

### Session 4: Patterns (1-2h) ⭐⭐

```
00:00 - Create patterns (60 min)
01:00 - Test patterns (30 min)
01:30 - Update code (30 min)
──────────────────────────────
Total: ~2 hours
```

---

## 🎯 Success Indicators

### After 30 Minutes
✅ Found Player HP
✅ Understand workflow
✅ Ready to continue

### After 3 Hours
✅ Player structure complete
✅ Base pointer found
✅ All player offsets known

### After 6 Hours
✅ Player + Entity structures done
✅ Offsets documented

### After 9 Hours
✅ Functions found
✅ Assembly code copied

### After 12 Hours
✅ Patterns created và tested
✅ All code files updated
✅ **PHASE 2 COMPLETE!** 🎉

---

## 📞 Common Questions

### Q: Tôi tìm được 1000+ results, làm sao?
**A**: Change value thêm nhiều lần. HP thay đổi thường xuyên → dễ narrow down.

### Q: Address thay đổi sau restart?
**A**: Đúng rồi! Đó là tại sao cần pointer scan.

### Q: Tôi không tìm được function?
**A**: Dùng "Find what writes" trên value mà function modify (e.g., HP decrease = Attack).

### Q: Pattern không work sau restart?
**A**: Pattern có thể chứa dynamic values. Add thêm wildcards.

### Q: Mất bao lâu?
**A**: 8-12 giờ total, có thể chia 4 sessions.

---

## 🎁 Reward After Phase 2

```
BEFORE Phase 2:
  [Core DLL] → Can't find player ❌
  [ESP] → Shows nothing ❌
  [Bots] → Can't work ❌

AFTER Phase 2:
  [Core DLL] → Finds everything ✅
  [ESP] → Shows all entities ✅
  [Bots] → Fully functional ✅

Result: 12,090 dòng code HOẠT ĐỘNG! 🎉
```

---

## 🚀 START NOW!

### Step 1: Read This (5 min)
✅ You're reading it now!

### Step 2: Install CE (5 min)
→ Download Cheat Engine

### Step 3: Open Main Guide (2 min)
→ [docs/PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md)

### Step 4: Begin Session 1 (2-3 hours)
→ Follow step-by-step
→ Check boxes
→ Find Player HP!

---

**Good luck!** 🍀

Remember: **Save frequently, test pointers, document immediately!**

---

**Visual Guide Complete** ✅
**Next**: [docs/PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md) ← START HERE
