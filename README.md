# JX1 V2.0 Auto - Võ Lâm Truyền Kỳ Auto Tool

Phần mềm hỗ trợ người chơi Võ Lâm Truyền Kỳ 1 - Dự án hợp tác với nhà phát hành.

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

## Cài Đặt

```bash
# Clone repository
git clone <repository-url>
cd jx1_v2.0_auto

# Cài đặt dependencies (sẽ cập nhật sau)
# npm install hoặc pip install -r requirements.txt
```

## Sử Dụng

```bash
# Hướng dẫn sử dụng sẽ được cập nhật
```

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

## Roadmap

### Phase 1: Core Infrastructure (Week 1-3)
- DLL injection architecture
- Memory manager & Hook manager
- Logger & Configuration system
- Pattern scanning & Offset management

### Phase 2: Reverse Engineering (Week 4-5)
- Analyze JX1 memory structure
- Find player/entity structures
- Discover critical functions
- Document offsets and patterns

### Phase 3: Game State & Hooks (Week 6-9)
- Implement game state reading (Player, Entity, Inventory)
- DirectX hook & Overlay (ImGui + ESP)
- Hook game functions (Attack, Move, Skill, Item)
- Network packet interception

### Phase 4: Bot Logic (Week 10-13)
- Combat bot (auto-attack, skill rotation, heal)
- Farming bot (grinding loop, XP tracking)
- Loot bot (item filter, auto-pickup)
- Quest bot & Pathfinding

### Phase 5: External GUI & Multi-Client (Week 14-18)
- Modern launcher with Electron/WPF
- Configuration panels & Statistics dashboard
- Multi-client support (5+ instances)
- IPC communication

### Phase 6: Polish & Release (Week 19-20)
- Bug fixes & Performance optimization
- User documentation & Video tutorials
- Testing & QA
- Production release

## Đóng Góp

Dự án đang trong giai đoạn phát triển. Mọi đóng góp đều được hoan nghênh.

## License

[Chưa xác định - Dự án hợp tác với nhà phát hành]

## Liên Hệ

- Developer: Quoc Chien Kieu
- Email: [Cập nhật sau]

---

**Lưu ý**: Đây là dự án hợp tác chính thức với nhà phát hành game Võ Lâm Truyền Kỳ nhằm mục đích hỗ trợ trải nghiệm người chơi.
