# Sau Khi Scan - Hướng Dẫn Cập Nhật Config

## 📋 Tổng Quan

Sau khi scan memory bằng [Auto Scanner](../../tools/AUTO_SCAN_README.md), bạn cần cập nhật các file config để bot có thể sử dụng các địa chỉ vừa tìm được.

## 🎯 Các File Cần Cập Nhật

| File | Mục đích | Khi nào update |
|------|----------|----------------|
| [Offsets.h](../../core_dll/src/utils/Offsets.h) | Lưu offset cố định | Sau khi tìm được offset trong structure |
| [Patterns.h](../../core_dll/src/utils/Patterns.h) | Pattern để tìm địa chỉ động | Khi địa chỉ thay đổi mỗi lần chạy |
| [game_info.json](../../config/game_info.json) | Thông tin game | Cập nhật process name, version |

## 📝 Workflow Chuẩn

```
┌─────────────────────┐
│  1. Scan Memory     │
│  (AutoScanMemory)   │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│  2. Analyze Results │
│  (Cheat Engine)     │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│  3. Find Pattern    │
│  (Is it Static?)    │
└──────────┬──────────┘
           │
     ┌─────┴─────┐
     │           │
     ▼           ▼
  Static?     Dynamic?
     │           │
     ▼           ▼
  Offsets.h  Patterns.h
     │           │
     └─────┬─────┘
           │
           ▼
┌─────────────────────┐
│  4. Test & Verify   │
└─────────────────────┘
```

---

## 1️⃣ Cập Nhật Offsets.h

### Khi Nào Sử Dụng?

- Địa chỉ là **static** (không đổi mỗi lần restart game)
- Hoặc bạn đã tìm được **base pointer + offset**

### Ví Dụ Cụ Thể

#### Tìm HP Offset

**Bước 1: Scan tìm địa chỉ HP**
```
Run AutoScanMemory.ps1
-> Tìm được địa chỉ: 0x12AB5678
```

**Bước 2: Tìm base pointer trong Cheat Engine**
```
1. Add address 0x12AB5678 to list
2. Right-click -> Pointer scan for this address
3. Tìm được: "JX1.exe"+0x00A3B000 -> [+0x10] -> [+0x4] = HP address
```

**Bước 3: Cập nhật Offsets.h**
```cpp
// File: core_dll/src/utils/Offsets.h

namespace JX1 {
namespace Offsets {

    // ✅ UPDATE: Player base pointer (static address)
    constexpr uintptr_t PLAYER_BASE = 0x00A3B000;  // ← Địa chỉ tìm được

    struct Player {
        // ✅ UPDATE: HP offset
        static constexpr int HP = 0x10;          // ← Offset từ player base
        static constexpr int MAX_HP = 0x14;      // ← Thường ngay sau HP (tìm thêm)

        static constexpr int MP = 0x18;          // ← TODO: Tìm tiếp
        static constexpr int MAX_MP = 0x1C;      // ← TODO: Tìm tiếp

        // Position (thường ở cuối structure)
        static constexpr int POS_X = 0x40;       // ← TODO: Scan coordinate X
        static constexpr int POS_Y = 0x44;       // ← TODO: Float, ngay sau X
        static constexpr int POS_Z = 0x48;       // ← TODO: Float, ngay sau Y

        // Character Info
        static constexpr int LEVEL = 0x30;       // ← TODO: Tìm level
        static constexpr int EXPERIENCE = 0x34;  // ← TODO: Tìm exp
        static constexpr int GOLD = 0x50;        // ← TODO: Tìm gold
    };

} // namespace Offsets
} // namespace JX1
```

**Bước 4: Comment ghi chú (quan trọng!)**
```cpp
/**
 * Player Base Pointer
 * Found at: "JX1.exe"+0x00A3B000
 * Method: Pointer scan from HP address 0x12AB5678
 * Game Version: 1.0.0.123
 * Last Updated: 2025-01-15
 * Verified: ✅ Stable across restarts
 */
constexpr uintptr_t PLAYER_BASE = 0x00A3B000;

struct Player {
    /**
     * HP Offset
     * Type: int32 (4 bytes)
     * Range: 0 - MAX_HP
     * Found at: PlayerBase + 0x10
     * Verified: ✅ Working
     */
    static constexpr int HP = 0x10;

    /**
     * Max HP Offset
     * Type: int32 (4 bytes)
     * Found at: PlayerBase + 0x14 (right after HP)
     * Verified: ✅ Working
     */
    static constexpr int MAX_HP = 0x14;
};
```

---

## 2️⃣ Cập Nhật Patterns.h

### Khi Nào Sử Dụng?

- Địa chỉ **thay đổi** mỗi lần restart game
- Cần tìm động (dynamic pattern scan)

### Cách Tạo Pattern

**Bước 1: Tìm code accessing địa chỉ**
```
1. Trong Cheat Engine, right-click địa chỉ HP
2. Chọn "Find out what accesses this address"
3. Thực hiện action (bị đánh, uống thuốc)
4. Sẽ thấy code như:

   mov eax,[ecx+10]    <- Code đọc HP
   Address: 0x00401234
```

**Bước 2: View assembly trong Cheat Engine/x64dbg**
```asm
00401230: 55                 push ebp
00401231: 8B EC              mov ebp,esp
00401233: 56                 push esi
00401234: 8B F1              mov esi,ecx
00401236: 8B 46 10           mov eax,[esi+10]    <- This reads HP!
00401239: 85 C0              test eax,eax
0040123B: 74 05              je 00401242
```

**Bước 3: Chọn unique byte sequence**
```
Chọn 10-15 bytes xung quanh instruction quan trọng:
55 8B EC 56 8B F1 8B 46 10 85 C0 74 05

Lưu ý: Bỏ qua những byte có thể thay đổi (addresses, offsets)
```

**Bước 4: Tạo pattern với wildcards**
```cpp
// File: core_dll/src/utils/Patterns.h

namespace JX1 {
namespace Patterns {

    /**
     * HP Read Function Pattern
     *
     * Assembly:
     *   55                 push ebp
     *   8B EC              mov ebp,esp
     *   56                 push esi
     *   8B F1              mov esi,ecx
     *   8B 46 10           mov eax,[esi+10]  <- Reads HP at offset 0x10
     *   85 C0              test eax,eax
     *   74 05              je short
     *
     * Pattern matches the unique instruction sequence
     * Offset 0x10 is where HP is located (see mov eax,[esi+10])
     *
     * Found at: JX1.exe + 0x00401230
     * Game Version: 1.0.0.123
     * Last Verified: 2025-01-15
     */
    constexpr const char* HP_READ_FUNCTION =
        "\x55\x8B\xEC\x56\x8B\xF1\x8B\x46\x10\x85\xC0\x74\x05";

    constexpr const char* HP_READ_FUNCTION_MASK =
        "xxxxxxxxxxxxx";  // All bytes are exact match

    /**
     * Player Base Pointer Pattern
     *
     * Assembly:
     *   8B 0D ?? ?? ?? ??  mov ecx,[JX1.exe+XXXXXX]  <- This is PLAYER_BASE
     *   85 C9              test ecx,ecx
     *   74 ??              je short
     *
     * The ?? ?? ?? ?? will be extracted as the player base address
     *
     * Example: 8B 0D A0 B3 4A 00
     *          -> Player base = 0x004AB3A0
     */
    constexpr const char* PLAYER_BASE =
        "\x8B\x0D\x00\x00\x00\x00\x85\xC9\x74";
        //      ^^^^^^^^^^^^^ These will be wildcards

    constexpr const char* PLAYER_BASE_MASK =
        "xx????xxx";
        //  ^^^^ Wildcards for address bytes

    /**
     * Attack Function Pattern
     * TODO: Find this pattern by:
     * 1. Find attack button handler
     * 2. Set breakpoint when clicking attack
     * 3. Step through to find actual attack function
     * 4. Copy byte pattern
     */
    constexpr const char* ATTACK_FUNCTION = "";  // TODO
    constexpr const char* ATTACK_FUNCTION_MASK = "";  // TODO

} // namespace Patterns
} // namespace JX1
```

### Pattern Syntax

```cpp
// Exact bytes - use when value is always the same
"\x55\x8B\xEC"         // push ebp; mov ebp,esp
"xxxxx"                // All exact

// With wildcards - use for dynamic values (addresses, offsets)
"\x8B\x0D\x00\x00\x00\x00"  // mov ecx,[address]
"xx????"                     // Address is wildcard

// Alternative wildcard notation (some pattern scanners)
"8B 0D ?? ?? ?? ??"          // Same as above (hex string format)
```

---

## 3️⃣ Cập Nhật game_info.json

### Thông Tin Game

```json
{
  "game": {
    "name": "Võ Lâm Truyền Kỳ 1",
    "short_name": "JX1",
    "version": "1.0.0.123",           // ← UPDATE: Game version tìm được
    "publisher": "VNG",
    "process_name": "JX1.exe",        // ← UPDATE: Nếu khác
    "window_class": "JX1Window",
    "directx_version": 9,
    "architecture": "x86"
  },

  "detection": {
    "process_names": [
      "JX1.exe",
      "JX1Client.exe",                // ← ADD: Nếu có tên khác
      "Sword.exe"
    ],
    "window_titles": [
      "Võ Lâm Truyền Kỳ",
      "JX1",
      "Sword"
    ]
  },

  "notes": [
    "DirectX 9 confirmed",
    "32-bit application",
    "Player base found at JX1.exe+0x00A3B000",  // ← ADD: Ghi chú findings
    "HP offset: +0x10 from player base",
    "Last updated: 2025-01-15"
  ]
}
```

---

## 📊 Ví Dụ Thực Tế: Tìm Toàn Bộ Player Structure

### Workflow

```
1. Scan HP         -> 0x12AB5678
2. Find pointer    -> [JX1.exe+0xA3B000]+0x10
3. Analyze nearby  -> Tìm các offset khác xung quanh
```

### Sử Dụng Cheat Engine

**Step 1: Add địa chỉ HP vào list**
```
Address: 0x12AB5678
Type: 4 Bytes
Value: 1500 (your current HP)
```

**Step 2: View memory region**
```
Right-click address -> Browse this memory region

Bạn sẽ thấy:
Offset  | Value (Hex)  | Value (Dec) | Guess
--------|--------------|-------------|--------
+0x00   | 00 00 00 01  | 1           | Player ID?
+0x04   | 64 00 00 00  | 100         | Level?
+0x08   | E8 03 00 00  | 1000        | Max MP?
+0x0C   | D0 02 00 00  | 720         | Current MP?
+0x10   | DC 05 00 00  | 1500        | ✅ Current HP (confirmed!)
+0x14   | 10 0E 00 00  | 3600        | ✅ Max HP (looks right!)
+0x18   | 40 42 0F 00  | 1000000.0   | Gold? (float?)
```

**Step 3: Verify từng offset**
```
1. Add +0x04 (Level) to list
2. Check if value matches your level
3. If yes -> Update Offsets.h
4. If no -> Try another interpretation
```

**Step 4: Cập nhật vào Offsets.h**
```cpp
struct Player {
    static constexpr int PLAYER_ID = 0x00;   // ✅ Verified
    static constexpr int LEVEL = 0x04;       // ✅ Verified: Matches in-game level
    static constexpr int MAX_MP = 0x08;      // ✅ Verified
    static constexpr int MP = 0x0C;          // ✅ Verified
    static constexpr int HP = 0x10;          // ✅ Verified: Matches scan result
    static constexpr int MAX_HP = 0x14;      // ✅ Verified
    static constexpr int GOLD = 0x18;        // ⚠️ TODO: Verify type (might be int, not float)
};
```

---

## 🔧 Testing & Verification

### Test Offsets

Tạo test function:

```cpp
// File: core_dll/src/test/TestOffsets.cpp

#include "../utils/Offsets.h"
#include "../memory/MemoryManager.h"
#include <iostream>

void TestPlayerOffsets() {
    auto& mem = MemoryManager::Get();

    // Get player base
    uintptr_t baseAddr = mem.GetModuleBase("JX1.exe");
    uintptr_t playerBase = mem.Read<uintptr_t>(baseAddr + JX1::Offsets::PLAYER_BASE);

    if (!playerBase) {
        std::cout << "❌ Player base is NULL!" << std::endl;
        return;
    }

    std::cout << "✅ Player base: 0x" << std::hex << playerBase << std::endl;

    // Test HP
    int hp = mem.Read<int>(playerBase + JX1::Offsets::Player::HP);
    int maxHp = mem.Read<int>(playerBase + JX1::Offsets::Player::MAX_HP);

    std::cout << "HP: " << std::dec << hp << " / " << maxHp << std::endl;

    if (hp > 0 && hp <= maxHp && maxHp > 0 && maxHp < 100000) {
        std::cout << "✅ HP offset looks correct!" << std::endl;
    } else {
        std::cout << "❌ HP offset might be wrong!" << std::endl;
    }

    // Test Level
    int level = mem.Read<int>(playerBase + JX1::Offsets::Player::LEVEL);
    std::cout << "Level: " << level << std::endl;

    if (level > 0 && level < 200) {
        std::cout << "✅ Level offset looks correct!" << std::endl;
    } else {
        std::cout << "❌ Level offset might be wrong!" << std::endl;
    }
}
```

### Test Patterns

```cpp
// File: core_dll/src/test/TestPatterns.cpp

#include "../utils/Patterns.h"
#include "../memory/MemoryManager.h"
#include <iostream>

void TestPatternScanning() {
    auto& mem = MemoryManager::Get();

    // Test player base pattern
    uintptr_t found = mem.FindPattern(
        JX1::Patterns::PLAYER_BASE,
        JX1::Patterns::PLAYER_BASE_MASK
    );

    if (found) {
        std::cout << "✅ Player base pattern found at: 0x"
                  << std::hex << found << std::endl;

        // Extract address from pattern
        // Pattern: 8B 0D [XX XX XX XX] 85 C9
        //               ^address here
        uintptr_t extractedAddr = mem.Read<uintptr_t>(found + 2);
        std::cout << "Extracted address: 0x" << std::hex << extractedAddr << std::endl;
    } else {
        std::cout << "❌ Player base pattern not found!" << std::endl;
        std::cout << "Possible reasons:" << std::endl;
        std::cout << "  - Game was updated" << std::endl;
        std::cout << "  - Pattern is incorrect" << std::endl;
        std::cout << "  - Need to rescan" << std::endl;
    }
}
```

---

## 📋 Checklist: Sau Khi Scan

Sau khi scan xong, check list này:

### ✅ Phase 1: Basic Values
- [ ] Tìm được HP address
- [ ] Tìm được Max HP address
- [ ] Tìm được MP address
- [ ] Tìm được Max MP address
- [ ] Update vào `Offsets.h`
- [ ] Test lại với test function
- [ ] Verify khi restart game (static hay dynamic?)

### ✅ Phase 2: Position & Character
- [ ] Tìm được Position X
- [ ] Tìm được Position Y
- [ ] Tìm được Position Z (nếu có)
- [ ] Tìm được Level
- [ ] Tìm được Experience
- [ ] Tìm được Gold
- [ ] Update vào `Offsets.h`

### ✅ Phase 3: Advanced
- [ ] Tìm được Player name
- [ ] Tìm được Entity list
- [ ] Tìm được Entity count
- [ ] Tìm được Inventory base
- [ ] Update vào `Offsets.h`

### ✅ Phase 4: Functions
- [ ] Tìm được Attack function
- [ ] Tìm được MoveTo function
- [ ] Tìm được UseSkill function
- [ ] Tạo patterns trong `Patterns.h`
- [ ] Test pattern scanning

### ✅ Phase 5: Documentation
- [ ] Update `game_info.json`
- [ ] Add comments vào offsets
- [ ] Add comments vào patterns
- [ ] Document game version
- [ ] Create changelog

---

## 🎯 Quick Reference

### Từ Scan Results -> Offsets.h

```
AutoScanMemory -> hp_addresses.txt:
0x12AB5678

Cheat Engine Pointer Scan:
[JX1.exe+0xA3B000] -> [+0x0] -> [+0x10] = 0x12AB5678

Update Offsets.h:
constexpr uintptr_t PLAYER_BASE = 0xA3B000;
struct Player {
    static constexpr int HP = 0x10;
};
```

### Từ Assembly -> Patterns.h

```
x64dbg:
00401230: 8B 0D A0 B3 4A 00    mov ecx,[004AB3A0]
00401236: 85 C9                test ecx,ecx
00401238: 74 05                je 0040123F

Update Patterns.h:
constexpr const char* PLAYER_BASE = "\x8B\x0D\x00\x00\x00\x00\x85\xC9\x74";
constexpr const char* PLAYER_BASE_MASK = "xx????xxx";
```

---

## 💡 Tips & Best Practices

1. **Always verify across game restarts**
   - Static offsets should remain the same
   - Dynamic addresses need patterns

2. **Document everything**
   - Add comments for every offset
   - Note the date and game version
   - Explain how you found it

3. **Test thoroughly**
   - Create test functions
   - Verify values make sense
   - Check edge cases (death, level up, etc.)

4. **Use version control**
   - Commit after finding each major offset
   - Tag stable versions
   - Keep backup of working configs

5. **Be patient**
   - Reverse engineering takes time
   - Not all values are easy to find
   - Some might be encrypted/obfuscated

---

## 🚀 Next Steps

After updating configs:

1. **Rebuild the project**
   ```batch
   cd scripts
   build.bat
   ```

2. **Test injection**
   ```batch
   injector.exe JX1.exe core_dll.dll
   ```

3. **Verify in-game**
   - Check overlay displays correct values
   - Test bot functions
   - Monitor for crashes

4. **Continue Phase 2**
   - Find more offsets
   - Document findings
   - Update code to use new offsets

---

**Good luck with reverse engineering! 🔍**
