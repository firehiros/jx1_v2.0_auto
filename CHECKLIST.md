# JX1 V2.0 Auto - Development Checklist

## Phase 0: Setup & Planning

### Project Setup
- [ ] Chọn ngôn ngữ lập trình (Python/Node.js/C#)
- [ ] Quyết định framework UI (Electron/PyQt/WPF)
- [ ] Tạo cấu trúc thư mục dự án
- [ ] Setup Git repository và .gitignore
- [ ] Tạo file package.json/requirements.txt
- [ ] Cài đặt dependencies cơ bản
- [ ] Setup linting và code formatting (ESLint/Black)
- [ ] Tạo file .env.example cho environment variables

### Documentation
- [ ] Hoàn thiện README.md
- [ ] Tạo CONTRIBUTING.md
- [ ] Tạo CODE_OF_CONDUCT.md
- [ ] Tạo ARCHITECTURE.md (kiến trúc hệ thống)
- [ ] Tạo API_DOCS.md (nếu có API)

---

## Phase 1: Core System (Foundation)

### 1.1 Config Manager
- [ ] Thiết kế schema config (JSON/YAML)
- [ ] Implement load/save config
- [ ] Validate config structure
- [ ] Support multiple profiles
- [ ] Config hot-reload (không cần restart)
- [ ] Encrypt sensitive data trong config
- [ ] Write unit tests

**Files cần tạo:**
- `src/core/config/ConfigManager.js`
- `src/core/config/ConfigSchema.js`
- `src/core/config/ConfigValidator.js`
- `config/default.json`
- `tests/config.test.js`

### 1.2 Logger System
- [ ] Implement log levels (DEBUG, INFO, WARN, ERROR)
- [ ] Log to file với rotation
- [ ] Log to console với màu sắc
- [ ] Timestamp formatting
- [ ] Log filtering
- [ ] Performance logging
- [ ] Write unit tests

**Files cần tạo:**
- `src/core/logger/Logger.js`
- `src/core/logger/FileLogger.js`
- `src/core/logger/ConsoleLogger.js`
- `logs/.gitkeep`
- `tests/logger.test.js`

### 1.3 Screen Capture
- [ ] Capture toàn màn hình
- [ ] Capture window cụ thể (JX1 game window)
- [ ] Capture region (vùng HP/MP bar, skill bar...)
- [ ] Optimize performance (FPS control)
- [ ] Screenshot on demand
- [ ] Video recording (optional)
- [ ] Write unit tests

**Files cần tạo:**
- `src/core/capture/ScreenCapture.js`
- `src/core/capture/WindowFinder.js`
- `src/core/capture/RegionCapture.js`
- `tests/capture.test.js`

### 1.4 Image Recognition
- [ ] Setup OpenCV/Tesseract
- [ ] Template matching cơ bản
- [ ] OCR cho text (HP/MP numbers)
- [ ] Color detection (HP bar red, MP bar blue)
- [ ] Multi-scale template matching
- [ ] Confidence threshold tuning
- [ ] Create template database
- [ ] Write unit tests

**Files cần tạo:**
- `src/core/vision/ImageRecognition.js`
- `src/core/vision/TemplateMatch.js`
- `src/core/vision/OCREngine.js`
- `src/core/vision/ColorDetector.js`
- `assets/templates/` (folder chứa ảnh template)
- `tests/vision.test.js`

### 1.5 Input Controller
- [ ] Keyboard input simulation
- [ ] Mouse input simulation
- [ ] Coordinate mapping
- [ ] Click với random offset (human-like)
- [ ] Typing với random delay (human-like)
- [ ] Hotkey registration
- [ ] Input queue system
- [ ] Write unit tests

**Files cần tạo:**
- `src/core/input/InputController.js`
- `src/core/input/Keyboard.js`
- `src/core/input/Mouse.js`
- `src/core/input/HumanBehavior.js`
- `tests/input.test.js`

### 1.6 State Manager
- [ ] Define game states (IDLE, COMBAT, MOVING, DEAD...)
- [ ] State transition logic
- [ ] State persistence
- [ ] State event emitter
- [ ] State history tracking
- [ ] Write unit tests

**Files cần tạo:**
- `src/core/state/StateManager.js`
- `src/core/state/GameState.js`
- `src/core/state/StateTransition.js`
- `tests/state.test.js`

### 1.7 Anti-Detection
- [ ] Random delay giữa các actions
- [ ] Human-like mouse movement (Bezier curve)
- [ ] Random idle time
- [ ] Variation in skill rotation
- [ ] Pattern randomization
- [ ] Write unit tests

**Files cần tạo:**
- `src/core/antidetect/HumanSimulator.js`
- `src/core/antidetect/RandomDelay.js`
- `src/core/antidetect/MousePath.js`
- `tests/antidetect.test.js`

---

## Phase 2: Character Management

### 2.1 HP/MP Monitor
- [ ] Detect HP bar position
- [ ] Detect MP bar position
- [ ] Read HP percentage
- [ ] Read MP percentage
- [ ] Read HP/MP numbers (OCR)
- [ ] HP/MP change event emitter
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/character/HPMPMonitor.js`
- `src/features/character/BarDetector.js`
- `tests/character/hpmp.test.js`

### 2.2 Auto Heal
- [ ] HP threshold configuration
- [ ] Detect potion slots
- [ ] Use HP potion
- [ ] Use HP skill
- [ ] Priority system (potion vs skill)
- [ ] Cooldown tracking
- [ ] Emergency heal logic
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/character/AutoHeal.js`
- `src/features/character/PotionManager.js`
- `tests/character/heal.test.js`

### 2.3 Auto Mana
- [ ] MP threshold configuration
- [ ] Detect mana potion slots
- [ ] Use MP potion
- [ ] Meditation support
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/character/AutoMana.js`
- `tests/character/mana.test.js`

### 2.4 Auto Buff
- [ ] Detect buff icons
- [ ] Buff duration tracking
- [ ] Auto rebuff when expired
- [ ] Buff priority system
- [ ] Party buff support
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/character/AutoBuff.js`
- `src/features/character/BuffTracker.js`
- `tests/character/buff.test.js`

### 2.5 Equipment Monitor
- [ ] Detect equipment durability
- [ ] Low durability warning
- [ ] Auto repair trigger
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/character/EquipmentMonitor.js`
- `tests/character/equipment.test.js`

### 2.6 Status Check
- [ ] Detect status effects (poison, stun, freeze...)
- [ ] Use antidote for poison
- [ ] Emergency actions for negative status
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/character/StatusMonitor.js`
- `src/features/character/StatusHandler.js`
- `tests/character/status.test.js`

---

## Phase 3: Combat System

### 3.1 Target Selection
- [ ] Detect enemies on screen
- [ ] Target nearest enemy
- [ ] Target lowest HP enemy
- [ ] Target by priority (boss > elite > normal)
- [ ] Ignore dead targets
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/combat/TargetSelector.js`
- `src/features/combat/EnemyDetector.js`
- `tests/combat/target.test.js`

### 3.2 Auto Attack
- [ ] Basic attack command
- [ ] Attack range check
- [ ] Auto approach target if out of range
- [ ] Attack cooldown
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/combat/AutoAttack.js`
- `tests/combat/attack.test.js`

### 3.3 Skill Rotation
- [ ] Define skill combos per class
- [ ] Skill priority system
- [ ] Cooldown tracking
- [ ] Mana cost check
- [ ] Range check
- [ ] Conditional skills (HP < 50% use defensive skill)
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/combat/SkillRotation.js`
- `src/features/combat/SkillManager.js`
- `src/features/combat/CooldownTracker.js`
- `config/skills/` (configs cho từng môn phái)
- `tests/combat/skill.test.js`

### 3.4 Auto Dodge
- [ ] Detect boss skill animation
- [ ] Dodge pattern recognition
- [ ] Auto move away from danger zone
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/combat/AutoDodge.js`
- `src/features/combat/DangerDetector.js`
- `tests/combat/dodge.test.js`

### 3.5 PK Protection
- [ ] Detect PK mode
- [ ] Detect enemy players
- [ ] Auto run when attacked by player
- [ ] Safe zone detection
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/combat/PKProtection.js`
- `src/features/combat/PlayerDetector.js`
- `tests/combat/pk.test.js`

---

## Phase 4: Farming System

### 4.1 Auto Grind
- [ ] Detect monsters in range
- [ ] Kill loop logic
- [ ] XP/hour tracking
- [ ] Break condition (level up, bag full, HP low)
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/farming/AutoGrind.js`
- `src/features/farming/GrindLoop.js`
- `tests/farming/grind.test.js`

### 4.2 Loot System
- [ ] Detect item drops
- [ ] Auto pick up items
- [ ] Item quality filter (grey, white, blue, purple, gold)
- [ ] Item type filter
- [ ] Bag full check
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/farming/LootSystem.js`
- `src/features/farming/ItemDetector.js`
- `src/features/farming/ItemFilter.js`
- `tests/farming/loot.test.js`

### 4.3 Path Finding
- [ ] A* algorithm implementation
- [ ] Obstacle detection
- [ ] Waypoint system
- [ ] Stuck detection
- [ ] Auto unstuck
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/navigation/PathFinder.js`
- `src/features/navigation/AStar.js`
- `src/features/navigation/ObstacleDetector.js`
- `tests/navigation/pathfind.test.js`

### 4.4 Map Navigation
- [ ] Detect current map
- [ ] Map database
- [ ] Teleport point detection
- [ ] Cross-map navigation
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/navigation/MapNavigator.js`
- `src/features/navigation/MapDatabase.js`
- `data/maps/` (map data JSON files)
- `tests/navigation/map.test.js`

### 4.5 Auto Quest
- [ ] Detect available quests
- [ ] Quest database
- [ ] Auto accept quest
- [ ] Quest objective tracking
- [ ] Auto complete quest
- [ ] Quest reward selection
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/quest/QuestSystem.js`
- `src/features/quest/QuestDatabase.js`
- `src/features/quest/QuestTracker.js`
- `data/quests/` (quest data)
- `tests/quest/quest.test.js`

---

## Phase 5: Item Management

### 5.1 Inventory Manager
- [ ] Detect inventory slots
- [ ] Item position tracking
- [ ] Bag full detection
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/inventory/InventoryManager.js`
- `tests/inventory/inventory.test.js`

### 5.2 Auto Sort
- [ ] Sort by item type
- [ ] Sort by item quality
- [ ] Sort by item level
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/inventory/AutoSort.js`
- `tests/inventory/sort.test.js`

### 5.3 Auto Sell
- [ ] Detect NPC merchant
- [ ] Open shop dialog
- [ ] Sell junk items
- [ ] Item protection list
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/inventory/AutoSell.js`
- `src/features/inventory/MerchantFinder.js`
- `tests/inventory/sell.test.js`

### 5.4 Auto Store
- [ ] Detect storage NPC
- [ ] Open storage dialog
- [ ] Store valuable items
- [ ] Storage organization
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/inventory/AutoStore.js`
- `tests/inventory/store.test.js`

### 5.5 Auto Repair
- [ ] Detect repair NPC
- [ ] Open repair dialog
- [ ] Repair all items
- [ ] Gold check before repair
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/inventory/AutoRepair.js`
- `tests/inventory/repair.test.js`

---

## Phase 6: Party/Team Features

### 6.1 Auto Follow
- [ ] Detect party leader position
- [ ] Follow leader movement
- [ ] Keep distance
- [ ] Handle obstacles
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/party/AutoFollow.js`
- `src/features/party/LeaderTracker.js`
- `tests/party/follow.test.js`

### 6.2 Team Buff
- [ ] Detect party members
- [ ] Buff party members
- [ ] Buff priority
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/party/TeamBuff.js`
- `tests/party/teambuff.test.js`

### 6.3 Team Heal
- [ ] Detect party member HP
- [ ] Heal party members
- [ ] Heal priority (lowest HP first)
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/party/TeamHeal.js`
- `tests/party/teamheal.test.js`

---

## Phase 7: Safety Features

### 7.1 Anti-AFK
- [ ] Random movement every X minutes
- [ ] Random skill cast
- [ ] Random chat message
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/safety/AntiAFK.js`
- `tests/safety/antiafk.test.js`

### 7.2 Emergency Stop
- [ ] Hotkey for emergency stop
- [ ] GM detection (if possible)
- [ ] Whisper detection (someone PMing you)
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/safety/EmergencyStop.js`
- `src/features/safety/GMDetector.js`
- `tests/safety/emergency.test.js`

### 7.3 Auto Logout
- [ ] Logout after X hours
- [ ] Logout when HP critically low
- [ ] Logout when detected danger
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/safety/AutoLogout.js`
- `tests/safety/logout.test.js`

---

## Phase 8: UI Development

### 8.1 Main Dashboard
- [ ] Create main window
- [ ] Start/Stop buttons
- [ ] Status display
- [ ] Log viewer
- [ ] Write UI tests

**Files cần tạo:**
- `src/ui/MainWindow.js`
- `src/ui/components/Dashboard.js`
- `src/ui/styles/main.css`

### 8.2 Configuration Panel
- [ ] Create config editor UI
- [ ] Profile selector
- [ ] Save/Load buttons
- [ ] Validation UI
- [ ] Write UI tests

**Files cần tạo:**
- `src/ui/components/ConfigPanel.js`
- `src/ui/components/ProfileSelector.js`

### 8.3 Statistics Dashboard
- [ ] XP/hour chart
- [ ] Gold/hour chart
- [ ] Items looted list
- [ ] Session time
- [ ] Write UI tests

**Files cần tạo:**
- `src/ui/components/Statistics.js`
- `src/ui/components/Charts.js`

### 8.4 Hotkey Manager UI
- [ ] Hotkey binding UI
- [ ] Conflict detection
- [ ] Reset to default
- [ ] Write UI tests

**Files cần tạo:**
- `src/ui/components/HotkeyManager.js`

---

## Phase 9: Advanced Features

### 9.1 Multi-Client Support
- [ ] Detect multiple game windows
- [ ] Switch between clients
- [ ] Sync actions across clients
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/advanced/MultiClient.js`
- `src/features/advanced/ClientManager.js`
- `tests/advanced/multiclient.test.js`

### 9.2 Script Editor
- [ ] Create script language/DSL
- [ ] Script parser
- [ ] Script executor
- [ ] Script debugger
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/advanced/ScriptEditor.js`
- `src/features/advanced/ScriptParser.js`
- `src/features/advanced/ScriptExecutor.js`
- `tests/advanced/script.test.js`

### 9.3 Macro System
- [ ] Record user actions
- [ ] Save macro
- [ ] Playback macro
- [ ] Macro editor
- [ ] Write unit tests

**Files cần tạo:**
- `src/features/advanced/MacroRecorder.js`
- `src/features/advanced/MacroPlayer.js`
- `tests/advanced/macro.test.js`

---

## Phase 10: Testing & Quality Assurance

### 10.1 Unit Tests
- [ ] Core system tests (80%+ coverage)
- [ ] Feature tests (80%+ coverage)
- [ ] Integration tests
- [ ] Setup CI/CD for tests

### 10.2 Performance Testing
- [ ] CPU usage optimization
- [ ] Memory leak detection
- [ ] FPS impact testing
- [ ] Load testing

### 10.3 User Testing
- [ ] Alpha testing với 5-10 users
- [ ] Beta testing với 50+ users
- [ ] Collect feedback
- [ ] Fix critical bugs

---

## Phase 11: Documentation & Release

### 11.1 User Documentation
- [ ] Installation guide
- [ ] User manual
- [ ] FAQ
- [ ] Video tutorials
- [ ] Troubleshooting guide

### 11.2 Developer Documentation
- [ ] API documentation
- [ ] Architecture overview
- [ ] Contributing guide
- [ ] Code style guide

### 11.3 Release
- [ ] Version tagging
- [ ] Release notes
- [ ] Build installers (Windows/Mac/Linux)
- [ ] Setup update mechanism
- [ ] Launch website/landing page

---

## Continuous Tasks (Throughout Development)

- [ ] Code review mỗi PR
- [ ] Update documentation khi thay đổi code
- [ ] Refactor code định kỳ
- [ ] Performance optimization
- [ ] Security audit
- [ ] Bug fixing
- [ ] User feedback integration

---

## Estimated Timeline

| Phase | Duration | Tasks |
|-------|----------|-------|
| Phase 0: Setup | 1 week | 8 tasks |
| Phase 1: Core System | 4 weeks | 7 modules |
| Phase 2: Character Management | 2 weeks | 6 modules |
| Phase 3: Combat System | 3 weeks | 5 modules |
| Phase 4: Farming System | 3 weeks | 5 modules |
| Phase 5: Item Management | 2 weeks | 5 modules |
| Phase 6: Party Features | 1 week | 3 modules |
| Phase 7: Safety Features | 1 week | 3 modules |
| Phase 8: UI Development | 3 weeks | 4 modules |
| Phase 9: Advanced Features | 2 weeks | 3 modules |
| Phase 10: Testing & QA | 2 weeks | Multiple tasks |
| Phase 11: Documentation & Release | 1 week | Multiple tasks |
| **TOTAL** | **~25 weeks (~6 months)** | **100+ tasks** |

---

## Priority Order (MVP - Minimum Viable Product)

Nếu muốn có sản phẩm demo nhanh, ưu tiên theo thứ tự:

1. **Week 1-2**: Core System cơ bản (Config, Logger, Screen Capture, Input)
2. **Week 3**: HP/MP Monitor + Auto Heal
3. **Week 4**: Auto Attack + Target Selection
4. **Week 5**: Loot System + Auto Grind
5. **Week 6**: Basic UI Dashboard
6. **Week 7-8**: Testing & Bug fixing

→ **8 weeks để có MVP demo được**

---

**Last Updated**: 2025-10-14
**Maintainer**: Quoc Chien Kieu
