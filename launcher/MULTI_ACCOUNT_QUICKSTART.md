# Multi-Account Quick Start Guide

**Goal:** Configure launcher để hỗ trợ nhiều tài khoản

**Time:** 10 phút

---

## 🎯 Mục Tiêu

Sau khi hoàn thành guide này, bạn sẽ có:
- ✅ Launcher hỗ trợ 3-5 tài khoản
- ✅ UI hiển thị nhiều account cards
- ✅ Control riêng biệt cho mỗi account
- ✅ Statistics tổng hợp

---

## ⚡ Quick Setup (2 phút)

### Bước 1: Enable Multi-Account

Edit `LAUNCHER_DESIGN.txt`:

```ini
[MultiAccount]
EnableMultiAccount = Yes
MaxAccounts = 5
```

### Bước 2: Add Accounts

```ini
[Account.1]
Enabled = Yes
Nickname = Main Character
Username = your_email1@gmail.com

[Account.2]
Enabled = Yes
Nickname = Alt Character
Username = your_email2@gmail.com

[Account.3]
Enabled = Yes
Nickname = Support Char
Username = your_email3@gmail.com
```

### Bước 3: Apply Changes

```powershell
cd launcher
.\apply_design.ps1
```

**Done!** 🎉

---

## 📝 Detailed Setup (10 phút)

### Step 1: Configure Display (2 phút)

Edit `LAUNCHER_DESIGN.txt`:

```ini
[MultiAccount.Display]
# Hiển thị dạng cards
DisplayMode = Cards

# 2 cards mỗi hàng
CardsPerRow = 2

# Hiển thị cả accounts inactive
ShowInactiveAccounts = Yes

# Kích thước card
CardHeight = 200
CardWidth = 400
CardSpacing = 10

# Màu theo trạng thái
ColorRunning = #00FF00    # Xanh lá = đang chạy
ColorPaused = #FFAA00     # Vàng = đang pause
ColorStopped = #888888    # Xám = đã dừng
ColorError = #FF0000      # Đỏ = lỗi
```

### Step 2: Configure Account Cards (2 phút)

```ini
[MultiAccount.AccountCard]
# Thông tin hiển thị trên mỗi card
ShowNickname = Yes            # Hiện nickname
ShowCharacterName = Yes       # Hiện tên nhân vật
ShowLevel = Yes               # Hiện level
ShowMap = Yes                 # Hiện map hiện tại
ShowHP = Yes                  # Hiện thanh HP
ShowMP = Yes                  # Hiện thanh MP
ShowStats = Yes               # Hiện stats (kills, items)
ShowButtons = Yes             # Hiện nút control

# Chế độ compact (ít thông tin hơn)
CompactMode = No
```

### Step 3: Configure Controls (2 phút)

```ini
[MultiAccount.Controls]
# Nút global (áp dụng cho tất cả)
ShowAddAccount = Yes      # Nút thêm account
ShowRefresh = Yes         # Nút refresh
ShowStopAll = Yes         # Nút stop tất cả
ShowStartAll = Yes        # Nút start tất cả
ShowGlobalSettings = Yes  # Nút settings chung

# Nút cho mỗi account
ShowPauseButton = Yes     # Nút pause
ShowStopButton = Yes      # Nút stop
ShowConfigButton = Yes    # Nút config
ShowDetailsButton = Yes   # Nút xem chi tiết
```

### Step 4: Add Your Accounts (2 phút)

```ini
[Account.1]
Enabled = Yes
Nickname = Main Tank         # Tên gọi của bạn
Username = tank@email.com    # Email/username game
CharacterName = LongNV_001   # Tên nhân vật (optional)
Level = 99                   # Level (optional, sẽ auto-detect)
DefaultMode = Farming        # Chế độ mặc định

[Account.2]
Enabled = Yes
Nickname = DPS Alt
Username = dps@email.com
CharacterName = LongNV_002
Level = 85
DefaultMode = Combat

[Account.3]
Enabled = Yes
Nickname = Support
Username = support@email.com
CharacterName = LongNV_003
Level = 75
DefaultMode = Questing

# Thêm account 4, 5... tùy ý
```

### Step 5: Configure Process Management (2 phút)

```ini
[ProcessManagement]
# Phát hiện game process
DetectionMethod = WindowTitle
ProcessName = JX1.exe
WindowTitlePattern = Võ Lâm Truyền Kỳ

# Tự động phát hiện mỗi 5 giây
AutoDetectInterval = 5000

# Cho phép nhiều instances
AllowMultipleInstances = Yes
MaxInstances = 5

[ProcessManagement.Injection]
# Đợi 2 giây trước khi inject
InjectionDelay = 2000

# Thử lại nếu fail
RetryOnFail = Yes
RetryCount = 3
RetryDelay = 5000
```

---

## 🎨 UI Preview

Sau khi setup, UI sẽ trông như thế này:

```
╔═════════════════════════════════════════════════════════╗
║ JX1 Auto v2.0 - Multi-Account Manager                  ║
╠═════════════════════════════════════════════════════════╣
║ Accounts (2 active / 3 total)    [➕] [🔄] [⏹] [⚙]   ║
╠═════════════════════════════════════════════════════════╣
║                                                          ║
║  ╔════════════════════╗    ╔════════════════════╗      ║
║  ║ Account 1: Tank   ║    ║ Account 2: DPS     ║      ║
║  ╠════════════════════╣    ╠════════════════════╣      ║
║  ║ LongNV_001 (Lv 99)║    ║ LongNV_002 (Lv 85)║      ║
║  ║ Map: Đại Lý       ║    ║ Map: Tô Châu       ║      ║
║  ║ HP: [██████████]  ║    ║ HP: [████████░░]   ║      ║
║  ║ MP: [██████████]  ║    ║ MP: [██████████]   ║      ║
║  ║                    ║    ║                    ║      ║
║  ║ ✓ Running │ Farming║    ║ ✓ Running │ Combat ║      ║
║  ║ 01:30:00          ║    ║ 00:45:00           ║      ║
║  ║                    ║    ║                    ║      ║
║  ║ Kills: 200        ║    ║ Kills: 85          ║      ║
║  ║ Items: 300        ║    ║ Items: 120         ║      ║
║  ║                    ║    ║                    ║      ║
║  ║ [⏸][⏹][⚙][📊]   ║    ║ [⏸][⏹][⚙][📊]    ║      ║
║  ╚════════════════════╝    ╚════════════════════╝      ║
║                                                          ║
║  ┌─ Available ─────────────────────────────────────┐   ║
║  │ • Account 3: Support    [▶ Start] [✎ Edit]     │   ║
║  └─────────────────────────────────────────────────┘   ║
║                                                          ║
╠═════════════════════════════════════════════════════════╣
║ Total: 285 kills │ 420 items │ 02:15:00                ║
╚═════════════════════════════════════════════════════════╝
```

---

## 🎮 Usage

### Start một account

1. Click **[▶ Start]** bên cạnh account name
2. Launcher sẽ detect game window
3. Auto-inject DLL
4. Bot bắt đầu chạy

### Start tất cả accounts

1. Click **[⏵ Start All]** ở trên
2. Tất cả enabled accounts sẽ được start

### Pause account

1. Click **[⏸ Pause]** trên account card
2. Bot tạm dừng (có thể resume)

### Stop account

1. Click **[⏹ Stop]** trên account card
2. Bot dừng hoàn toàn
3. DLL được eject

### Configure account

1. Click **[⚙ Config]** trên account card
2. Mở settings riêng cho account đó

### View details

1. Click **[📊 Details]** trên account card
2. Xem stats chi tiết

---

## 🔧 Advanced Configuration

### Startup Behavior

```ini
[MultiAccount.StartupBehavior]
# Tự động phát hiện game đang chạy
AutoDetectRunningGames = Yes

# Tự động inject tất cả khi start
AutoInjectAll = No

# Restore session trước
RestoreLastSession = Yes

# Cảnh báo khi > 3 accounts
WarnAboveAccounts = 3

# Tối đa 5 accounts cùng lúc
MaxConcurrentAccounts = 5
```

### Statistics Aggregation

```ini
[Statistics]
# Hiển thị tổng stats của tất cả accounts
ShowTotalKills = Yes
ShowTotalGold = Yes
ShowTotalItems = Yes
ShowTotalRuntime = Yes

# Update stats mỗi giây
StatsUpdateInterval = 1000

# Export stats tự động
EnableStatsExport = Yes
ExportInterval = 300000      # Mỗi 5 phút
ExportPath = logs/stats_{date}.csv
```

---

## 💡 Tips

### Tip 1: Màu sắc giúp phân biệt

```ini
[MultiAccount.Display]
ColorRunning = #00FF00    # Xanh = đang farm
ColorPaused = #FFAA00     # Vàng = đang pause
ColorError = #FF0000      # Đỏ = có lỗi
```

### Tip 2: Compact mode cho nhiều accounts

Nếu có > 5 accounts, dùng compact mode:

```ini
[MultiAccount.AccountCard]
CompactMode = Yes
```

Card sẽ nhỏ gọn hơn, ít thông tin hơn.

### Tip 3: Sync settings

Nếu muốn tất cả accounts dùng chung settings:

```ini
[MultiAccount]
SyncSettings = Yes
```

### Tip 4: Auto-start all

Để tự động start tất cả khi mở launcher:

```ini
[MultiAccount]
AutoStartAll = Yes
```

⚠️ Cẩn thận: Điều này sẽ start TẤT CẢ enabled accounts!

---

## ⚠️ Limitations

### Current Limitations

1. **Max 5 accounts** - Có thể tăng nhưng ảnh hưởng performance
2. **Single DLL** - Tất cả accounts dùng chung DLL
3. **No sync** - Mỗi account độc lập hoàn toàn
4. **Manual login** - Phải login game thủ công

### Future Features

- [ ] Auto-login support
- [ ] Account scheduler
- [ ] Cloud sync profiles
- [ ] Mobile monitoring app

---

## 🐛 Troubleshooting

### Problem: Không detect được game

**Solution:**
```ini
[ProcessManagement]
AutoDetectInterval = 2000    # Giảm xuống 2s
DetectionMethod = Both       # Dùng cả ProcessName và WindowTitle
```

### Problem: Inject fail

**Solution:**
```ini
[ProcessManagement.Injection]
InjectionDelay = 5000        # Tăng delay
RetryCount = 5               # Tăng số lần retry
```

### Problem: UI lag với nhiều accounts

**Solution:**
```ini
[Statistics]
StatsUpdateInterval = 2000   # Update chậm hơn (2s)

[MultiAccount.AccountCard]
CompactMode = Yes            # Dùng compact mode
```

### Problem: Quá nhiều accounts, không nhìn hết

**Solution:**
```ini
[MultiAccount.Display]
DisplayMode = List           # Dùng list mode thay vì cards
ShowActiveOnly = Yes         # Chỉ hiện accounts đang active
```

---

## 📊 Complete Example

File `LAUNCHER_DESIGN.txt` hoàn chỉnh cho 3 accounts:

```ini
# ... (các sections khác) ...

[MultiAccount]
EnableMultiAccount = Yes
MaxAccounts = 5
AutoStartAll = No

[MultiAccount.Display]
DisplayMode = Cards
CardsPerRow = 2
ColorRunning = #00FF00
ColorPaused = #FFAA00
ColorError = #FF0000

[MultiAccount.AccountCard]
ShowNickname = Yes
ShowCharacterName = Yes
ShowLevel = Yes
ShowHP = Yes
ShowMP = Yes
ShowStats = Yes
CompactMode = No

[Account.1]
Enabled = Yes
Nickname = Main Tank
Username = tank@email.com
DefaultMode = Farming

[Account.2]
Enabled = Yes
Nickname = DPS Alt
Username = dps@email.com
DefaultMode = Combat

[Account.3]
Enabled = Yes
Nickname = Support
Username = support@email.com
DefaultMode = Questing

[ProcessManagement]
DetectionMethod = WindowTitle
ProcessName = JX1.exe
AllowMultipleInstances = Yes
MaxInstances = 5

[ProcessManagement.Injection]
InjectionDelay = 2000
RetryOnFail = Yes
RetryCount = 3

[Statistics]
ShowTotalKills = Yes
ShowTotalGold = Yes
StatsUpdateInterval = 1000
EnableStatsExport = Yes
```

---

## ✅ Checklist

Trước khi sử dụng, check list này:

- [ ] Đã enable `EnableMultiAccount = Yes`
- [ ] Đã add ít nhất 2 accounts
- [ ] Đã set `MaxAccounts` phù hợp
- [ ] Đã config `DisplayMode` (Cards/List)
- [ ] Đã set `AllowMultipleInstances = Yes`
- [ ] Đã test với 1 account trước
- [ ] Đã apply design: `.\apply_design.ps1`
- [ ] Đã build: `dotnet build`

---

## 🚀 Next Steps

1. ✅ **Setup multi-account** (guide này)
2. ⏭️ **Implement UI components** (developer task)
3. ⏭️ **Add process detection** (developer task)
4. ⏭️ **Test với 2-3 accounts**
5. ⏭️ **Production deployment**

---

## 📖 Related Docs

- **Full Design**: [MULTI_ACCOUNT_DESIGN.md](MULTI_ACCOUNT_DESIGN.md)
- **Design File**: [LAUNCHER_DESIGN.txt](LAUNCHER_DESIGN.txt)
- **Apply Tool**: [apply_design.ps1](apply_design.ps1)

---

**Total Setup Time:** 10 phút

**Difficulty:** Easy (chỉ cần edit text file!)

**Result:** Launcher hỗ trợ 3-5 accounts! 🎉

