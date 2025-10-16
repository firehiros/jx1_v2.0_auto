# Memory Configuration Validation

## Overview

The launcher now includes automatic validation of `memory_config.json` to prevent crashes and ensure correct operation. When you attempt to inject the DLL, the launcher will:

1. ✅ Validate all memory addresses, offsets, and patterns
2. ❌ Show detailed error report if issues are found
3. ⚠️ Allow you to continue anyway (not recommended)
4. 📝 Provide helpful suggestions to fix each issue

## Features

### Automatic Validation

Before DLL injection, the launcher automatically:
- Checks all base addresses (player, entity manager, inventory)
- Validates player offsets (HP, MP, position, level, etc.)
- Verifies entity offsets
- Checks inventory offsets
- Validates AOB patterns
- Verifies function addresses

### Error Report Window

If validation fails, a detailed error window shows:

#### Statistics Panel
- **Total Checked**: Number of items validated
- **Errors**: Critical issues that need fixing (addresses = 0x0)
- **Warnings**: Non-critical issues (unverified addresses)
- **Status**: Overall validation status

#### Errors Tab
For each error, you'll see:
- **Category**: Which section has the issue (e.g., "Base Addresses", "Player Offsets")
- **Field**: Specific field name (e.g., "player_base", "hp")
- **Path**: JSON path to the field (e.g., `base_addresses.player_base.address`)
- **Issue**: What's wrong (e.g., "Address is 0x0 or invalid")
- **Current Value**: The invalid value found
- **Suggestion**: How to fix it (e.g., "Use Cheat Engine to find the correct base address")

#### Warnings Tab
Shows non-critical issues:
- Addresses that haven't been verified yet
- Patterns that might need testing

### Action Buttons

1. **Open Config File**: Opens `memory_config.json` in your default editor
2. **Retry Validation**: Re-validates after you make changes
3. **Continue Anyway**: Proceeds with injection despite errors (⚠️ NOT RECOMMENDED)
4. **Close**: Cancels injection to fix issues first

## Usage Example

### Scenario 1: All Addresses Are Default (0x0)

When you first try to inject:

```
❌ Memory Configuration Validation Failed

Found 45 errors and 0 warnings in 50 items checked

Errors:
❌ Base Addresses → player_base
   Path: base_addresses.player_base.address
   Issue: Address is 0x0 or invalid (Current: 0x0)
   💡 Use Cheat Engine to find the correct base address

❌ Player Offsets → hp
   Path: player_offsets.health.hp.offset
   Issue: Offset is 0x0 or invalid (Current: 0x0)
   💡 Use Cheat Engine to find the correct offset

[... more errors ...]
```

### Scenario 2: After Fixing Some Addresses

```
✅ Memory Configuration Validation Passed

Found 0 errors and 12 warnings in 50 items checked

Warnings:
⚠️ Player Offsets → gold
   Offset not verified - may cause issues

⚠️ Function Addresses → use_skill
   Function address not verified
```

The launcher will allow injection with warnings.

## How to Fix Errors

### Step 1: Use Cheat Engine
Follow the guide: [CHEAT_ENGINE_GUIDE.md](03-reverse-engineering/CHEAT_ENGINE_GUIDE.md)

### Step 2: Update Config File
Open `config/memory_config.json` and update the values:

```json
{
  "base_addresses": {
    "player_base": {
      "address": "0x12345678",  // ✅ Update this
      "verified": true,          // ✅ Mark as verified
      "found_at": "2025-01-15 10:30:00"
    }
  },
  "player_offsets": {
    "health": {
      "hp": {
        "offset": "0x10",        // ✅ Update offsets
        "verified": true
      }
    }
  }
}
```

### Step 3: Retry Validation
Click "Retry Validation" in the error window to re-check.

## Technical Details

### Validation Rules

An address/offset is considered **invalid** if:
- Value is `"0x0"`, `"0"`, or `"null"`
- Value is empty or whitespace
- Field is missing

An address/offset generates a **warning** if:
- `verified` field is `false`
- Value looks valid but hasn't been tested

### Validation Categories

1. **Base Addresses** (Critical)
   - player_base
   - entity_manager_base
   - inventory_base

2. **Player Offsets** (Important)
   - Health (hp, max_hp, mp, max_mp)
   - Position (x, y, z, direction)
   - Character (level, experience, gold, name, id)
   - State (is_dead, in_combat, is_moving)

3. **Entity Offsets** (Important)
   - entity_id, entity_type
   - hp, max_hp
   - pos_x, pos_y, pos_z
   - name, is_dead, is_hostile

4. **Inventory Offsets** (Medium)
   - item_count, max_items
   - items_array

5. **Patterns** (Advanced)
   - player_base, entity_manager
   - attack_function, moveto_function
   - useskill_function

6. **Function Addresses** (Advanced)
   - attack, use_skill
   - move_to, pickup_item
   - use_item

## Bypass Validation (Development Only)

If you need to test without validation:

```csharp
// In code
_injector.InjectDll(skipValidation: true);
```

Or click "Continue Anyway" in the error window.

⚠️ **WARNING**: This may cause:
- Application crashes
- Game crashes
- Incorrect memory reads/writes
- Unexpected behavior

## Configuration Service API

### MemoryConfigValidator

```csharp
var validator = new MemoryConfigValidator();

// Validate config
var result = validator.ValidateConfig();

if (!result.IsValid)
{
    Console.WriteLine($"Found {result.InvalidCount} errors");

    foreach (var error in result.Errors)
    {
        Console.WriteLine($"{error.Category} → {error.Field}: {error.Issue}");
    }
}

// Quick check
bool isValid = validator.IsConfigValid();

// Get summary
string summary = validator.GetValidationSummary();
```

### ValidationResult

```csharp
public class ValidationResult
{
    public bool IsValid { get; set; }
    public List<ValidationError> Errors { get; set; }
    public List<ValidationWarning> Warnings { get; set; }
    public int TotalChecked { get; set; }
    public int InvalidCount => Errors.Count;
    public int WarningCount => Warnings.Count;
}
```

## Benefits

✅ **Prevents Crashes**: Catches invalid config before injection
✅ **Clear Feedback**: Shows exactly what's wrong and how to fix it
✅ **Time Saving**: No more guessing which address is incorrect
✅ **User Friendly**: Visual error window with helpful suggestions
✅ **Flexible**: Can bypass validation for testing (not recommended)

## Future Enhancements

- [ ] Auto-scan mode to find addresses automatically
- [ ] Config templates for different game versions
- [ ] Export/import verified configs
- [ ] Live validation while editing config
- [ ] Integration with Cheat Engine scripts

## Related Documentation

- [CHEAT_ENGINE_GUIDE.md](03-reverse-engineering/CHEAT_ENGINE_GUIDE.md) - How to find addresses
- [PHASE2_EXECUTION_GUIDE.md](PHASE2_EXECUTION_GUIDE.md) - Complete Phase 2 guide
- [BUILD_INSTRUCTIONS.md](01-build/BUILD_INSTRUCTIONS.md) - Building the launcher

---

**Note**: This validation system is designed to help you complete Phase 2 more efficiently by catching configuration errors early!
