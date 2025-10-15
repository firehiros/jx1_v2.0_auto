# Auto Memory Scanner Tools

## 📦 What's Included

| Tool | Description | Usage |
|------|-------------|-------|
| [auto_scan.bat](auto_scan.bat) | **Main launcher** - Double-click to run | Just double-click! |
| [AutoScanMemory_v2.ps1](AutoScanMemory_v2.ps1) | **Auto scanner** - Scans memory & generates config | PowerShell script |
| [validate_config.ps1](validate_config.ps1) | **Config validator** - Check config completeness | Run after scanning |
| [auto_scan_memory.lua](auto_scan_memory.lua) | **Cheat Engine script** - Alternative method | Load in CE |

## 🚀 Quick Start (3 Steps)

### Step 1: Scan Memory
```batch
1. Open game JX1
2. Double-click: auto_scan.bat
3. Choose menu option (e.g., "1" for HP scan)
```

### Step 2: Validate
```powershell
.\validate_config.ps1
```

### Step 3: Use in Code
```cpp
#include "config/MemoryConfig.h"

auto& config = MemoryConfig::Get();
config.LoadFromFile("../config/memory_config.json");

int hp = ReadMemory(PLAYER_BASE + PLAYER_HP_OFFSET);
```

## 📖 Documentation

- **Complete Guide**: [CENTRALIZED_CONFIG_GUIDE.md](../docs/03-reverse-engineering/CENTRALIZED_CONFIG_GUIDE.md)
- **After Scan Config**: [AFTER_SCAN_CONFIG.md](../docs/03-reverse-engineering/AFTER_SCAN_CONFIG.md)
- **Cheat Engine Guide**: [CHEAT_ENGINE_GUIDE.md](../docs/03-reverse-engineering/CHEAT_ENGINE_GUIDE.md)

## 🎯 How It Works

```
┌─────────────────┐
│  1. Run         │
│  auto_scan.bat  │ ← Double-click
└────────┬────────┘
         │
         ▼
┌─────────────────────────┐
│  2. AutoScanMemory_v2   │
│  - Scan HP, MP, Gold... │ ← Interactive menu
│  - Auto-generate config │
└────────┬────────────────┘
         │
         ▼
┌─────────────────────────┐
│  3. memory_config.json  │ ← Auto-generated
│  All offsets saved here │
└────────┬────────────────┘
         │
         ▼
┌─────────────────────────┐
│  4. validate_config.ps1 │ ← Check completeness
│  Shows readiness: 75%   │
└────────┬────────────────┘
         │
         ▼
┌─────────────────────────┐
│  5. C++ Code            │
│  MemoryConfig::Get()    │ ← Use in bot
│  Auto-load from JSON    │
└─────────────────────────┘
```

## 🔧 Features

### ✅ AutoScanMemory_v2.ps1

- **Auto-scan workflow**: Step-by-step guided scanning
- **Memory analysis**: Analyzes region around found address
- **Smart suggestions**: Detects Max HP, MP automatically
- **Batch scan**: Scan HP, MP, Level, Gold in sequence
- **JSON generation**: Auto-generates `memory_config.json`
- **Scan history**: Tracks all scans with timestamps
- **Hot-reload**: Update config without restarting

### ✅ validate_config.ps1

- **Completeness check**: How many offsets are verified?
- **Format validation**: Check hex addresses are valid
- **Pattern validation**: Pattern/mask lengths match
- **Readiness score**: Shows % ready (e.g., 75%)
- **Recommendations**: Tells you what's missing

### ✅ memory_config.json

- **Single source of truth**: All offsets in one file
- **No code changes**: Update JSON, no recompile needed
- **Verified flags**: Track which offsets are tested
- **Scan metadata**: Timestamp, game version, method used
- **Extensible**: Easy to add new offsets

## 📋 Menu Options (AutoScanMemory_v2)

```
1) Scan for HP (Auto mode)       ← Find HP address + Max HP
2) Scan for MP (Auto mode)       ← Find MP address + Max MP
3) Scan for Level                ← Find Level address
4) Scan for Gold/Money           ← Find Gold address
5) Batch Scan All                ← Scan HP → MP → Level → Gold
6) View Current Config           ← Show what's configured
7) Save Config                   ← Save to JSON file
8) Exit                          ← Exit program
```

## 🎓 Example: Scan HP

```
Run: auto_scan.bat
Select: 1 (Scan for HP)

[Step 1] Enter your CURRENT HP value: 1500
Scanning memory...
Found 5000 addresses

[Step 2] Change your HP (take damage/heal)
Enter your NEW HP value: 1200
Filtering...
Remaining: 50 addresses

[Step 3] Change HP again
Enter your NEW HP value: 1000
Remaining: 3 addresses

SUCCESS! Found HP address(es):
  0x12AB5678  ← This is the one!

Analyzing memory region around 0x12AB5678...
Offset  | Value    | Guess
--------|----------|------------
+0x00   | 1        | Player ID?
+0x10   | 1000     | ✅ Current HP
+0x14   | 3600     | ✅ Max HP (auto-detected!)
+0x18   | 720      | Current MP?
+0x1C   | 1000     | Max MP?

Is the first address correct? (y/n): y

✅ Config updated!
  - HP offset: 0x10 (verified)
  - Max HP offset: 0x14 (verified)

Select: 7 (Save Config)
Config saved to: ../config/memory_config.json
```

## 🔍 Validate Config

```powershell
PS> .\validate_config.ps1

========================================
   Memory Config Validator
========================================

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
    [OK]    mp : 0x18 [✓]
    [OK]    max_mp : 0x1C [✓]
  Category: character
    [OK]    level : 0x30 [✓]
    [WARN]  gold : Not set (0x0)

========================================
   Validation Summary
========================================
  ⚠️  0 error(s) and 3 warning(s) found

Configuration Readiness:
  Player Offsets: 5 / 17 verified (29.4%)

Recommendations:
  • Run AutoScanMemory_v2.ps1 to find missing offsets
  • 12 more offsets needed for full configuration
```

## 🎯 Generated Config Example

```json
{
  "_metadata": {
    "version": "1.0.0",
    "generated_by": "AutoScanMemory v2.0",
    "generated_at": "2025-01-15 14:30:00"
  },

  "base_addresses": {
    "player_base": {
      "address": "0x00A3B000",
      "verified": true
    }
  },

  "player_offsets": {
    "health": {
      "hp": {
        "offset": "0x10",
        "type": "int32",
        "verified": true,
        "scan_value": 1500
      },
      "max_hp": {
        "offset": "0x14",
        "type": "int32",
        "verified": true,
        "scan_value": 3600
      }
    }
  },

  "scan_history": [
    {
      "timestamp": "2025-01-15 14:25:00",
      "value_type": "hp",
      "addresses_found": ["0x12AB5678"],
      "filters_applied": 2,
      "verified": true
    }
  ]
}
```

## 🔗 C++ Integration

### Load Config on Startup

```cpp
// In DllMain
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        // Load memory config
        auto& config = MemoryConfig::Get();
        if (!config.LoadFromFile("../config/memory_config.json")) {
            MessageBoxA(NULL, config.GetLastError().c_str(),
                       "Config Error", MB_OK);
            return FALSE;
        }

        // Initialize bot
        InitializeBot();
    }
    return TRUE;
}
```

### Use in Player Class

```cpp
class Player {
public:
    int GetHP() const {
        return Read<int>(MEM_CONFIG.GetPlayerHP_Offset());
    }

    void SetHP(int value) {
        Write<int>(MEM_CONFIG.GetPlayerHP_Offset(), value);
    }

    bool IsDead() const {
        return GetHP() <= 0;
    }

    float GetHealthPercent() const {
        int hp = GetHP();
        int maxHp = Read<int>(MEM_CONFIG.GetPlayerMaxHP_Offset());
        return maxHp > 0 ? (float)hp / maxHp : 0.0f;
    }
};
```

## 💡 Tips

1. **Run as Administrator** for better memory access
2. **Change values significantly** (e.g., HP: 1500 → 1000 instead of 1500 → 1490)
3. **Use Batch Scan** to scan multiple values efficiently
4. **Validate before building** to ensure config is complete
5. **Save config after each successful scan** to avoid data loss
6. **Check scan history** to see what's been found
7. **Hot-reload in dev mode** - No need to restart game

## 🐛 Troubleshooting

### Scanner finds too many addresses

- Change the value more significantly
- Run more filter iterations
- Wait a few seconds between scans

### Scanner finds no addresses

- Check if running as Administrator
- Verify process name is correct
- Try scanning for different values first
- Ensure game is not using anti-cheat

### Config validation fails

- Check JSON syntax (use JSONLint)
- Ensure hex addresses start with "0x"
- Run scanner again for missing offsets

### C++ can't load config

- Check file path is correct (relative to DLL location)
- Ensure nlohmann/json library is linked
- Verify JSON file is valid UTF-8

## 📚 Further Reading

- [Centralized Config Guide](../docs/03-reverse-engineering/CENTRALIZED_CONFIG_GUIDE.md) - Complete guide
- [After Scan Config](../docs/03-reverse-engineering/AFTER_SCAN_CONFIG.md) - What to do after scanning
- [Cheat Engine Guide](../docs/03-reverse-engineering/CHEAT_ENGINE_GUIDE.md) - Manual scanning guide

## 🎉 Ready to Go!

```batch
# 1. Double-click this:
auto_scan.bat

# 2. Follow the prompts
# 3. Save config
# 4. Validate
.\validate_config.ps1

# 5. Build and test!
..\scripts\build.bat
```

**Happy reverse engineering! 🔍**
