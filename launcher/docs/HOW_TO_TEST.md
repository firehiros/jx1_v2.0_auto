# How to Test Multi-Account UI

**Created:** 2025-10-15
**Status:** Ready to Test

---

## 🚀 Quick Test (5 Minutes)

### Step 1: Build Project

```powershell
cd launcher
dotnet build
```

**Expected output:**
```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

### Step 2: Run Launcher

```powershell
dotnet run
```

**Expected:** Window opens with "Accounts" tab visible

### Step 3: Add Test Accounts

Click **[➕ Add Account]** button 3 times.

**Expected:** 3 accounts appear in "Available Accounts" list

### Step 4: Check UI

**You should see:**
- ✅ Header: "JX1 Auto v2.0 - Multi-Account Edition"
- ✅ "Accounts" tab (new!)
- ✅ "Accounts (0 active / 3 total)" header
- ✅ 3 accounts in "Available Accounts" list
- ✅ [▶ Start] [✎ Edit] buttons for each account
- ✅ Footer showing "Total Statistics"

---

## 🧪 Detailed Testing

### Test 1: Add Account

1. Click **[➕ Add Account]**
2. Account appears with default name "Account N"
3. Check available accounts list updates
4. Check total count increases

**Expected Result:** ✅ Account added successfully

### Test 2: Edit Account (Manual)

Currently need to manually edit `accounts.json`:

```json
[
  {
    "Id": 1,
    "Nickname": "Main Tank",
    "Username": "tank@email.com",
    "CharacterName": "LongNV_001",
    "Level": 99,
    "Enabled": true,
    "BotMode": "Farming"
  },
  {
    "Id": 2,
    "Nickname": "DPS Alt",
    "Username": "dps@email.com",
    "CharacterName": "LongNV_002",
    "Level": 85,
    "Enabled": true,
    "BotMode": "Combat"
  }
]
```

Then click **[🔄 Refresh]**

**Expected Result:** ✅ Accounts show with custom names

### Test 3: Start Account (Mock Test)

**Note:** This will fail without game running, which is expected!

1. Click **[▶ Start]** on Account 1
2. Check error message appears
3. Account stays in "Idle" state

**Expected Result:** ⚠️ Error "Game process not found" (This is correct!)

### Test 4: UI Colors

Check account cards have correct colors:
- Idle accounts: Gray border
- Would be green when running
- Would be yellow when paused
- Would be red on error

**Expected Result:** ✅ Color coding works

### Test 5: Total Stats

Check footer shows:
- Total Kills: 0
- Total Items: 0
- Total Gold: 0
- Total Runtime: 00:00:00

**Expected Result:** ✅ Stats display correctly (all zeros for now)

---

## 🎮 Test with Real Game (Advanced)

### Prerequisites

1. ✅ Game JX1 running
2. ✅ Memory offsets configured (memory_config.json)
3. ✅ Core DLL built (core_dll.dll)

### Test Steps

#### 1. Start Game

```
Open game JX1
Login with account 1
Wait at character selection or in-game
```

#### 2. Add Account in Launcher

```json
// Edit accounts.json
[
  {
    "Id": 1,
    "Nickname": "Test Account",
    "Username": "test@email.com",
    "CharacterName": "",  // Will auto-detect
    "Level": 0,           // Will auto-detect
    "Enabled": true,
    "BotMode": "Farming"
  }
]
```

Refresh launcher.

#### 3. Start Account

1. Click **[▶ Start]** on Account 1
2. Process detector finds game window
3. DLL injects to process
4. Account card moves to "Active Accounts"
5. Status changes to "✓ Running"
6. HP/MP bars appear (if offsets configured)

**Expected Result:** ✅ Account starts successfully!

#### 4. Monitor Stats

Watch account card update:
- HP/MP bars should show real values
- Runtime counter increases
- Stats update (kills, items, gold)

**Expected Result:** ✅ Real-time stats working!

#### 5. Pause Account

1. Click **[⏸]** on account card
2. Status changes to "⏸ Paused"
3. Card border turns yellow
4. Bot pauses (if DLL supports it)

**Expected Result:** ✅ Pause works!

#### 6. Stop Account

1. Click **[⏹]** on account card
2. Status changes to "Stopping..."
3. DLL ejects
4. Account moves back to "Available Accounts"

**Expected Result:** ✅ Stop works!

---

## 🐛 Expected Issues (Known Limitations)

### Issue 1: Game Process Not Found

**Symptom:** Click Start → Error "Game process not found"

**Reason:** Game not running or process name mismatch

**Fix:**
```ini
# Edit LAUNCHER_DESIGN.txt
[ProcessManagement]
ProcessName = JX1.exe          # Change to actual process name
WindowTitlePattern = 正確的視窗標題  # Change to actual window title
```

### Issue 2: InjectionFailed

**Symptom:** Click Start → Error "Failed to inject DLL"

**Reason:** DLL not built or not found

**Fix:**
```bash
# Build core DLL first
cd core_dll
cmake --build build --config Release

# Check DLL exists
ls core_dll/build/Release/core_dll.dll
```

### Issue 3: HP/MP Bars Show 0%

**Symptom:** Account starts but HP/MP = 0/0

**Reason:** Memory offsets not configured

**Fix:**
```bash
# Run memory scanner
cd tools
auto_scan.bat

# Scan HP, MP offsets
# Save to memory_config.json
```

### Issue 4: Stats Not Updating

**Symptom:** Kills/Items/Gold stay at 0

**Reason:** IPC not implemented yet (Phase 2)

**Current Status:** ⏳ This is expected! IPC will be added later.

---

## ✅ Success Criteria

### Minimum (UI Testing)

- [ ] Launcher builds successfully
- [ ] Window opens without errors
- [ ] "Accounts" tab visible
- [ ] Can add accounts
- [ ] Accounts appear in list
- [ ] Buttons are clickable
- [ ] Colors display correctly

### With Game (Integration Testing)

- [ ] Detects game process
- [ ] Injects DLL successfully
- [ ] Account status updates
- [ ] HP/MP bars show real data
- [ ] Stats update in real-time
- [ ] Can pause/resume
- [ ] Can stop account
- [ ] Multiple accounts work simultaneously

---

## 🎯 Current Test Status

### ✅ Can Test Now (No Game Needed)

- UI layout
- Add accounts
- UI colors and styling
- Button states
- List/Grid display
- Footer stats (zeros)

### ⏳ Need Game to Test

- Process detection
- DLL injection
- Real HP/MP data
- Stats tracking
- Pause/Resume
- Multi-instance support

---

## 📝 Test Checklist

### Build & Launch
- [ ] `dotnet build` succeeds
- [ ] `dotnet run` launches window
- [ ] No runtime errors in console
- [ ] Window size: 1000x700
- [ ] Title: "JX1 Auto v2.0 - Multi-Account Launcher"

### UI Components
- [ ] "Accounts" tab exists and is first tab
- [ ] Header shows "Accounts (X active / Y total)"
- [ ] Global buttons visible: Add, Refresh, Start All, Stop All
- [ ] Footer shows total stats

### Account Management
- [ ] Click Add Account → New account appears
- [ ] Account has default name "Account N"
- [ ] Account appears in "Available Accounts"
- [ ] Each account has [▶ Start] [✎ Edit] buttons

### Visual Styling
- [ ] Dark theme applied
- [ ] Header background: #1E1E1E
- [ ] Account cards have borders
- [ ] Idle accounts have gray borders
- [ ] Footer has stats display

---

## 🔧 Build Commands

### Clean Build

```powershell
dotnet clean
dotnet build --configuration Release
```

### Debug Build

```powershell
dotnet build --configuration Debug
dotnet run
```

### Check for Errors

```powershell
dotnet build --verbosity detailed
```

---

## 📊 Expected Output

### Console Output (Normal)

```
Build started...
Build succeeded.
    1 Project(s) succeeded

Time Elapsed 00:00:03.45
```

### Console Output (Errors)

If you see errors like:
```
error CS0246: The type or namespace name 'AccountProfile' could not be found
```

**Fix:** Make sure all files are in project file:
```powershell
dotnet build --force
```

---

## 🎨 Visual Preview

When running, you should see:

```
╔══════════════════════════════════════════════════╗
║ JX1 Auto v2.0 - Multi-Account Edition          ║
╠══════════════════════════════════════════════════╣
║ [Accounts] [Legacy Mode] [Settings] [About]     ║
╠══════════════════════════════════════════════════╣
║ Accounts (0 active / 3 total)  [➕][🔄][▶][⏹] ║
╠══════════════════════════════════════════════════╣
║                                                  ║
║  No active accounts. Click '▶ Start' to begin.  ║
║                                                  ║
║ ┌─ Available Accounts ─────────────────────────┐║
║ │ ● 1: Account 1      [▶ Start] [✎ Edit]      │║
║ │ ● 2: Account 2      [▶ Start] [✎ Edit]      │║
║ │ ● 3: Account 3      [▶ Start] [✎ Edit]      │║
║ └──────────────────────────────────────────────┘║
║                                                  ║
╠══════════════════════════════════════════════════╣
║ Total: 0 kills │ 0 items │ 0 gold │ 00:00:00   ║
╚══════════════════════════════════════════════════╝
```

---

## 🚀 Next Steps After Testing

1. ✅ If UI works → Test with game
2. ✅ If game detection works → Test injection
3. ✅ If injection works → Test stats
4. ✅ If stats work → Test multi-account
5. ✅ Production ready!

---

**Ready to test!** Run: `cd launcher && dotnet run` 🎉

