# Cheat Engine - Hướng Dẫn Chi Tiết Cho JX1 Auto

**Mục đích**: Tìm memory addresses, offsets, và patterns để implement vào JX1AutoCore.dll

---

## 📋 Checklist - Những Thứ Cần Tìm

### 🎯 Phase 2A: Player Data
- [ ] Player Base Address/Pointer
- [ ] Player HP (Current/Max)
- [ ] Player MP (Current/Max)
- [ ] Player Position (X, Y, Z)
- [ ] Player Level
- [ ] Player Name
- [ ] Player Gold

### 🎯 Phase 2B: Entity/Monster Data
- [ ] Entity List Base Address
- [ ] Entity Count
- [ ] Entity HP
- [ ] Entity Position
- [ ] Entity Type/ID
- [ ] Entity Name

### 🎯 Phase 2C: Functions
- [ ] Move Function
- [ ] Attack Function
- [ ] Use Item Function
- [ ] Pick Item Function
- [ ] Talk to NPC Function

---

## 🔧 Cài Đặt Cheat Engine

### Tải & Cài Đặt
```
1. Download: https://www.cheatengine.org/downloads.php
2. Version khuyến nghị: 7.5+
3. Cài đặt với settings mặc định
```

### Cấu Hình Khuyến Nghị
```
Settings → General Settings:
✓ Show Advanced Options
✓ Enable Speedhack
✗ Load system module list (tắt để tăng tốc)

Settings → Scan Settings:
✓ MEM_PRIVATE
✓ MEM_IMAGE
✓ MEM_MAPPED
```

---

## 📖 Tutorial 1: Tìm Player HP

### Bước 1: Attach Process
```
1. Chạy game JX1
2. Mở Cheat Engine
3. Click icon 💻 (góc trái trên cùng)
4. Tìm process: JX1.exe hoặc JX1Client.exe
5. Click [Open]
```

### Bước 2: First Scan
```
Giả sử trong game: HP = 2500/3000

Trong Cheat Engine:
┌─────────────────────────────┐
│ Value Type: 4 Bytes         │
│ Scan Type:  Exact Value     │
│ Value:      2500            │
│                             │
│ [First Scan]                │
└─────────────────────────────┘

Kết quả: ~50,000 addresses (quá nhiều)
```

### Bước 3: Next Scan (Lọc)
```
Làm HP thay đổi trong game:
- Bị đánh → HP = 2200
- Hoặc uống thuốc → HP = 2800

Trong Cheat Engine:
┌─────────────────────────────┐
│ Value: 2200                 │
│ [Next Scan]                 │
└─────────────────────────────┘

Kết quả: ~500 addresses
```

### Bước 4: Lặp Lại
```
Tiếp tục làm HP thay đổi và Next Scan
Cho đến khi: < 10 addresses
```

### Bước 5: Xác Định Address Đúng
```
1. Freeze (đóng băng) từng address
2. Xem HP trong game có đóng băng không?
3. Thử change value → xem HP trong game thay đổi không?
4. Tìm được address đúng → Double-click để add vào list
```

**Ví dụ kết quả:**
```
Address: 0x1A2B3C4D
Value: 2500
Type: 4 Bytes
Description: Player HP Current
```

---

## 📖 Tutorial 2: Tìm Player Pointer Chain

**Tại sao cần pointer?**
- Address trực tiếp (0x1A2B3C4D) thay đổi mỗi lần chạy game
- Pointer chain ổn định, không đổi

### Bước 1: Tìm Address Hiện Tại
```
Làm theo Tutorial 1 → Tìm được HP address
Ví dụ: 0x1A2B3C4D
```

### Bước 2: Pointer Scan
```
1. Right-click address trong Address List
2. Chọn "Pointer scan for this address"
3. Dialog hiện ra:
   ┌────────────────────────────────────┐
   │ Save scandata to:                  │
   │ C:\Temp\jx1_hp_pointer.scandata    │
   │                                    │
   │ Max level: 5                       │
   │ Max different offsets per node: 3  │
   │ ☑ Only find paths with...          │
   │   Static addresses                 │
   │                                    │
   │ [OK]                               │
   └────────────────────────────────────┘
4. Click OK → Đợi scan (1-5 phút)
```

**Kết quả ban đầu:**
```
~100,000 pointers (quá nhiều)
```

### Bước 3: Rescan để Lọc Pointer

**Lần 1:**
```
1. Đóng game hoàn toàn
2. Mở lại game
3. Tìm lại HP address (sẽ khác address lần trước)
   Ví dụ: 0x7F8E9D6C (khác với 0x1A2B3C4D)
4. Trong Pointer Scanner window:
   Menu → Pointer scanner → Rescan memory
5. Nhập address mới: 7F8E9D6C
6. Click OK
```

**Kết quả sau lần 1:** ~5,000 pointers

**Lần 2-3:** Lặp lại như trên
**Kết quả cuối:** < 50 pointers

### Bước 4: Chọn Pointer Tốt Nhất

**Tiêu chí:**
```
✓ Offset đơn giản (ít level)
✓ Base address từ module (JX1.exe+...)
✗ Tránh pointer có base quá lớn
✗ Tránh pointer có quá nhiều level (>5)
```

**Ví dụ pointer tốt:**
```
Address              Offsets
"JX1.exe"+00A2B4C0 → 0x14 → 0x28 → 0x10

Giải thích:
- Base: Module JX1.exe + offset 0x00A2B4C0
- Level 1: Read pointer tại base, cộng 0x14
- Level 2: Read pointer tại (level1 + 0x14), cộng 0x28
- Level 3: Read int tại (level2 + 0x28) + 0x10 = HP value
```

### Bước 5: Test Pointer

```
1. Double-click vào pointer để add vào Address List
2. Đóng game, mở lại
3. Xem pointer có tự động cập nhật đúng giá trị không?
4. Nếu đúng → Pointer hợp lệ ✓
```

---

## 📖 Tutorial 3: Tìm Player Struct (Nâng Cao)

**Mục tiêu:** Tìm toàn bộ struct Player thay vì từng field riêng lẻ

### Bước 1: Tìm Nhiều Fields
```
Tìm theo Tutorial 1:
- HP Current:  0x1A2B3C10
- HP Max:      0x1A2B3C14  (+ 0x04)
- MP Current:  0x1A2B3C18  (+ 0x08)
- MP Max:      0x1A2B3C1C  (+ 0x0C)
- Level:       0x1A2B3C20  (+ 0x10)
```

### Bước 2: Phân Tích Struct

```
Nhận xét:
- Các field cách nhau 4 bytes
- Có vẻ là một struct liên tục

Giả thuyết Struct:
struct Player {
    // ...
    int32_t hpCurrent;   // +0x10
    int32_t hpMax;       // +0x14
    int32_t mpCurrent;   // +0x18
    int32_t mpMax;       // +0x1C
    int32_t level;       // +0x20
    // ...
};
```

### Bước 3: Tìm Base Pointer
```
1. Pointer scan cho HP Current
2. Tìm pointer chain:
   "JX1.exe"+00A2B4C0 → 0x14 → 0x28

→ Địa chỉ tại ("JX1.exe"+00A2B4C0 → 0x14 → 0x28)
  chính là base của Player struct

Player* player = (Player*)(ptrChain);
```

### Bước 4: Tìm Thêm Fields

**Tìm Player Position:**
```
1. Note lại vị trí hiện tại trong game
2. Dùng Unknown Initial Value scan
3. Di chuyển trong game
4. Scan "Changed Value"
5. Đứng yên
6. Scan "Unchanged Value"
7. Di chuyển lại
8. Scan "Changed Value"
9. Lặp lại → Tìm được X, Y, Z coordinates

Thường là Float type:
- X: 1234.56
- Y: 789.12
- Z: 234.78
```

### Bước 5: Xây Dựng Struct Hoàn Chỉnh

**Trong file Offsets.h:**
```cpp
namespace Offsets {
    namespace Player {
        constexpr uintptr_t BASE_PTR = 0x00A2B4C0;  // JX1.exe + offset
        constexpr int PTR_OFFSET_1 = 0x14;
        constexpr int PTR_OFFSET_2 = 0x28;

        // Player struct offsets
        constexpr int HP_CURRENT = 0x10;
        constexpr int HP_MAX = 0x14;
        constexpr int MP_CURRENT = 0x18;
        constexpr int MP_MAX = 0x1C;
        constexpr int LEVEL = 0x20;
        constexpr int POSITION_X = 0x40;
        constexpr int POSITION_Y = 0x44;
        constexpr int POSITION_Z = 0x48;
        constexpr int NAME = 0x60;  // char[32]
    }
}
```

---

## 📖 Tutorial 4: Tìm Functions (Pattern Scanning)

**Mục tiêu:** Tìm function game gọi (VD: hàm gây damage)

### Bước 1: Find What Writes
```
1. Tìm HP address
2. Right-click → "Find out what writes to this address"
3. Dialog "The following opcodes write to..." hiện ra
4. Quay game, bị đánh (HP giảm)
5. Cheat Engine sẽ show instruction ghi vào HP
```

**Ví dụ kết quả:**
```
JX1.exe+12A4B5 - 29 41 10  - SUB [ECX+10],EAX
```

### Bước 2: Xem Disassembly
```
1. Click vào instruction
2. Click [Show disassembler]
3. Xem code xung quanh:

JX1.exe+12A4A0 - 55           - PUSH EBP           ← Start of function
JX1.exe+12A4A1 - 8B EC        - MOV EBP,ESP
JX1.exe+12A4A3 - 83 EC 08     - SUB ESP,08
JX1.exe+12A4A6 - 8B 45 08     - MOV EAX,[EBP+08]
JX1.exe+12A4A9 - 89 45 FC     - MOV [EBP-04],EAX
...
JX1.exe+12A4B5 - 29 41 10     - SUB [ECX+10],EAX   ← Our instruction
JX1.exe+12A4B8 - C3           - RET                ← End of function
```

### Bước 3: Tạo Pattern

**Chọn đoạn code độc nhất:**
```
Tránh:
✗ PUSH EBP / MOV EBP,ESP (quá phổ biến)
✗ NOP / INT 3 (padding)

Chọn:
✓ Sequence độc nhất 8-16 bytes
✓ Bao gồm instruction quan trọng

Ví dụ pattern:
8B 45 08 89 45 FC ... 29 41 10
```

**Wildcards cho giá trị động:**
```
Nếu có:
MOV EAX,[12345678]  → 8B 05 78 56 34 12

Dùng wildcards:
8B 05 ?? ?? ?? ??

Trong code:
"8B 05 ? ? ? ?"
```

### Bước 4: Implement trong Code

**File Patterns.h:**
```cpp
namespace Patterns {
    constexpr const char* DAMAGE_FUNCTION =
        "8B 45 08 89 45 FC ? ? ? ? 29 41 10";
}
```

**Sử dụng trong MemoryManager:**
```cpp
uintptr_t damageFuncAddr = MemoryManager::FindPattern(
    "JX1.exe",
    Patterns::DAMAGE_FUNCTION
);

if (damageFuncAddr) {
    // Hook hoặc call function này
}
```

---

## 📖 Tutorial 5: Tìm Entity List

**Mục tiêu:** Tìm list tất cả entities (monsters, NPCs, items)

### Bước 1: Tìm Monster HP
```
1. Target một con monster
2. Quan sát HP của monster
3. Scan giá trị HP (giống Player HP)
4. Đánh monster để HP thay đổi
5. Next Scan
6. Tìm được monster HP address
```

### Bước 2: Tìm Entity Count
```
Giả thuyết: Entity list là array

1. Đếm số lượng entities gần bạn
   Ví dụ: 5 monsters + 2 NPCs = 7 entities
2. Scan value = 7 (4 Bytes)
3. Di chuyển đến nơi khác
4. Đếm lại → 3 entities
5. Scan value = 3
6. Lặp lại → Tìm được entity count
```

### Bước 3: Tìm Entity Array Base
```
1. Tìm monster HP: 0x2A3B4C10
2. Pointer scan cho address này
3. Tìm pointer như:
   "JX1.exe"+00BCDEFF → 0x08 → (index * size)

4. Test với nhiều monsters
5. Xác định entity size
```

**Ví dụ:**
```
Entity 0: Base + (0 * 0x200) = 0x2A3B4C00
Entity 1: Base + (1 * 0x200) = 0x2A3B4E00
Entity 2: Base + (2 * 0x200) = 0x2A3B5000

→ Entity size = 0x200 (512 bytes)
```

### Bước 4: Document

**Offsets.h:**
```cpp
namespace Offsets {
    namespace EntityManager {
        constexpr uintptr_t BASE_PTR = 0x00BCDEFF;
        constexpr int PTR_OFFSET = 0x08;
        constexpr int ENTITY_COUNT = 0x10;
        constexpr int ENTITY_SIZE = 0x200;
    }

    namespace Entity {
        constexpr int HP_CURRENT = 0x10;
        constexpr int HP_MAX = 0x14;
        constexpr int POSITION_X = 0x40;
        constexpr int POSITION_Y = 0x44;
        constexpr int POSITION_Z = 0x48;
        constexpr int TYPE = 0x08;  // 1=Monster, 2=NPC, 3=Item
        constexpr int ID = 0x0C;
    }
}
```

---

## 🛠️ Template Code Usage

### Reading Player Data
```cpp
uintptr_t GetPlayerBase() {
    uintptr_t moduleBase = MemoryManager::GetModuleBase("JX1.exe");
    uintptr_t ptr = MemoryManager::Read<uintptr_t>(
        moduleBase + Offsets::Player::BASE_PTR
    );
    ptr = MemoryManager::Read<uintptr_t>(ptr + Offsets::Player::PTR_OFFSET_1);
    ptr = MemoryManager::Read<uintptr_t>(ptr + Offsets::Player::PTR_OFFSET_2);
    return ptr;
}

int GetPlayerHP() {
    uintptr_t playerBase = GetPlayerBase();
    return MemoryManager::Read<int>(playerBase + Offsets::Player::HP_CURRENT);
}

Vector3 GetPlayerPosition() {
    uintptr_t playerBase = GetPlayerBase();
    Vector3 pos;
    pos.x = MemoryManager::Read<float>(playerBase + Offsets::Player::POSITION_X);
    pos.y = MemoryManager::Read<float>(playerBase + Offsets::Player::POSITION_Y);
    pos.z = MemoryManager::Read<float>(playerBase + Offsets::Player::POSITION_Z);
    return pos;
}
```

### Finding Pattern
```cpp
uintptr_t attackFunc = MemoryManager::FindPattern(
    "JX1.exe",
    Patterns::ATTACK_FUNCTION
);

if (attackFunc) {
    Logger::Info("Found Attack Function at: 0x%p", (void*)attackFunc);
    // Hook hoặc gọi function
}
```

---

## 📝 Checklist Khi Tìm Offsets

### ✅ Verification Steps
```
□ Test pointer trên nhiều lần restart game
□ Test trên các character khác nhau
□ Document offset trong Offsets.h
□ Document pattern trong Patterns.h
□ Implement read function trong Player.cpp/Entity.cpp
□ Test trong game thực tế
```

### ✅ Documentation Format
```cpp
// Offsets.h
namespace Offsets {
    namespace Player {
        // Found: 2025-10-15
        // Method: Pointer scan from HP
        // Stable: ✓ (tested 10 restarts)
        constexpr uintptr_t BASE_PTR = 0x00A2B4C0;

        // Struct offsets
        constexpr int HP_CURRENT = 0x10;  // int32
        constexpr int HP_MAX = 0x14;      // int32
    }
}
```

---

## 🔍 Debugging Tips

### Khi Pointer Không Hoạt Động
```
1. Kiểm tra game architecture (x86 vs x64)
2. Kiểm tra ASLR (Address Space Layout Randomization)
3. Try pointer scan lại với settings khác:
   - Tăng max level lên 7
   - Tăng max offsets lên 5
4. Check module base có thay đổi không
```

### Khi Pattern Không Tìm Thấy
```
1. Kiểm tra pattern có chính xác không
2. Thử pattern ngắn hơn hoặc dài hơn
3. Thêm wildcards cho giá trị dynamic
4. Scan trong module khác (có thể code nằm trong DLL)
```

---

## 🎯 Next Steps

Sau khi hoàn thành Phase 2:
1. Update [Offsets.h](core_dll/src/utils/Offsets.h)
2. Update [Patterns.h](core_dll/src/utils/Patterns.h)
3. Implement reading trong [Player.cpp](core_dll/src/game/Player.cpp)
4. Implement reading trong [Entity.cpp](core_dll/src/game/Entity.cpp)
5. Test với overlay để hiển thị dữ liệu real-time
6. Move to Phase 3: Bot Implementation

---

## 📚 Additional Resources

- Cheat Engine Tutorial: Help → Tutorial trong Cheat Engine
- Guided CE Tutorial: Hoàn thành 9 levels để thành thạo
- Community: https://fearlessrevolution.com/
- Patterns: https://guidedhacking.com/

---

**Last Updated:** 2025-10-15
**Author:** JX1 Auto Development Team
**Status:** Ready for Phase 2 - Reverse Engineering
