# Centralized Config System - Complete Guide

## 🎯 Overview

Hệ thống config tập trung này giúp bạn:

- ✅ **Scan memory một lần** → **Auto-generate config** → **Dùng luôn trong code**
- ✅ **Một file duy nhất** chứa tất cả offsets, patterns, addresses
- ✅ **Không cần edit code C++** mỗi khi tìm được offset mới
- ✅ **Hot-reload** config khi đang develop
- ✅ **Validate** config trước khi build

## 📁 File Structure

```
jx1_v2.0_auto/
├── config/
│   └── memory_config.json          ← SINGLE SOURCE OF TRUTH
│
├── tools/
│   ├── AutoScanMemory_v2.ps1       ← Auto-scan & generate config
│   ├── validate_config.ps1         ← Validate config
│   └── auto_scan.bat               ← Easy launcher
│
└── core_dll/src/
    ├── config/
    │   ├── MemoryConfig.h          ← Config loader class
    │   ├── MemoryConfig.cpp        ← Implementation
    │   └── ConfigUsageExample.cpp  ← Usage examples
    │
    └── utils/
        ├── Offsets.h               ← (Deprecated - use MemoryConfig)
        └── Patterns.h              ← (Deprecated - use MemoryConfig)
```

---

## 🚀 Quick Start

### Step 1: Scan Memory

```batch
cd tools
auto_scan.bat
```

Hoặc:

```powershell
.\AutoScanMemory_v2.ps1
```

### Step 2: Scan HP (Example)

```
Menu:
1) Scan for HP (Auto mode)   ← Choose this

[Step 1] Enter your CURRENT HP value: 1500
Scanning... Found 5000 addresses

[Step 2] Change your HP, enter NEW value: 1200
Filtering... Remaining: 50 addresses

[Step 3] Change HP again: 1000
Remaining: 3 addresses

SUCCESS! Found HP address(es):
  0x12AB5678
  0x12AB5ABC
  0x12AB5CDE

Is the first address correct? (y/n): y

Config updated successfully!
```

### Step 3: Save Config

```
Menu:
7) Save Config   ← Choose this

Config saved to: ../config/memory_config.json
```

### Step 4: Validate Config

```powershell
.\validate_config.ps1
```

Output:
```
✅ All checks passed!
Configuration Readiness:
  Player Offsets: 4 / 17 verified (23.5%)

Recommendations:
  • Run AutoScanMemory_v2.ps1 to find missing offsets
```

### Step 5: Use in C++ Code

```cpp
#include "config/MemoryConfig.h"

// Load config on startup
auto& config = MemoryConfig::Get();
config.LoadFromFile("../config/memory_config.json");

// Use offsets
int hp = ReadMemory(PLAYER_BASE + MEM_CONFIG.GetPlayerHP_Offset());
```

---

## 📝 memory_config.json Structure

### Base Format

```json
{
  "_metadata": {
    "version": "1.0.0",
    "generated_by": "AutoScanMemory v2.0",
    "generated_at": "2025-01-15 14:30:00",
    "game_version": "1.0.0.123"
  },

  "game_info": {
    "process_name": "JX1.exe",
    "module_name": "JX1.exe",
    "architecture": "x86"
  },

  "base_addresses": {
    "player_base": {
      "address": "0x00A3B000",    ← Auto-filled by scanner
      "verified": true,
      "method": "pointer_scan"
    }
  },

  "player_offsets": {
    "health": {
      "hp": {
        "offset": "0x10",         ← Auto-filled by scanner
        "type": "int32",
        "verified": true,
        "scan_value": 1500        ← Last scanned value
      }
    }
  }
}
```

### All Sections

| Section | Description | Auto-Generated? |
|---------|-------------|-----------------|
| `_metadata` | Version, timestamp | ✅ Yes |
| `game_info` | Process name, architecture | Manual |
| `base_addresses` | Player base, entity manager | ✅ Yes (partial) |
| `player_offsets` | HP, MP, Level, Gold, etc. | ✅ Yes |
| `entity_offsets` | Entity structure offsets | ⚠️ Semi-auto |
| `patterns` | Byte patterns for scanning | Manual |
| `scan_history` | History of all scans | ✅ Yes |

---

## 🔧 C++ Usage Examples

### Example 1: Basic Usage

```cpp
#include "config/MemoryConfig.h"
#include "memory/MemoryManager.h"

void GetPlayerInfo() {
    auto& config = MemoryConfig::Get();
    auto& mem = MemoryManager::Get();

    // Get player base
    uintptr_t playerBase = config.GetPlayerBase();

    if (playerBase) {
        // Get HP
        int hp = mem.Read<int>(playerBase + config.GetPlayerHP_Offset());
        int maxHp = mem.Read<int>(playerBase + config.GetPlayerMaxHP_Offset());

        std::cout << "HP: " << hp << " / " << maxHp << std::endl;
    }
}
```

### Example 2: Using Macros

```cpp
#include "config/MemoryConfig.h"

int hp = ReadMemory<int>(PLAYER_BASE + PLAYER_HP_OFFSET);
int mp = ReadMemory<int>(PLAYER_BASE + PLAYER_MP_OFFSET);
int level = ReadMemory<int>(PLAYER_BASE + PLAYER_LEVEL_OFFSET);
```

### Example 3: Player Class

```cpp
class Player {
public:
    Player(uintptr_t address) : m_Address(address) {}

    int GetHP() const {
        return Read<int>(MEM_CONFIG.GetPlayerHP_Offset());
    }

    void SetHP(int value) {
        Write<int>(MEM_CONFIG.GetPlayerHP_Offset(), value);
    }

    float GetPosX() const {
        return Read<float>(MEM_CONFIG.GetPlayerPosX_Offset());
    }

    void Teleport(float x, float y, float z) {
        Write<float>(MEM_CONFIG.GetPlayerPosX_Offset(), x);
        Write<float>(MEM_CONFIG.GetPlayerPosY_Offset(), y);
        Write<float>(MEM_CONFIG.GetPlayerPosZ_Offset(), z);
    }

private:
    template<typename T>
    T Read(uintptr_t offset) const {
        return MemoryManager::Get().Read<T>(m_Address + offset);
    }

    template<typename T>
    void Write(uintptr_t offset, T value) {
        MemoryManager::Get().Write<T>(m_Address + offset, value);
    }

    uintptr_t m_Address;
};
```

### Example 4: Initialization

```cpp
// In DllMain or initialization function
bool InitializeBot() {
    auto& config = MemoryConfig::Get();

    // Load config
    if (!config.LoadFromFile("../config/memory_config.json")) {
        MessageBoxA(NULL, config.GetLastError().c_str(), "Config Error", MB_OK);
        return false;
    }

    // Print config for debugging
    config.PrintConfig();

    // Check if critical offsets are set
    if (config.GetPlayerBase() == 0) {
        MessageBoxA(NULL, "Player base not configured!", "Error", MB_OK);
        return false;
    }

    return true;
}
```

### Example 5: Hot-Reload (Development)

```cpp
void OnF5_Pressed() {
    auto& config = MemoryConfig::Get();

    if (config.Reload()) {
        std::cout << "Config reloaded successfully!" << std::endl;
        // No need to restart game or DLL
    }
    else {
        std::cerr << "Failed to reload: " << config.GetLastError() << std::endl;
    }
}
```

---

## 🔍 Auto-Scan Workflow Details

### Full Workflow

```
1. Start game
   ↓
2. Run AutoScanMemory_v2.ps1
   ↓
3. Choose "1) Scan for HP"
   ↓
4. Enter current HP → Scanner finds 5000 addresses
   ↓
5. Change HP in game → Enter new HP → 50 addresses remain
   ↓
6. Repeat until <= 10 addresses
   ↓
7. Scanner analyzes memory region around found address
   ↓
8. Scanner suggests nearby offsets (Max HP, MP, etc.)
   ↓
9. Confirm → Config updated automatically
   ↓
10. Choose "5) Batch Scan All" to scan HP, MP, Level, Gold in sequence
    ↓
11. Choose "7) Save Config"
    ↓
12. Run validate_config.ps1 to check
    ↓
13. Use in C++ code!
```

### Memory Region Analysis

When scanner finds HP address, it analyzes nearby memory:

```
Analyzing memory region around 0x12AB5678...
Offset  | Hex Value    | Dec Value   | Type Guess
--------|--------------|-------------|------------
+0x00   | 00 00 00 01  | 1           | Player ID?
+0x04   | 64 00 00 00  | 100         | Level?
+0x08   | E8 03 00 00  | 1000        | Max MP?
+0x0C   | D0 02 00 00  | 720         | Current MP?
+0x10   | DC 05 00 00  | 1500        | ✅ Current HP (confirmed!)
+0x14   | 10 0E 00 00  | 3600        | ✅ Max HP (auto-detected!)
+0x18   | 40 42 0F 00  | 1000000     | Gold?
```

Scanner automatically:
- Finds HP at +0x10
- Detects Max HP at +0x14 (value > HP)
- Updates both in config
- Suggests other nearby values

---

## ✅ Validation

### Run Validator

```powershell
.\validate_config.ps1
```

### What It Checks

- ✅ **Metadata** - Version, timestamp
- ✅ **Game Info** - Process name exists
- ✅ **Base Addresses** - Valid hex format, not 0x0
- ✅ **Player Offsets** - All offsets present, valid format
- ✅ **Patterns** - Pattern/mask defined, lengths match
- ✅ **Scan History** - Has verified scans
- ✅ **Readiness** - Calculate % of verified offsets

### Example Output

```
Validating Metadata...
  [OK]    Version: 1.0.0
  [OK]    Generated at: 2025-01-15 14:30:00

Validating Base Addresses...
  [OK]    player_base : 0x00A3B000 [Verified: true]
  [WARN]  entity_manager_base : Not set (0x0)

Validating Player Offsets...
  Category: health
    [OK]    hp : 0x10 [✓]
    [OK]    max_hp : 0x14 [✓]
    [WARN]  mp : Not set (0x0)

Validation Summary
  ⚠️  0 error(s) and 3 warning(s) found

Configuration Readiness:
  Player Offsets: 4 / 17 verified (23.5%)

Recommendations:
  • Run AutoScanMemory_v2.ps1 to find missing offsets
  ⚠️  Config is partially ready - more offsets needed
```

---

## 🎨 Advanced Features

### 1. Batch Scan All

Scan multiple values in one session:

```
Menu:
5) Batch Scan All

This will scan: HP → MP → Level → Gold

--- Scanning for: HP ---
[Steps for HP scan...]
Continue to next value? (y/n): y

--- Scanning for: MP ---
[Steps for MP scan...]
Continue to next value? (y/n): y

Batch scan completed!
All values saved to config.
```

### 2. View Current Config

```
Menu:
6) View Current Config

Current Configuration
Player Offsets:
  HP:       0x10 (Verified: true)
  Max HP:   0x14 (Verified: true)
  MP:       0x0 (Verified: false)
  Level:    0x30 (Verified: true)
  Gold:     0x50 (Verified: true)

Scan History: 5 scans
```

### 3. Pattern Scanning (Future)

Config supports pattern definitions:

```json
"patterns": {
  "player_base": {
    "pattern": "\\x8B\\x0D\\x00\\x00\\x00\\x00\\x85\\xC9",
    "mask": "xx????xx",
    "offset_to_address": 2,
    "verified": true
  }
}
```

Use in C++:
```cpp
auto pattern = config.GetPlayerBasePattern();
uintptr_t found = mem.FindPattern(pattern.pattern, pattern.mask);
```

---

## 📊 Config vs Code Comparison

### Before (Hardcoded)

```cpp
// Offsets.h - Must edit manually
constexpr uintptr_t PLAYER_BASE = 0x00A3B000;

struct Player {
    static constexpr int HP = 0x10;
    static constexpr int MAX_HP = 0x14;
    // ...
};

// Player.cpp
int GetHP() {
    return Read<int>(PLAYER_BASE + Player::HP);  // Fixed at compile time
}
```

**Problems:**
- ❌ Must recompile after each offset change
- ❌ No way to verify offsets at runtime
- ❌ No scan history
- ❌ Hard to share offsets between tools

### After (Config-Driven)

```cpp
// MemoryConfig.h - Auto-generated from JSON
auto& config = MemoryConfig::Get();
config.LoadFromFile("memory_config.json");

int GetHP() {
    return Read<int>(
        config.GetPlayerBase() + config.GetPlayerHP_Offset()
    );  // Runtime, hot-reloadable
}
```

**Benefits:**
- ✅ No recompile needed
- ✅ Hot-reload during development
- ✅ Verified flag per offset
- ✅ Scan history tracked
- ✅ Shareable JSON file
- ✅ Auto-generated by scanner

---

## 🔧 Troubleshooting

### Problem: Config not loading

```cpp
if (!config.LoadFromFile("../config/memory_config.json")) {
    std::cout << "Error: " << config.GetLastError() << std::endl;
    // Check:
    // 1. File path correct?
    // 2. JSON syntax valid?
    // 3. nlohmann/json library included?
}
```

### Problem: Offsets return 0

```cpp
if (config.GetPlayerHP_Offset() == 0) {
    // Check:
    // 1. Did you scan HP yet?
    // 2. Is offset verified in JSON?
    // 3. Is JSON "offset" field = "0x0"?
}
```

### Problem: Scanner can't find addresses

- Use "Run as Administrator"
- Check process name in config
- Try scanning with different value ranges
- Ensure game is not using anti-cheat

### Problem: Addresses change after restart

- Use pattern scanning instead of static addresses
- Define patterns in config
- Scanner will find addresses dynamically

---

## 📚 Migration Guide

### From Old System to New System

#### Step 1: Keep Both During Transition

```cpp
// Keep Offsets.h as fallback
#include "utils/Offsets.h"
#include "config/MemoryConfig.h"

uintptr_t GetPlayerBase() {
    auto& config = MemoryConfig::Get();

    // Try new system first
    uintptr_t base = config.GetPlayerBase();
    if (base != 0) {
        return base;
    }

    // Fallback to old system
    return Offsets::PLAYER_BASE;
}
```

#### Step 2: Gradually Migrate

```cpp
// Week 1: HP, MP
int hp = Read<int>(PLAYER_BASE + MEM_CONFIG.GetPlayerHP_Offset());

// Week 2: Position
float x = Read<float>(PLAYER_BASE + MEM_CONFIG.GetPlayerPosX_Offset());

// Week 3: All player offsets migrated
// Week 4: Entity offsets
// Week 5: Remove Offsets.h completely
```

#### Step 3: Update Build System

```cmake
# CMakeLists.txt
target_sources(core_dll PRIVATE
    src/config/MemoryConfig.cpp
    # ...
)

# Add nlohmann/json
find_package(nlohmann_json REQUIRED)
target_link_libraries(core_dll PRIVATE nlohmann_json::nlohmann_json)
```

---

## 🎓 Best Practices

### 1. Always Validate Before Build

```batch
# Before building
tools\validate_config.ps1

# Only build if validation passes
if %ERRORLEVEL% == 0 (
    scripts\build.bat
)
```

### 2. Commit Config to Git

```bash
git add config/memory_config.json
git commit -m "Update memory offsets for version 1.0.0.123"
```

### 3. Version Your Configs

```
config/
├── memory_config.json              ← Current/latest
├── memory_config_v1.0.0.100.json  ← Backup for game version 1.0.0.100
└── memory_config_v1.0.0.123.json  ← Backup for game version 1.0.0.123
```

### 4. Document Findings

```json
"player_offsets": {
  "health": {
    "hp": {
      "offset": "0x10",
      "description": "Found via scanner on 2025-01-15, stable across restarts",
      "verified": true,
      "scan_value": 1500
    }
  }
}
```

### 5. Test After Each Scan

```cpp
// Test function after scanning
void TestOffsets() {
    auto& config = MemoryConfig::Get();

    int hp = GetPlayerHP();
    if (hp > 0 && hp < 100000) {
        std::cout << "✅ HP offset working: " << hp << std::endl;
    } else {
        std::cout << "❌ HP offset might be wrong!" << std::endl;
    }
}
```

---

## 🚀 Summary

| Feature | Old System | New System |
|---------|------------|------------|
| Config Storage | Hardcoded in .h files | JSON file |
| Update Process | Edit code → Recompile | Scan → Auto-generate → Done |
| Hot-Reload | ❌ No | ✅ Yes |
| Validation | ❌ No | ✅ Yes |
| Scan History | ❌ No | ✅ Yes |
| Verification Status | ❌ No | ✅ Yes |
| Shareable | ❌ Hard | ✅ Easy (JSON) |
| Auto-Generation | ❌ Manual | ✅ Auto-scan tool |

**Next Steps:**

1. ✅ Run `AutoScanMemory_v2.ps1`
2. ✅ Scan HP, MP, Level, Gold
3. ✅ Save config
4. ✅ Validate with `validate_config.ps1`
5. ✅ Use in C++ code
6. ✅ Build and test!

---

**Happy Reverse Engineering! 🔍**
