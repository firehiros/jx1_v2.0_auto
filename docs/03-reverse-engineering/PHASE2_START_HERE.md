# 🎯 PHASE 2 - BẮT ĐẦU TẠI ĐÂY

**Mục đích**: Tìm memory offsets và function patterns để unlock 12,090 dòng code đã viết!

**Thời gian**: 8-12 giờ (có thể chia làm 4 sessions)

**Yêu cầu**: Windows 10/11 + JX1 game + Cheat Engine

---

## 🚀 Quick Start (5 Phút)

### Bước 1: Chuẩn Bị Môi Trường

**Cần có**:
- ✅ Windows 10 hoặc 11
- ✅ JX1 game đã cài đặt
- ✅ Cheat Engine 7.5+ ([Download](https://www.cheatengine.org/downloads.php))
- ✅ Wireshark (optional, cho network analysis) ([Download](https://www.wireshark.org/download.html))

### Bước 2: Mở Tài Liệu

**Mở các file sau** (theo thứ tự):

1. **[PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md)** ← **FILE CHÍNH**
   - Hướng dẫn từng bước với checkboxes
   - Follow đúng thứ tự
   - Check boxes khi xong mỗi bước

2. **[PHASE2_QUICK_REFERENCE.md](docs/PHASE2_QUICK_REFERENCE.md)** ← Mở song song
   - Cheatsheet cho Cheat Engine
   - Mở trong tab khác để tra cứu nhanh

3. **[MEMORY_MAP.md](docs/MEMORY_MAP.md)** ← Ghi chú findings
   - Template để document kết quả
   - Fill in khi tìm được offsets

### Bước 3: Launch Tools

1. **Launch JX1**
   - Chạy game
   - Login vào character
   - Đứng ở nơi an toàn

2. **Launch Cheat Engine**
   - Click icon máy tính (Open Process)
   - Chọn "JX1.exe"
   - Click "Open"

3. **Load Template**
   - File → Load
   - Chọn `tools/JX1_Template.CT`

### Bước 4: Follow Guide

**Mở file này và follow từng bước**:
[PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md)

---

## 📖 Tổng Quan Phase 2

### Bạn Sẽ Tìm Gì?

**Session 1: Player Structure** (2-3 giờ)
- ✅ Player HP, MP
- ✅ Player Position (X, Y, Z)
- ✅ Player Level, Gold, XP
- ✅ Player Name
- ✅ Player Base Pointer (QUAN TRỌNG!)

**Session 2: Entity Manager** (2-3 giờ)
- ✅ Entity Count
- ✅ Entity Array Pointer
- ✅ Entity Structure (ID, Type, HP, Position, Name)

**Session 3: Functions** (2-3 giờ)
- ✅ Attack Function
- ✅ MoveTo Function
- ✅ UseSkill Function
- ✅ UseItem Function
- ✅ PickupItem Function
- ✅ SendChat Function

**Session 4: Patterns & Network** (2-3 giờ)
- ✅ Create patterns cho tất cả
- ✅ Test patterns
- ✅ Network opcodes (Wireshark)
- ✅ Verify everything

---

## 🎯 Quy Trình Từng Bước

### SESSION 1: Player Structure (2-3 giờ)

#### 1.1 Tìm Player HP (30 phút)

**Steps**:
```
1. Note HP hiện tại trong game: ______ HP
2. Cheat Engine:
   - Value Type: 4 Bytes
   - Scan Type: Exact Value
   - Value: [Current HP]
   - Click "First Scan"
3. Trong game: Bị damage (để HP giảm)
4. Note HP mới: ______ HP
5. Cheat Engine:
   - Value: [New HP]
   - Click "Next Scan"
6. Lặp lại cho đến khi còn < 10 addresses
7. Test từng address (change value → check game)
8. Khi tìm được: Label "Player HP"
9. Note address: _______________
```

**Chi tiết**: Xem [PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md) - Session 1

#### 1.2 Tìm Player MP, Position, Level, Gold

Tương tự HP, follow guide cho từng field.

#### 1.3 Tìm Player Base Pointer (QUAN TRỌNG!)

**Tại sao quan trọng**: Addresses thay đổi mỗi lần restart game. Base pointer là STABLE.

**Steps**:
```
1. Right-click Player HP address
2. Select "Pointer scan for this address"
3. Wait for scan complete (1-5 phút)
4. Save scan: "player_hp_pointers.scandata"
5. Close game + CE
6. Restart game + CE
7. Scan lại HP (nhanh)
8. Pointer scanner → Rescan memory
9. Address: [New HP address]
10. Remaining pointers = STABLE pointers!
11. Chọn pointer với:
    - Level 1-3 (ít offsets)
    - Green base (JX1.exe+offset)
12. Test: Close/restart → pointer vẫn work?
```

**Result**: `JX1.exe + 0x________ → Player Base`

---

### SESSION 2: Entity Manager (2-3 giờ)

#### 2.1 Tìm Entity Count (30 phút)

**Steps**:
```
1. Count monsters/NPCs visible: _____ entities
2. Cheat Engine:
   - Value Type: 4 Bytes
   - Scan Type: Unknown initial value
   - Click "First Scan"
3. Kill monster hoặc spawn thêm
4. Click "Next Scan" → "Decreased value" (nếu kill)
5. Spawn monster hoặc di chuyển
6. Click "Next Scan" → "Increased value"
7. Lặp lại cho đến < 20 addresses
8. Label "Entity Count"
```

#### 2.2 Tìm Entity Structure

**Steps**:
```
1. Tìm tên monster (String search)
2. View memory region around name
3. Tìm ID, HP, Position gần đó
4. Tính offsets từ entity base
5. Document trong MEMORY_MAP.md
```

---

### SESSION 3: Functions (2-3 giờ)

#### 3.1 Tìm Attack Function (45 phút)

**Method**: Find what writes to enemy HP

**Steps**:
```
1. Tìm HP của monster (như tìm Player HP)
2. Right-click monster HP
3. Select "Find out what writes to this address"
4. Window mới mở ra (trống)
5. Trong game: Attack monster
6. Cheat Engine window hiện instructions
7. Double-click instruction → Memory Viewer
8. Scroll up tìm function start:
   push ebp
   mov ebp,esp
   sub esp,[number]
9. Note address: _______________
10. Copy 10-15 bytes đầu tiên (hex)
```

**Tìm các functions khác tương tự**:
- MoveTo: Find what writes to Position
- UseSkill: Find what writes khi dùng skill
- etc.

---

### SESSION 4: Patterns & Verification (2-3 giờ)

#### 4.1 Tạo Patterns

**Từ Assembly sang Pattern**:

```
Assembly:
  55              push ebp
  8B EC           mov ebp,esp
  83 EC 20        sub esp,20       ← 0x20 có thể thay đổi
  56              push esi
  8B F1           mov esi,ecx

Pattern:
  \x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1

Mask:
  xxxxx?xxx
        ↑ wildcard cho 0x20
```

Update trong [Patterns.h](core_dll/src/utils/Patterns.h)

#### 4.2 Network Analysis (Optional)

**Với Wireshark**:
```
1. Start Wireshark
2. Filter: tcp
3. Trong game: Move character
4. Find packet trong Wireshark
5. Note opcode
6. Update Packets.h
```

---

## 📝 Checklist Tổng Thể

### Trước Khi Bắt Đầu
- [ ] Windows machine ready
- [ ] JX1 installed và có thể login
- [ ] Cheat Engine 7.5+ installed
- [ ] Downloaded tất cả documentation
- [ ] Có 2-4 giờ thời gian không bị gián đoạn

### Phải Tìm Được
- [ ] Player HP offset
- [ ] Player MP offset
- [ ] Player Position X, Y, Z offsets
- [ ] Player Level offset
- [ ] Player Gold offset
- [ ] Player Name offset
- [ ] Player Base Pointer (JX1.exe + ???)
- [ ] Entity Count offset
- [ ] Entity Array Pointer offset
- [ ] Entity structure (ID, Type, HP, Position, Name offsets)
- [ ] Attack Function address + pattern
- [ ] MoveTo Function address + pattern
- [ ] UseSkill Function address + pattern
- [ ] UseItem Function address + pattern
- [ ] PickupItem Function address + pattern
- [ ] SendChat Function address + pattern

### Phải Update Files
- [ ] [Offsets.h](core_dll/src/utils/Offsets.h) - Replace 0x0 với actual values
- [ ] [Patterns.h](core_dll/src/utils/Patterns.h) - Add patterns
- [ ] [MEMORY_MAP.md](docs/MEMORY_MAP.md) - Document findings
- [ ] Save Cheat Engine table

---

## 🎓 Learning Resources

### Cheat Engine Tutorials

**Beginner**:
- Cheat Engine built-in tutorial (Help → Cheat Engine Tutorial)
- YouTube: "Cheat Engine Tutorial"

**Advanced**:
- [CE Wiki](https://wiki.cheatengine.org/)
- [Guided Hacking](https://guidedhacking.com/)

### Network Analysis

**Wireshark**:
- YouTube: "Wireshark Tutorial"
- Filter game traffic: `tcp.port == [JX1_PORT]`

---

## 💡 Tips for Success

### General Tips

1. **Save frequently**: Save CE table mỗi 15 phút
2. **Take notes**: Ghi chú vào MEMORY_MAP.md ngay
3. **Take breaks**: Nghỉ 10 phút sau mỗi giờ
4. **Be patient**: Reverse engineering cần patience

### Cheat Engine Tips

1. **Start với known values**: HP, Level, Gold dễ tìm nhất
2. **Use float cho positions**: Position values là float
3. **Use unknown initial value**: Cho counts và dynamic values
4. **Pointer scan**: Để tìm stable pointers
5. **Find what writes**: Để discover functions

### Common Mistakes

❌ **Mistake**: Quên save CE table
✅ **Fix**: Save sau mỗi discovery

❌ **Mistake**: Không test pointer sau restart
✅ **Fix**: ALWAYS restart game và test lại

❌ **Mistake**: Pattern quá ngắn (không unique)
✅ **Fix**: Dùng 10-15 bytes, test sau restart

---

## 📞 Nếu Gặp Vấn Đề

### Issue: Tìm quá nhiều results (1000+)

**Solutions**:
1. Change value thêm lần nữa → Next Scan
2. Dùng scan type khác (float thay vì 4 bytes)
3. Scan cả HP VÀ MP cùng lúc (group scan)

### Issue: Address thay đổi sau restart

**Solution**: Đây là normal! Dùng pointer scan để tìm stable pointer.

### Issue: Không tìm được function

**Solutions**:
1. Dùng "Find what writes" trên value mà function modify
2. Search string trong game ("You attacked!")
3. Try x64dbg nếu CE không đủ

### Issue: Pattern không work sau restart

**Solutions**:
1. Pattern có thể include dynamic values
2. Add more wildcards
3. Make pattern longer (15-20 bytes)

---

## 🎯 Success Criteria

Phase 2 thành công khi:

✅ **Found all offsets**:
- Player structure (HP, MP, Position, Level, Gold, Name)
- Entity structure (ID, Type, HP, Position, Name)
- Player Base Pointer (stable)
- Entity Manager pointer

✅ **Found all functions**:
- Attack, MoveTo, UseSkill
- UseItem, PickupItem, SendChat
- Patterns created cho tất cả

✅ **Updated code files**:
- Offsets.h với actual values
- Patterns.h với actual patterns
- MEMORY_MAP.md hoàn chỉnh

✅ **Verified everything**:
- Test patterns sau restart game
- All patterns work ✅

---

## 📚 Your Complete Phase 2 Package

Bạn đã có **7 tài liệu** để guide Phase 2:

### 1. Start Guide (File này)
**PHASE2_START_HERE.md** ← Bạn đang đọc
- Overview
- Quick start
- Checklist

### 2. Main Execution Guide ⭐ QUAN TRỌNG NHẤT
**[PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md)** (900+ dòng)
- Step-by-step với checkboxes
- 4 sessions chi tiết
- Follow đúng thứ tự

### 3. Quick Reference
**[PHASE2_QUICK_REFERENCE.md](docs/PHASE2_QUICK_REFERENCE.md)** (350+ dòng)
- Cheat Engine commands
- Pattern creation guide
- Troubleshooting

### 4. Full Tutorial
**[PHASE2_REVERSE_ENGINEERING.md](docs/PHASE2_REVERSE_ENGINEERING.md)** (400+ dòng)
- Theory và concepts
- Detailed explanations
- Examples

### 5. Documentation Template
**[MEMORY_MAP.md](docs/MEMORY_MAP.md)** (400+ dòng)
- Template để ghi findings
- Structured format

### 6. Index
**[PHASE2_INDEX.md](docs/PHASE2_INDEX.md)**
- Navigation guide
- Quick links

### 7. Overview
**[PHASE2_READY.md](PHASE2_READY.md)**
- Phase 2 overview
- What to expect

---

## 🎮 Exact Steps to Start

### Right Now - Do This:

**1. Read Overview** (10 phút):
```bash
Mở: PHASE2_READY.md
Đọc: Overview section
```

**2. Read Execution Guide Introduction** (10 phút):
```bash
Mở: docs/PHASE2_EXECUTION_GUIDE.md
Đọc: Sections "Before You Start" và "Session 1 intro"
```

**3. Install Cheat Engine** (5 phút):
```
Download: https://www.cheatengine.org/downloads.php
Install: Cheat Engine 7.5
```

**4. Print or Open Reference** (2 phút):
```bash
Mở: docs/PHASE2_QUICK_REFERENCE.md
Keep trong tab khác
```

**5. Start JX1** (2 phút):
```
Launch JX1
Login to character
Idle ở safe area
```

**6. Open Cheat Engine** (2 phút):
```
Launch Cheat Engine
Click "Open Process"
Select "JX1.exe"
Load tools/JX1_Template.CT
```

**7. Begin Session 1** (2-3 giờ):
```bash
Follow: docs/PHASE2_EXECUTION_GUIDE.md
Section: "Session 1: Player Structure Discovery"
Check boxes as you go
```

---

## 📋 Session Planning

### Nếu Bạn Có 1 Ngày (8-12 giờ):

**9:00 AM - 12:00 PM**: Session 1 (Player Structure)
- Break: 15 phút

**12:00 PM - 1:00 PM**: Lunch

**1:00 PM - 4:00 PM**: Session 2 (Entity Manager)
- Break: 15 phút

**4:00 PM - 7:00 PM**: Session 3 (Functions)
- Break: 15 phút

**7:00 PM - 9:00 PM**: Session 4 (Patterns & Verification)

**Done!** ✅

### Nếu Bạn Chia Nhiều Ngày:

**Day 1**: Session 1 (Player) - 2-3 giờ
**Day 2**: Session 2 (Entities) - 2-3 giờ
**Day 3**: Session 3 (Functions) - 2-3 giờ
**Day 4**: Session 4 (Patterns) - 2-3 giờ

---

## 🎯 After Session 1 - Quick Check

Sau Session 1, bạn nên có:

```cpp
// In Offsets.h
namespace Offsets {
    constexpr uintptr_t PLAYER_BASE = 0x12345678;  // ← Found!

    struct Player {
        static constexpr int HP = 0x10;      // ← Found!
        static constexpr int MAX_HP = 0x14;  // ← Found!
        static constexpr int MP = 0x18;      // ← Found!
        static constexpr int MAX_MP = 0x1C;  // ← Found!
        static constexpr int POS_X = 0x20;   // ← Found!
        static constexpr int POS_Y = 0x24;   // ← Found!
        static constexpr int POS_Z = 0x28;   // ← Found!
        static constexpr int LEVEL = 0x2C;   // ← Found!
        static constexpr int GOLD = 0x30;    // ← Found!
        // ...
    };
}
```

**Verify**: Close game, restart, load CE table → Pointer vẫn show đúng HP? ✅

---

## 🚨 Critical Success Factors

### 1. Pointer Scan is ESSENTIAL

Nếu không có stable pointer:
- ❌ Mỗi lần restart game phải tìm lại address
- ❌ Patterns sẽ không work
- ❌ DLL sẽ không tìm được player

Với stable pointer:
- ✅ Restart game → Pattern tìm lại được
- ✅ DLL work mọi lúc
- ✅ Production ready

### 2. Test After Restart

**ALWAYS test**:
```
1. Save CE table
2. Close game
3. Close CE
4. Restart game
5. Restart CE
6. Load table
7. Check: Pointers vẫn work? ✅
```

Nếu không work → Pointer không stable → Phải tìm lại!

### 3. Document Everything

Ngay khi tìm được:
```
HP offset: 0x10
  ↓
Ghi vào MEMORY_MAP.md NGAY
  ↓
Update Offsets.h NGAY
  ↓
Save CE table NGAY
```

Đừng để end of session mới document!

---

## 📊 Estimated Timeline

| Task | Time | Difficulty |
|------|------|------------|
| **Session 1**: Player Structure | 2-3h | ⭐⭐ Medium |
| **Session 2**: Entity Manager | 2-3h | ⭐⭐⭐ Hard |
| **Session 3**: Functions | 2-3h | ⭐⭐⭐⭐ Very Hard |
| **Session 4**: Patterns | 1-2h | ⭐⭐ Medium |
| **Total** | **8-12h** | **⭐⭐⭐ Hard** |

**Success Rate**: 95% nếu follow guide carefully

---

## 🎁 What You Get After Phase 2

### Immediate Benefits

**12,090 dòng code sẽ HOẠT ĐỘNG**:
- ✅ Player/Entity reading works
- ✅ ESP shows real entities
- ✅ Functions can be called
- ✅ Bots can run
- ✅ Network packets can be sent

**Test Ngay**:
```cpp
// Inject DLL
// In-game console shows:
[Player] Player initialized at 0x12345678
[Player] Name: YourCharName
[Player] HP: 850 / 1000 (85.0%)
[Player] Position: (1234.5, 5678.9, 100.0)

[EntityManager] Total Entities: 15
  Monsters: 5
  Players: 2
  NPCs: 3
  Items: 5

[CombatBot] Starting combat bot...
[CombatBot] Target acquired: Monster_01
[CombatBot] Attack called on entity ID 12345
```

**ESP Overlay** sẽ hiển thị:
- Red boxes quanh monsters
- Cyan boxes quanh players
- Yellow boxes quanh NPCs
- Names, HP bars, distances

**Bots hoạt động**:
- CombatBot tự động tấn công
- FarmingBot follow waypoints
- LootBot nhặt items
- QuestBot complete quests

---

## 🎯 Your Mission

**Goal**: Complete Phase 2 execution

**Tools Ready**: ✅ (7 documents, CE template)

**Time Needed**: 8-12 giờ

**Next Action**:
1. Open [PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md)
2. Follow Session 1
3. Check boxes as you complete steps

---

## 🚀 START HERE!

**📖 Open this file now**:
### [docs/PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md)

**Section to start**: "Session 1: Player Structure Discovery"

**Check boxes as you complete each step!**

---

**Good luck!** 🍀

Bạn đã có tất cả tools và documentation cần thiết. Phase 2 execution là straightforward nếu follow guide carefully!

**Remember**:
- ✅ Save frequently
- ✅ Test pointers after restart
- ✅ Document findings immediately
- ✅ Take breaks when needed

**Result**: Unlock 12,090 dòng code! 🎉

---

**Start Time**: ____________
**Session 1 Complete**: ____________
**Session 2 Complete**: ____________
**Session 3 Complete**: ____________
**Session 4 Complete**: ____________
**Phase 2 Complete**: ____________ 🏆
