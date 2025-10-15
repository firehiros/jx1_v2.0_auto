# JX1 Auto v2.0 - Hướng Dẫn Sử Dụng

**Phiên bản:** 2.0.0
**Ngôn ngữ:** Tiếng Việt
**Cập nhật:** 2025-10-15

---

## 📖 Mục Lục

1. [Giới Thiệu](#intro)
2. [Cài Đặt Ban Đầu](#setup)
3. [Tìm Memory Offsets](#scan)
4. [Sử Dụng Launcher](#launcher)
5. [Quản Lý Nhiều Tài Khoản](#multi-account)
6. [Cấu Hình Bot](#config)
7. [Sử Dụng Bot Trong Game](#ingame)
8. [Câu Hỏi Thường Gặp](#faq)

---

## 🎯 Giới Thiệu {#intro}

### JX1 Auto v2.0 Là Gì?

**JX1 Auto v2.0** là tool tự động hóa cho game Võ Lâm Truyền Kỳ 1, giúp bạn:

✅ **Tự động đánh quái** (Auto Combat)
✅ **Tự động nhặt đồ** (Auto Loot)
✅ **Tự động farm vàng** (Auto Farming)
✅ **Quản lý nhiều tài khoản** (Multi-Account)
✅ **Hiển thị ESP** (thấy quái, người chơi, items qua tường)
✅ **Thống kê chi tiết** (kills, gold, items)

### Thành Phần Chính

**1. Launcher (Ứng dụng ngoài game)**
- Quản lý tài khoản
- Cấu hình settings
- Xem thống kê
- Điều khiển bot

**2. Core DLL (Bot trong game)**
- Tự động combat
- ESP overlay
- Menu cài đặt
- Bot logic

**3. Auto Scanner (Tool scan memory)**
- Tự động tìm offsets
- Không cần kiến thức technical
- Chỉ cần nhập giá trị HP, MP

---

## 🚀 Cài Đặt Ban Đầu {#setup}

### Bước 1: Chuẩn Bị

**Yêu cầu:**
- ✅ Windows 10/11
- ✅ .NET 7 Runtime ([Download](https://dotnet.microsoft.com/download))
- ✅ Visual Studio 2022 hoặc Build Tools
- ✅ Game Võ Lâm Truyền Kỳ 1 đã cài đặt

**Kiểm tra:**
```powershell
# Check .NET version
dotnet --version
# Kết quả mong đợi: 7.0.x
```

### Bước 2: Download Project

```bash
# Clone repository
git clone <repository-url>
cd jx1_v2.0_auto
```

### Bước 3: Build Launcher

```powershell
cd launcher
dotnet build
```

**Kết quả:** `Build succeeded` ✅

### Bước 4: Test Launcher

```powershell
dotnet run
```

**Kết quả:** Launcher window mở ra ✅

---

## 🔍 Tìm Memory Offsets {#scan}

### Tại Sao Cần Scan?

Bot cần biết **địa chỉ memory** của HP, MP, Level, Gold để đọc và điều khiển game.

**Chỉ cần làm 1 lần!** Kết quả lưu vào file config, sau đó dùng mãi mãi.

### Bước 1: Chuẩn Bị

```
1. ✅ Mở game JX1
2. ✅ Login vào nhân vật
3. ✅ Nhìn thấy HP, MP hiện tại
```

### Bước 2: Chạy Auto Scanner

```powershell
cd tools
auto_scan.bat
```

**Hoặc:**
```powershell
.\AutoScanMemory_v2.ps1
```

### Bước 3: Scan HP

**Menu hiện ra:**
```
============================================
   JX1 Auto Memory Scanner v2.0
============================================

1) Scan for HP (Auto mode)
2) Scan for MP (Auto mode)
3) Scan for Level
4) Scan for Gold/Money
5) Batch Scan All ← Khuyên dùng cái này!
6) View Current Config
7) Save Config
8) Exit
```

**Chọn 1 (hoặc 5 để scan tất cả):**

```
Enter your choice: 1

[Step 1] Enter your CURRENT HP value: 1500
Scanning memory...
Found 5000 addresses

[Step 2] Change your HP (take damage or heal)
Enter your NEW HP value: 1200
Filtering...
Remaining: 50 addresses

[Step 3] Change HP again
Enter your NEW HP value: 1000
Remaining: 3 addresses

SUCCESS! Found HP address(es):
  0x12AB5678

Analyzing memory region...
Offset  | Value    | Guess
--------|----------|------------
+0x10   | 1000     | ✅ HP (confirmed!)
+0x14   | 3600     | ✅ Max HP (auto-detected!)

Is the first address correct? (y/n): y

✅ Config updated!
```

### Bước 4: Scan Các Giá Trị Khác

**Lặp lại với MP, Level, Gold:**
- Chọn option 2, 3, 4
- Hoặc dùng option 5 (Batch Scan All) để scan tất cả liên tục

### Bước 5: Save Config

```
Menu:
7) Save Config

Config saved to: ../config/memory_config.json
```

### Bước 6: Validate

```powershell
.\validate_config.ps1
```

**Kết quả:**
```
✅ All checks passed!
Configuration Readiness: 5/17 verified (29%)

Recommendations:
  • 12 more offsets needed (optional)
  • Core offsets (HP, MP) ready ✅
```

**Nếu có HP, MP, Position → Đủ để chạy bot cơ bản!**

---

## 💻 Sử Dụng Launcher {#launcher}

### Khởi Động Launcher

**Cách 1: PowerShell**
```powershell
cd launcher
dotnet run
```

**Cách 2: Build executable**
```powershell
dotnet publish -c Release
.\bin\Release\net7.0\JX1Launcher.exe
```

**Cách 3: Quick build script**
```batch
cd launcher
build_and_run.bat
```

### Giao Diện Launcher

```
╔═══════════════════════════════════════════════════════════╗
║ JX1 Auto v2.0 - Multi-Account Launcher                   ║
╠═══════════════════════════════════════════════════════════╣
║ [Accounts] [Legacy Mode] [Settings] [Profiles] [About]   ║
╠═══════════════════════════════════════════════════════════╣
║                                                            ║
║  (Nội dung tab)                                           ║
║                                                            ║
╠═══════════════════════════════════════════════════════════╣
║ Status message                                    v2.0.0  ║
╚═══════════════════════════════════════════════════════════╝
```

### Tab "Accounts" (Tab chính)

**Tại đây bạn:**
- Thêm/xóa accounts
- Start/Stop từng account
- Xem stats real-time
- Control từng bot

### Tab "Settings"

**Cấu hình:**
- Combat settings (khoảng cách đánh, heal threshold)
- ESP settings (hiện quái, người chơi)
- Loot settings (nhặt đồ gì, chất lượng tối thiểu)

### Tab "Profiles"

**Quản lý profiles:**
- Save nhiều bộ settings
- Quick-switch giữa các profiles
- Ví dụ: Profile "Farming", Profile "PVP", Profile "Leveling"

---

## 👥 Quản Lý Nhiều Tài Khoản {#multi-account}

### Thêm Tài Khoản

**Cách 1: Qua UI**
1. Click **[➕ Add Account]**
2. Account mới xuất hiện
3. Click **[✎ Edit]** để chỉnh sửa

**Cách 2: Edit File JSON**

Mở `launcher/accounts.json`:
```json
[
  {
    "Id": 1,
    "Nickname": "Nhân Vật Chính",
    "Username": "email1@gmail.com",
    "CharacterName": "LongNV_001",
    "Level": 99,
    "Enabled": true,
    "BotMode": "Farming"
  },
  {
    "Id": 2,
    "Nickname": "Nhân Vật Phụ",
    "Username": "email2@gmail.com",
    "CharacterName": "LongNV_002",
    "Level": 85,
    "Enabled": true,
    "BotMode": "Combat"
  }
]
```

Save file và click **[🔄 Refresh]** trong launcher.

### Start Một Tài Khoản

**Các bước:**
1. Mở game và login vào tài khoản
2. Trong launcher, tìm account tương ứng
3. Click **[▶ Start]** bên cạnh account
4. Đợi injection hoàn tất
5. Account card chuyển sang "Active Accounts"
6. Bot bắt đầu chạy!

**Chú ý:**
- Game phải đang chạy trước
- Nhân vật phải đã login
- DLL sẽ tự động inject

### Start Tất Cả Accounts

1. Mở tất cả game instances
2. Login tất cả accounts
3. Click **[▶ Start All]** trong launcher
4. Tất cả accounts start lần lượt (delay 2s giữa mỗi account)

**Kết quả:**
- Tất cả bots chạy song song
- Mỗi account độc lập
- Stats tổng hợp ở footer

### Pause/Resume Account

**Pause:**
1. Click **[⏸]** trên account card
2. Bot tạm dừng
3. Card chuyển màu vàng
4. DLL vẫn inject (có thể resume)

**Resume:**
1. Click **[⏸]** lần nữa
2. Bot tiếp tục
3. Card chuyển màu xanh

### Stop Account

1. Click **[⏹]** trên account card
2. Bot dừng hoàn toàn
3. DLL eject khỏi game
4. Account về "Available Accounts"

### Xem Chi Tiết Account

Click **[📊]** để xem:
- Chi tiết stats
- Session history
- Error logs
- Performance metrics

---

## ⚙️ Cấu Hình Bot {#config}

### Cấu Hình Combat

**Tab Settings → Combat:**

1. **Combat Range** - Khoảng cách tấn công
   - Slider: 100-1000 units
   - Khuyên dùng: 500 units
   - Càng xa càng an toàn, nhưng di chuyển nhiều

2. **Heal Threshold** - Heal khi HP xuống bao nhiêu %
   - Slider: 10-80%
   - Khuyên dùng: 40%
   - Thấp hơn = ít waste potion, cao hơn = an toàn

3. **Flee Threshold** - Chạy khi HP xuống bao nhiêu %
   - Slider: 5-50%
   - Khuyên dùng: 15%
   - Tránh chết

### Cấu Hình Loot

**Tab Settings → Loot:**

1. **Minimum Quality** - Chất lượng tối thiểu
   - Grey (Trash) - Nhặt tất cả
   - White (Common) - Khuyên dùng
   - Blue (Rare) - Chỉ đồ xanh trở lên
   - Purple/Gold - Chỉ đồ epic+

2. **Item Types** - Loại đồ nhặt
   - ☑ Weapons - Vũ khí
   - ☑ Armor - Áo giáp
   - ☑ Potions - Thuốc
   - ☑ Materials - Nguyên liệu

### Cấu Hình ESP

**Tab Settings → ESP:**

1. **Show Options** - Hiển thị gì
   - ☑ Monsters - Quái
   - ☑ Players - Người chơi khác
   - ☑ NPCs - NPC
   - ☑ Items - Đồ rơi

2. **Display Options** - Cách hiển thị
   - ☑ Boxes - Hộp xung quanh
   - ☑ Names - Tên
   - ☑ HP Bars - Thanh máu
   - ☑ Distance - Khoảng cách

3. **Colors** - Màu sắc
   - Click vào color box để đổi màu
   - Mỗi loại entity có màu riêng

### Lưu Cấu Hình

**Sau khi chỉnh xong:**
1. Click **[Save Settings]**
2. Settings lưu vào file
3. Áp dụng cho tất cả accounts (hoặc theo account nếu config riêng)

---

## 🎮 Sử Dụng Bot Trong Game {#ingame}

### Khởi Động Bot

**Các bước:**
1. Start game JX1
2. Login vào nhân vật
3. Mở launcher
4. Click **[▶ Start]** cho account
5. Đợi DLL inject thành công
6. **Nhấn INSERT** trong game để mở menu

### Menu Trong Game (INSERT)

**Menu hiện ra:**
```
╔════════════════════════════════════╗
║ JX1 Auto v2.0 - Settings          ║
╠════════════════════════════════════╣
║ [ESP] [Bot] [Debug] [About]       ║
╠════════════════════════════════════╣
║  (Settings của tab hiện tại)      ║
╚════════════════════════════════════╝
```

### Tab ESP (Trong Game Menu)

**Bật/tắt ESP:**
- ☑ Show Monsters
- ☑ Show Players
- ☑ Show Items

**Khi bật, bạn sẽ thấy:**
- Hộp màu xung quanh quái
- Tên quái trên đầu
- Thanh máu
- Khoảng cách

### Tab Bot (Trong Game Menu)

**Control bot:**
- ☑ Enable Auto Combat
- ☑ Enable Auto Loot
- ☑ Enable Auto Heal

**Chú ý:** Tab này hiện chỉ show preview. Full features sẽ có sau khi scan xong offsets.

### Tắt Bot

**Cách 1: Từ launcher**
- Click **[⏹ Stop]** trên account card

**Cách 2: Từ game**
- Nhấn **END** key
- DLL tự eject

**Cách 3: Emergency**
- Close game
- DLL tự động eject

---

## 🎯 Workflow Điển Hình

### Workflow 1: Single Account Farming

**Mục tiêu:** Farm vàng với 1 account

**Các bước:**
1. ✅ Scan memory offsets (1 lần duy nhất)
2. ✅ Build core DLL
3. ✅ Start game và login
4. ✅ Mở launcher → Add account
5. ✅ Click Start account
6. ✅ Config farming route
7. ✅ Để bot tự chạy
8. ✅ Check stats sau vài giờ

**Kết quả:** Bot farm tự động, bạn AFK!

### Workflow 2: Multi-Account Gold Farming

**Mục tiêu:** Farm vàng với 3 accounts cùng lúc

**Các bước:**
1. ✅ Chuẩn bị 3 accounts game
2. ✅ Mở 3 game windows (3 instances)
3. ✅ Login 3 accounts
4. ✅ Trong launcher:
   - Add Account 1: "Main Farmer"
   - Add Account 2: "Alt Farmer"
   - Add Account 3: "Support"
5. ✅ Click **[▶ Start All]**
6. ✅ Tất cả 3 bots chạy
7. ✅ Monitor stats tổng hợp

**Kết quả:**
- 3x farming efficiency
- Tổng gold = sum of all accounts
- Independent control mỗi account

### Workflow 3: AFK Leveling

**Mục tiêu:** Tăng cấp khi AFK

**Các bước:**
1. ✅ Tìm spot có quái phù hợp với level
2. ✅ Config bot mode: Combat
3. ✅ Set heal threshold: 50%
4. ✅ Enable auto-combat
5. ✅ Start bot
6. ✅ AFK

**Kết quả:** Nhân vật tự động đánh quái, tăng cấp!

---

## ❓ Câu Hỏi Thường Gặp {#faq}

### Q1: Có bị ban không?

**A:** Tool này được thiết kế an toàn với:
- Memory reading chủ yếu (ít risk)
- Anti-detection features
- Human-like behavior patterns

**Khuyến nghị:**
- Dùng với moderation
- Không abuse
- Tuân thủ ToS của game

### Q2: Phải scan memory mỗi khi mở game?

**A:** KHÔNG! Chỉ cần scan **1 lần duy nhất**.

Kết quả lưu trong `memory_config.json`, dùng mãi mãi.

**Chỉ cần scan lại khi:**
- Game update version mới
- Offsets thay đổi (hiếm khi)

### Q3: Có thể dùng bao nhiêu accounts?

**A:** Tối đa **5 accounts** cùng lúc (configurable).

**Lý do:**
- Performance (CPU, RAM)
- Stable operation
- Easy monitoring

**Nếu muốn nhiều hơn:**
Edit `LAUNCHER_DESIGN.txt`:
```ini
[MultiAccount]
MaxAccounts = 10
```

### Q4: Bot có tự động login không?

**A:** Chưa (Phase 6 - future feature).

**Hiện tại:** Phải login thủ công trước khi start bot.

**Workaround:** Dùng AutoHotkey script riêng để auto-login.

### Q5: Làm sao customize giao diện launcher?

**A:** Edit file text!

```powershell
# Edit
notepad launcher/LAUNCHER_DESIGN.txt

# Change colors, sizes, text

# Preview
cd launcher
.\preview_design.ps1

# Apply
.\apply_design.ps1

# Build
dotnet build
dotnet run
```

### Q6: Scan không ra kết quả?

**A:** Thử các cách sau:
1. Run as Administrator
2. Thay đổi giá trị nhiều hơn (HP: 1500 → 1000 thay vì 1500 → 1490)
3. Đợi vài giây giữa các lần scan
4. Scan nhiều lần hơn để lọc

### Q7: DLL inject fail?

**A:** Check:
1. ✅ Game đang chạy?
2. ✅ DLL đã build? (`core_dll/build/Release/core_dll.dll`)
3. ✅ Antivirus có block không?
4. ✅ Run launcher as Administrator?

### Q8: ESP không hiện trong game?

**A:** Check:
1. ✅ Offsets đã scan chưa?
2. ✅ DLL đã inject chưa?
3. ✅ ESP enabled trong menu? (Nhấn INSERT → ESP tab)
4. ✅ DirectX 9 hook working? (Check logs)

### Q9: Stats không cập nhật?

**A:** Stats cập nhật qua 2 cách:
1. **From memory** - HP, MP, Position (real-time)
2. **From bot** - Kills, Items, Gold (event-driven)

Nếu không update:
- Check offsets configured
- Check IPC working (future feature)

### Q10: Làm sao export stats?

**A:**
**Cách 1: Auto-export**
```ini
# LAUNCHER_DESIGN.txt
[Statistics]
EnableStatsExport = Yes
ExportInterval = 300000  # 5 minutes
```

**Cách 2: Manual**
```
Click account → [📊 Details] → [Export] button
```

File save ở: `logs/stats_2025-01-15.csv`

---

## 🎓 Tips & Tricks

### Tip 1: Dùng Batch Scan

**Thay vì scan từng cái:**
```
Tools → AutoScanMemory → Option 5 (Batch Scan All)
```

Scan HP → MP → Level → Gold liên tục trong 1 session!

### Tip 2: Backup Config

**Sau khi scan xong:**
```powershell
# Backup config file
Copy-Item config/memory_config.json config/memory_config.backup.json

# Hoặc commit vào git
git add config/memory_config.json
git commit -m "Add memory offsets for version X.X.X"
```

### Tip 3: Test với 1 Account Trước

**Đừng vội start 5 accounts!**

Test kỹ với 1 account trước:
- Start → OK?
- ESP hiện → OK?
- Stats update → OK?
- Sau đó mới scale lên nhiều accounts

### Tip 4: Monitor Resource Usage

**Task Manager:**
- CPU usage per game
- RAM usage per game
- Nếu quá cao → Giảm số accounts

**Khuyên dùng:**
- 2-3 accounts cho PC thường
- 4-5 accounts cho PC mạnh

### Tip 5: Sử Dụng Profiles

**Tạo profiles cho các tình huống:**
- Profile "Farming" - Heal 40%, Auto loot
- Profile "PVP" - Heal 60%, Aggressive
- Profile "Leveling" - Heal 50%, Follow route

Quick-switch khi cần!

### Tip 6: Custom Hotkeys

**Edit LAUNCHER_DESIGN.txt:**
```ini
[Hotkeys]
ToggleMenu = INSERT
QuickInject = F1
EmergencyStop = F12
```

### Tip 7: Themes

**Thử các themes:**
```powershell
# Dark red gaming theme
Copy-Item launcher/themes/dark_red_theme.txt launcher/LAUNCHER_DESIGN.txt

# Light blue professional
Copy-Item launcher/themes/light_blue_theme.txt launcher/LAUNCHER_DESIGN.txt

# Apply
cd launcher
.\apply_design.ps1
```

---

## 🆘 Troubleshooting

### Problem: Launcher không mở

**Check:**
```powershell
# Check .NET installed
dotnet --version

# Reinstall .NET 7 if needed
# Download from: https://dotnet.microsoft.com
```

### Problem: Build fail

**Check:**
```powershell
# Clean and rebuild
dotnet clean
dotnet build --verbosity detailed

# Check error messages
# Install missing packages if needed
```

### Problem: Không detect được game

**Check LAUNCHER_DESIGN.txt:**
```ini
[ProcessManagement]
ProcessName = JX1.exe           # Đúng tên process?
WindowTitlePattern = Võ Lâm     # Đúng window title?
```

### Problem: Multi-account lag

**Reduce load:**
```ini
[Statistics]
StatsUpdateInterval = 2000  # Update chậm hơn (2s thay vì 1s)

[MultiAccount.AccountCard]
CompactMode = Yes            # Dùng compact mode
```

### Problem: Account card không hiện stats

**Check:**
1. Offsets đã scan?
2. DLL injected?
3. IPC working? (future feature)

**Workaround:** Stats sẽ update sau khi implement IPC.

---

## 📚 Tài Liệu Liên Quan

### Cho Người Dùng Mới:
- [README.md](../README.md) - Tổng quan project
- [Quick Start](00-getting-started/QUICK_START.md) - 5 phút bắt đầu
- [Build Guide](01-build/BUILD_INSTRUCTIONS.md) - Hướng dẫn build

### Cho Reverse Engineer:
- [Cheat Engine Guide](03-reverse-engineering/CHEAT_ENGINE_GUIDE.md) - Hướng dẫn CE
- [Auto Scanner](../tools/README.md) - Auto-scan tool
- [Config Guide](03-reverse-engineering/CENTRALIZED_CONFIG_GUIDE.md) - Config system

### Cho Developer:
- [Architecture](02-architecture/ARCHITECTURE_HOOK.md) - Kiến trúc hệ thống
- [Features Doc](FEATURES_DOCUMENTATION.md) - Chi tiết features
- [Multi-Account](../launcher/MULTI_ACCOUNT_DESIGN.md) - Multi-account design

---

## 🎉 Kết Luận

**JX1 Auto v2.0** là tool mạnh mẽ với:
- ✅ 25+ features
- ✅ Multi-account support (up to 5)
- ✅ Auto memory scanning
- ✅ Professional UI
- ✅ Comprehensive documentation
- ✅ Easy to use và customize

**Bắt đầu ngay:**
```batch
1. cd tools && auto_scan.bat          (Scan memory)
2. cd ..\scripts && build.bat         (Build DLL)
3. cd ..\launcher && dotnet run       (Run launcher)
4. Add accounts → Start → Enjoy!      (Use bot)
```

**Thời gian setup:** ~30 phút (bao gồm scan memory)

**Sau đó:** Tự động hóa hoàn toàn! 🎮✨

---

**Need help?** Check [DOCUMENTATION_INDEX.md](../DOCUMENTATION_INDEX.md) for complete docs!

**Version:** 2.0.0
**Support:** See documentation files
**License:** For educational purposes only

