# JX1 Auto v2.0 - Complete Automation Framework

**Phần mềm tự động hóa chuyên nghiệp cho Võ Lâm Truyền Kỳ 1**

**Official Partnership Edition** - Hợp tác chính thức với nhà phát hành

---

## 🎯 Project Status: 90% Complete!

**Progress**: ✅ 9/11 phases done
**Code**: ~12,090 lines
**Docs**: ~9,500 lines
**Total**: ~21,590 lines

---

## ⚡ NEXT ACTION: Complete Phase 2!

### 🚨 CRITICAL: Phase 2 Execution Required

**12,090 dòng code đang chờ Phase 2 để hoạt động!**

**→ START HERE**: [PHASE2_START_HERE.md](PHASE2_START_HERE.md)

**Time**: 8-12 giờ
**Result**: Unlock everything! ✅

---

## 🎮 Quick Start (After Phase 2)

### Build & Run

```bash
# 1. Build Core DLL
cd core_dll/build
cmake .. -A Win32
cmake --build . --config Release

# 2. Build Launcher
cd launcher
dotnet build -c Release

# 3. Run Launcher
cd bin/Release/net7.0-windows
JX1Launcher.exe
```

### Use

1. Launch `JX1Launcher.exe`
2. Start JX1 game
3. Click "Inject DLL"
4. Press INSERT in game
5. Configure và start bot!

---

## 📖 Complete Documentation

### Phase 2 Guides (START HERE!)

- **[PHASE2_START_HERE.md](PHASE2_START_HERE.md)** ← **READ FIRST**
- **[PHASE2_QUICKSTART.md](PHASE2_QUICKSTART.md)** ← 30 min tutorial
- **[PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md)** ← Main guide (900+ lines)
- [PHASE2_QUICK_REFERENCE.md](docs/PHASE2_QUICK_REFERENCE.md) ← Cheatsheet

### Implementation Docs

- [PHASE3_COMPLETE.md](docs/PHASE3_COMPLETE.md) - Game state
- [PHASE4_COMPLETE.md](docs/PHASE4_COMPLETE.md) - Overlay
- [PHASE5_COMPLETE.md](docs/PHASE5_COMPLETE.md) - Functions
- [PHASE6_COMPLETE.md](docs/PHASE6_COMPLETE.md) - Bots
- [PHASE7_COMPLETE.md](docs/PHASE7_COMPLETE.md) - Advanced
- [PHASE8_COMPLETE.md](docs/PHASE8_COMPLETE.md) - Network
- [PHASE9_COMPLETE.md](docs/PHASE9_COMPLETE.md) - Launcher

### Project Status

- [FINAL_STATUS.md](FINAL_STATUS.md) - Current status

---

## Mục Lục
- [Giới thiệu](#giới-thiệu)
- [Danh sách chức năng](#danh-sách-chức-năng)
- [Cài đặt](#cài-đặt)
- [Sử dụng](#sử-dụng)
- [Đóng góp](#đóng-góp)

## Giới thiệu

JX1 V2.0 Auto là công cụ tự động hóa được phát triển để hỗ trợ người chơi Võ Lâm Truyền Kỳ 1. Dự án được thực hiện với sự hợp tác của nhà phát hành nhằm cải thiện trải nghiệm người chơi.

## Danh Sách Chức Năng

### 1. CORE SYSTEM (Hệ thống nền)
- [ ] **Config Manager**: Quản lý cấu hình người dùng
- [ ] **Logger**: Ghi log hoạt động, debug
- [ ] **State Manager**: Quản lý trạng thái nhân vật/game
- [ ] **Screen Capture**: Chụp màn hình game
- [ ] **Image Recognition**: Nhận diện UI, vật phẩm, quái
- [ ] **Input Controller**: Điều khiển bàn phím/chuột
- [ ] **Anti-Detection**: Mô phỏng hành vi người chơi thật

### 2. CHARACTER MANAGEMENT (Quản lý nhân vật)
- [ ] **HP/MP Monitor**: Theo dõi máu/mana
- [ ] **Auto Buff**: Tự động đánh buff
- [ ] **Auto Heal**: Tự động hồi máu (uống thuốc/skill)
- [ ] **Auto Mana**: Tự động hồi mana
- [ ] **Equipment Monitor**: Theo dõi độ bền trang bị
- [ ] **Status Check**: Kiểm tra trạng thái (độc, choáng, etc.)

### 3. COMBAT SYSTEM (Chiến đấu)
- [ ] **Auto Attack**: Tự động đánh thường
- [ ] **Skill Rotation**: Quay vòng skill theo combo
- [ ] **Target Selection**: Chọn mục tiêu (gần nhất, HP thấp nhất, etc.)
- [ ] **Auto Dodge**: Né tránh kỹ năng boss
- [ ] **PK Protection**: Tránh PK hoặc chạy khi bị tấn công

### 4. FARMING (Cày level/đồ)
- [ ] **Auto Quest**: Tự động nhận/nộp nhiệm vụ
- [ ] **Auto Grind**: Tự động đánh quái cày level
- [ ] **Map Navigation**: Di chuyển giữa các map
- [ ] **Path Finding**: Tìm đường tránh vật cản
- [ ] **Loot System**: Nhặt đồ rơi
- [ ] **Item Filter**: Lọc đồ nhặt (theo quality, type)

### 5. ITEM MANAGEMENT (Quản lý vật phẩm)
- [ ] **Auto Sort**: Tự động sắp xếp túi đồ
- [ ] **Auto Sell**: Tự động bán đồ rác
- [ ] **Auto Store**: Tự động cất kho đồ quý
- [ ] **Item Protection**: Bảo vệ đồ quan trọng không bán nhầm
- [ ] **Crafting Assistant**: Hỗ trợ chế tạo

### 6. PARTY/TEAM (Nhóm đội)
- [ ] **Auto Follow**: Tự động đi theo đội trưởng
- [ ] **Team Buff**: Buff cho đồng đội
- [ ] **Team Heal**: Hồi máu đồng đội
- [ ] **Raid Coordination**: Phối hợp đánh boss nhóm

### 7. ECONOMY (Kinh tế)
- [ ] **Auto Trade**: Tự động buôn bán
- [ ] **Market Scanner**: Quét giá chợ
- [ ] **Auto Repair**: Tự động sửa đồ

### 8. EVENTS (Sự kiện)
- [ ] **Event Detector**: Phát hiện sự kiện đặc biệt
- [ ] **Boss Timer**: Đếm giờ boss xuất hiện
- [ ] **Auto Join Event**: Tự động tham gia event

### 9. SAFETY & ANTI-AFK (An toàn)
- [ ] **Anti-AFK**: Chống bị kick do AFK
- [ ] **Random Movement**: Di chuyển ngẫu nhiên
- [ ] **Chat Bot**: Tự động trả lời chat (tránh bị nghi ngờ)
- [ ] **Emergency Stop**: Dừng khẩn cấp khi phát hiện GM
- [ ] **Auto Logout**: Tự động thoát khi hết thời gian/gặp nguy hiểm

### 10. UI/UX (Giao diện)
- [ ] **Dashboard**: Bảng điều khiển trực quan
- [ ] **Profile Management**: Quản lý nhiều profile khác nhau
- [ ] **Hotkey System**: Phím tắt điều khiển
- [ ] **Statistics**: Thống kê exp/giờ, gold/giờ, items
- [ ] **Notifications**: Thông báo sự kiện quan trọng

### 11. ADVANCED FEATURES (Nâng cao)
- [ ] **Multi-Client Support**: Hỗ trợ nhiều client cùng lúc
- [ ] **Script Editor**: Cho phép user tự viết script
- [ ] **Macro System**: Ghi lại và phát lại hành động
- [ ] **AI Learning**: Học từ hành vi người chơi
- [ ] **Cloud Sync**: Đồng bộ config qua cloud

## 📚 Documentation

**➡️ [Full Documentation](docs/README.md)**

### Quick Links

- **🚀 [Quick Start Guide](docs/00-getting-started/QUICK_START.md)** - Bắt đầu trong 5 phút
- **🔨 [Build Instructions](docs/01-build/BUILD_INSTRUCTIONS.md)** - Hướng dẫn build project
- **🏗️ [Architecture](docs/02-architecture/ARCHITECTURE_HOOK.md)** - Kiến trúc hệ thống
- **🔍 [Cheat Engine Guide](docs/03-reverse-engineering/CHEAT_ENGINE_GUIDE.md)** - Phase 2 RE guide
- **📋 [Development Checklist](docs/04-development/CHECKLIST.md)** - Track progress

### Documentation Structure

```
docs/
├── 00-getting-started/     # Setup & quick start
├── 01-build/               # Build instructions
├── 02-architecture/        # System design
├── 03-reverse-engineering/ # Cheat Engine & Phase 2
├── 04-development/         # Development workflow
└── 05-research/            # Research notes
```

## Cài Đặt

### Prerequisites
- Visual Studio 2022 (với C++ workload)
- DirectX SDK (June 2010)
- Git
- JX1 Game client

### Quick Setup
```bash
# 1. Clone repository
git clone <repository-url>
cd jx1_v2.0_auto

# 2. Download third-party libraries
# - MinHook → core_dll/third_party/minhook/
# - ImGui → core_dll/third_party/imgui/
# - nlohmann/json → core_dll/third_party/json/

# 3. Build
scripts\build.bat Debug
scripts\build_injector.bat

# 4. Inject vào game
cd bin
SimpleInjector.exe JX1.exe JX1AutoCore.dll
```

**Chi tiết:** [docs/00-getting-started/QUICK_START.md](docs/00-getting-started/QUICK_START.md)

## Sử Dụng

```bash
# Build core DLL
scripts\build.bat Debug

# Chạy game JX1
# ...

# Inject DLL
cd bin
SimpleInjector.exe JX1.exe JX1AutoCore.dll

# Press END key để shutdown
```

**Hướng dẫn chi tiết:** [docs/](docs/)

## Công Nghệ Sử Dụng

**Core DLL (Injected Component)**:
- C++17/20 (High performance)
- MinHook (Function hooking)
- ImGui (Overlay rendering)
- DirectX 9/11 (Graphics hook)
- nlohmann/json (Configuration)

**External Launcher**:
- Electron/WPF/Qt (Modern UI)
- React/Vue (Dashboard)
- Node.js/C# (Backend logic)

## 📊 Current Status

**Current Phase:** Phase 2 - Reverse Engineering
**Progress:** ~20% complete

```
✅ Phase 1: Core Infrastructure (COMPLETE)
   - DLL injection, Logger, MemoryManager, HookManager

⏳ Phase 2: Reverse Engineering (CURRENT)
   - Finding memory offsets with Cheat Engine
   - See: docs/03-reverse-engineering/

🔜 Phase 3: Game State Reading
🔜 Phase 4: DirectX Hook & Overlay
🔜 Phase 5-8: See docs/04-development/CHECKLIST.md
```

**Full roadmap:** [docs/04-development/CHECKLIST.md](docs/04-development/CHECKLIST.md)

## Roadmap

### ✅ Phase 1: Core Infrastructure (COMPLETE)
- [x] DLL injection architecture
- [x] Memory manager & Hook manager
- [x] Logger & Configuration system
- [x] Pattern scanning & Offset management

### ⏳ Phase 2: Reverse Engineering (CURRENT - 2-4 hours)
- [ ] Find Player offsets (HP, MP, Position, Level)
- [ ] Find Entity/Monster structures
- [ ] Find Function patterns (Attack, Move, UseItem)
- [ ] Document in Offsets.h & Patterns.h

**Guide:** [Cheat Engine Guide](docs/03-reverse-engineering/CHEAT_ENGINE_GUIDE.md)

### 🔜 Phase 3: Game State Reading
- [ ] Implement Player class reading
- [ ] Implement Entity/EntityManager
- [ ] Real-time data reading

### 🔜 Phase 4: DirectX Hook & Overlay
- [ ] Hook DirectX Present()
- [ ] Setup ImGui rendering
- [ ] Implement ESP & Overlay

### 🔜 Phase 5: Bot Logic
- [ ] Combat bot (auto-attack, skills)
- [ ] Farming bot (grinding, XP)
- [ ] Loot bot (item filter)

### 🔜 Phase 6: Network & Advanced Hooks
- [ ] Packet interception
- [ ] Function hooks

### 🔜 Phase 7: External GUI
- [ ] WPF Launcher
- [ ] Configuration UI
- [ ] Statistics dashboard

### 🔜 Phase 8: Polish & Multi-Client
- [ ] Multi-client support
- [ ] Optimization
- [ ] Testing & Release

## Đóng Góp

Dự án đang trong giai đoạn phát triển. Mọi đóng góp đều được hoan nghênh.

## License

[Chưa xác định - Dự án hợp tác với nhà phát hành]

## Liên Hệ

- Developer: Quoc Chien Kieu
- Email: [Cập nhật sau]

---

**Lưu ý**: Đây là dự án hợp tác chính thức với nhà phát hành game Võ Lâm Truyền Kỳ nhằm mục đích hỗ trợ trải nghiệm người chơi.
