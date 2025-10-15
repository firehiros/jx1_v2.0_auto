# JX1 V2.0 Auto - Full Hook Development Checklist

**Approach**: DLL Injection + Game Hooking
**Official Partnership with Publisher**
**Timeline**: 20 weeks (~5 months)

---

## Phase 0: Setup & Preparation (Week 1)

### Development Environment
- [ ] Install Visual Studio 2022 (C++ workload)
- [ ] Install CMake 3.20+
- [ ] Install Git
- [ ] Setup version control (.gitignore for VS, binaries)
- [ ] Install Cheat Engine 7.5
- [ ] Install x64dbg/IDA Free
- [ ] Install HxD (Hex Editor)

### Libraries & Dependencies
- [ ] Download MinHook library
- [ ] Download ImGui (docking branch)
- [ ] Download nlohmann/json
- [ ] Download DirectX SDK (if needed for DX9)
- [ ] Setup vcpkg for package management

### Project Structure
- [ ] Create CMake project for core DLL
- [ ] Create Visual Studio solution for launcher
- [ ] Setup build scripts (Debug/Release configurations)
- [ ] Create docs folder structure
- [ ] Create config folder structure

### Documentation
- [ ] Read ARCHITECTURE_HOOK.md thoroughly
- [ ] Create MEMORY_OFFSETS.md template
- [ ] Create FUNCTION_SIGNATURES.md template
- [ ] Create PACKET_PROTOCOL.md template

**Deliverable**: Development environment ready, project skeleton created

---

## Phase 1: Core Infrastructure (Week 2-3)

### 1.1 Basic DLL
- [ ] Create DLL entry point (DllMain)
- [ ] Implement thread creation for main logic
- [ ] Setup console allocation for debugging
- [ ] Implement graceful shutdown mechanism
- [ ] Test basic injection with simple injector

**Files**:
- `core_dll/src/dllmain.cpp`
- `core_dll/src/main.cpp`

### 1.2 Logger System
- [ ] Implement console logger (colored output)
- [ ] Implement file logger (with rotation)
- [ ] Log levels (DEBUG, INFO, WARN, ERROR)
- [ ] Thread-safe logging
- [ ] Performance profiling logs

**Files**:
- `core_dll/src/core/Logger.h`
- `core_dll/src/core/Logger.cpp`

### 1.3 Memory Manager
- [ ] Implement GetModuleBase()
- [ ] Implement pattern scanning (single pattern)
- [ ] Implement multi-pattern scanning (optimization)
- [ ] Implement safe Read<T>()
- [ ] Implement safe Write<T>()
- [ ] Implement pointer chain resolver
- [ ] Unit tests for memory operations

**Files**:
- `core_dll/src/core/MemoryManager.h`
- `core_dll/src/core/MemoryManager.cpp`
- `core_dll/src/utils/Patterns.h`

### 1.4 Hook Manager
- [ ] Integrate MinHook
- [ ] Implement HookManager singleton
- [ ] Implement Initialize()/Shutdown()
- [ ] Implement AddHook()/RemoveHook()
- [ ] Hook registry system
- [ ] Error handling for failed hooks
- [ ] Unit tests

**Files**:
- `core_dll/src/core/HookManager.h`
- `core_dll/src/core/HookManager.cpp`

### 1.5 Configuration System
- [ ] Design JSON config schema
- [ ] Implement Config loader (nlohmann/json)
- [ ] Hot-reload config support
- [ ] Offset configuration system
- [ ] Skill configuration system
- [ ] Validate config on load

**Files**:
- `core_dll/src/core/ConfigManager.h`
- `core_dll/src/core/ConfigManager.cpp`
- `config/offsets.json`
- `config/patterns.json`
- `config/skills.json`

**Deliverable**: Core infrastructure working, can inject DLL and log messages

---

## Phase 2: Reverse Engineering JX1 (Week 4-5)

### 2.1 Basic Reconnaissance
- [ ] Launch JX1 game
- [ ] Attach Cheat Engine to process
- [ ] Find process name and architecture (32/64-bit)
- [ ] Identify DirectX version (check d3d9.dll or d3d11.dll)
- [ ] Document game engine info

### 2.2 Find Player Structure
- [ ] Search for HP value (unknown initial value → scan)
- [ ] Take damage → next scan (decreased value)
- [ ] Find HP address
- [ ] Find HP pointer chain (pointer scanner)
- [ ] Find MaxHP relative to HP
- [ ] Find MP and MaxMP
- [ ] Find Level, XP, Gold
- [ ] Find Position (X, Y, Z coordinates)
- [ ] Find player name
- [ ] Document complete Player structure

**Document in**: `docs/MEMORY_OFFSETS.md`

### 2.3 Find Entity List
- [ ] Spawn monsters nearby
- [ ] Search for monster HP
- [ ] Find entity base address
- [ ] Find entity count
- [ ] Find entity array/list structure
- [ ] Identify entity types (player, monster, NPC, item)
- [ ] Document Entity structure

### 2.4 Find Critical Functions
- [ ] Find Attack function (use x64dbg breakpoints)
- [ ] Find MoveTo function
- [ ] Find UseSkill function
- [ ] Find UseItem function
- [ ] Find PickupItem function
- [ ] Find SendChat function
- [ ] Document function signatures and calling conventions

**Document in**: `docs/FUNCTION_SIGNATURES.md`

### 2.5 Pattern Signatures
- [ ] Convert addresses to pattern signatures (IDA/x64dbg)
- [ ] Test pattern stability across game restarts
- [ ] Create backup patterns (in case of game update)
- [ ] Document all patterns

### 2.6 Network Analysis
- [ ] Use Wireshark to capture game traffic
- [ ] Identify packet structure
- [ ] Find encryption algorithm (if any)
- [ ] Document common packets (login, move, attack, chat)

**Document in**: `docs/PACKET_PROTOCOL.md`

**Deliverable**: Complete memory map and function signatures documented

---

## Phase 3: Game State Reading (Week 6)

### 3.1 Player Class Implementation
- [ ] Implement Player class skeleton
- [ ] Implement GetHP() / GetMaxHP()
- [ ] Implement GetMP() / GetMaxMP()
- [ ] Implement GetLevel() / GetXP()
- [ ] Implement GetPosition() / GetDirection()
- [ ] Implement GetName()
- [ ] Implement GetGold()
- [ ] Implement IsDead() / IsInCombat()
- [ ] Test all getters in-game

**Files**:
- `core_dll/src/game/Player.h`
- `core_dll/src/game/Player.cpp`

### 3.2 Entity Class Implementation
- [ ] Implement Entity base class
- [ ] Implement GetID() / GetType()
- [ ] Implement GetHP() / GetMaxHP()
- [ ] Implement GetPosition()
- [ ] Implement GetName()
- [ ] Implement GetDistance(Entity* other)
- [ ] Implement IsDead() / IsHostile()

**Files**:
- `core_dll/src/game/Entity.h`
- `core_dll/src/game/Entity.cpp`

### 3.3 Entity Manager Implementation
- [ ] Implement EntityManager singleton
- [ ] Implement GetAllEntities()
- [ ] Implement GetNearbyEntities(radius)
- [ ] Implement GetNearbyEnemies(radius)
- [ ] Implement GetNearbyPlayers(radius)
- [ ] Implement GetNearbyItems(radius)
- [ ] Implement GetEntityByID(id)
- [ ] Implement GetClosestEnemy()
- [ ] Filter entities by type/distance

**Files**:
- `core_dll/src/game/EntityManager.h`
- `core_dll/src/game/EntityManager.cpp`

### 3.4 Inventory Manager
- [ ] Find inventory structure (Cheat Engine)
- [ ] Implement Inventory class
- [ ] Implement GetItemCount()
- [ ] Implement GetItem(slot)
- [ ] Implement HasItem(itemID)
- [ ] Implement IsFull()
- [ ] Implement FindEmptySlot()

**Files**:
- `core_dll/src/game/Inventory.h`
- `core_dll/src/game/Inventory.cpp`

### 3.5 Skill Manager
- [ ] Find skill list structure
- [ ] Implement SkillManager class
- [ ] Implement GetSkill(skillID)
- [ ] Implement GetSkillCooldown(skillID)
- [ ] Implement IsSkillReady(skillID)
- [ ] Implement GetSkillManaCost(skillID)

**Files**:
- `core_dll/src/game/SkillManager.h`
- `core_dll/src/game/SkillManager.cpp`

**Deliverable**: Can read all game state accurately in real-time

---

## Phase 4: DirectX Hook & Overlay (Week 7)

### 4.1 DirectX Detection
- [ ] Find DirectX device pointer
- [ ] Identify DirectX version (DX9/DX11)
- [ ] Get VTable addresses
- [ ] Locate Present/EndScene function

### 4.2 DirectX Hook Implementation
- [ ] Hook IDirect3DDevice9::Present (DX9) or IDXGISwapChain::Present (DX11)
- [ ] Hook IDirect3DDevice9::Reset (DX9) or ResizeBuffers (DX11)
- [ ] Test hook stability (no crashes)
- [ ] Implement hook recovery on device loss

**Files**:
- `core_dll/src/hooks/DirectXHook.h`
- `core_dll/src/hooks/DirectXHook.cpp`
- `core_dll/src/hooks/D3D9Hook.cpp` (if DX9)
- `core_dll/src/hooks/D3D11Hook.cpp` (if DX11)

### 4.3 ImGui Integration
- [ ] Initialize ImGui context
- [ ] Setup ImGui for DX9/DX11
- [ ] Create ImGui frame in Present hook
- [ ] Render simple text overlay
- [ ] Test input handling (mouse/keyboard)
- [ ] Setup ImGui styling

### 4.4 Overlay Menu
- [ ] Create main menu window
- [ ] Add tabs (Main, Combat, Farming, Settings)
- [ ] Add checkboxes for bot enable/disable
- [ ] Add sliders for thresholds (HP%, MP%)
- [ ] Add input fields for configuration
- [ ] Add status display (HP, MP, Level, etc.)

**Files**:
- `core_dll/src/overlay/OverlayRenderer.h`
- `core_dll/src/overlay/OverlayRenderer.cpp`
- `core_dll/src/overlay/Menu.h`
- `core_dll/src/overlay/Menu.cpp`

### 4.5 ESP Renderer
- [ ] Implement WorldToScreen() function
- [ ] Draw 2D boxes around entities
- [ ] Draw entity names
- [ ] Draw HP bars
- [ ] Draw distance
- [ ] Color code (red=enemy, green=friend, yellow=NPC)
- [ ] Add ESP toggle

**Files**:
- `core_dll/src/overlay/ESPRenderer.h`
- `core_dll/src/overlay/ESPRenderer.cpp`

**Deliverable**: Working overlay with menu and ESP

---

## Phase 5: Game Function Hooks (Week 8-9)

### 5.1 Attack Function Hook
- [ ] Find attack function signature
- [ ] Create hook for attack function
- [ ] Implement custom Attack(Entity* target)
- [ ] Test auto-attack on target
- [ ] Add attack cooldown handling

**Files**:
- `core_dll/src/hooks/GameFunctionHooks.h`
- `core_dll/src/hooks/GameFunctionHooks.cpp`

### 5.2 Movement Function Hook
- [ ] Find MoveTo function signature
- [ ] Hook MoveTo function
- [ ] Implement custom MoveTo(x, y, z)
- [ ] Test character movement
- [ ] Add pathfinding preparation

### 5.3 Skill Function Hook
- [ ] Find UseSkill function signature
- [ ] Hook UseSkill function
- [ ] Implement custom UseSkill(skillID, target)
- [ ] Test skill usage
- [ ] Add skill cooldown tracking

### 5.4 Item Function Hook
- [ ] Find UseItem function signature
- [ ] Hook UseItem function
- [ ] Implement custom UseItem(itemID, slot)
- [ ] Test item usage (potions)

### 5.5 Pickup Function Hook
- [ ] Find PickupItem function signature
- [ ] Hook PickupItem function
- [ ] Implement custom PickupItem(itemID)
- [ ] Test item pickup

### 5.6 Chat Function Hook
- [ ] Find SendChat function signature
- [ ] Hook SendChat function
- [ ] Implement custom SendChat(message)
- [ ] Test chat sending

**Deliverable**: Can control player actions programmatically

---

## Phase 6: Network Hook (Week 10)

### 6.1 Socket Hook Setup
- [ ] Find WinSock usage in game
- [ ] Hook send() function
- [ ] Hook recv() function
- [ ] Hook WSASend() if used
- [ ] Hook WSARecv() if used
- [ ] Log all packets (hex dump)

**Files**:
- `core_dll/src/hooks/NetworkHook.h`
- `core_dll/src/hooks/NetworkHook.cpp`

### 6.2 Packet Analysis
- [ ] Identify packet header structure
- [ ] Find packet opcode/ID field
- [ ] Find packet length field
- [ ] Document common packet types
- [ ] Create packet structure definitions

### 6.3 Packet Encryption
- [ ] Analyze if packets are encrypted
- [ ] Find encryption/decryption functions
- [ ] Reverse engineer encryption algorithm
- [ ] Implement encryption/decryption
- [ ] Test encrypted packet sending

### 6.4 Packet Handler
- [ ] Create PacketHandler class
- [ ] Parse incoming packets
- [ ] Parse outgoing packets
- [ ] Create packet builders for common actions
- [ ] Implement packet injection

**Files**:
- `core_dll/src/network/PacketHandler.h`
- `core_dll/src/network/PacketHandler.cpp`
- `core_dll/src/network/Packets.h` (packet structures)

**Deliverable**: Can intercept and inject network packets

---

## Phase 7: Bot Logic Engine (Week 11-13)

### 7.1 Bot Engine Core
- [ ] Create BotEngine singleton
- [ ] Implement Start()/Stop()
- [ ] Implement main update loop
- [ ] Thread-safe state management
- [ ] Bot state machine (IDLE, COMBAT, LOOTING, MOVING)

**Files**:
- `core_dll/src/bot/BotEngine.h`
- `core_dll/src/bot/BotEngine.cpp`

### 7.2 Combat Bot
- [ ] Create CombatBot class
- [ ] Implement target selection (nearest, lowest HP, priority)
- [ ] Implement auto-attack logic
- [ ] Implement skill rotation
- [ ] Implement HP/MP monitoring
- [ ] Implement auto-heal (potions + skills)
- [ ] Implement auto-buff
- [ ] Implement combat range checking
- [ ] Test combat bot on low-level monsters

**Files**:
- `core_dll/src/bot/CombatBot.h`
- `core_dll/src/bot/CombatBot.cpp`

### 7.3 Loot Bot
- [ ] Create LootBot class
- [ ] Detect nearby item drops
- [ ] Implement item filter (by quality/type)
- [ ] Implement auto-pickup
- [ ] Implement inventory full check
- [ ] Implement loot priority system

**Files**:
- `core_dll/src/bot/LootBot.h`
- `core_dll/src/bot/LootBot.cpp`

### 7.4 Farming Bot
- [ ] Create FarmingBot class
- [ ] Implement grinding loop (kill → loot → repeat)
- [ ] Implement area farming (stay in radius)
- [ ] Implement return to spawn point
- [ ] Implement stuck detection
- [ ] Implement auto-unstuck
- [ ] Calculate XP/hour, Gold/hour

**Files**:
- `core_dll/src/bot/FarmingBot.h`
- `core_dll/src/bot/FarmingBot.cpp`

### 7.5 Path Finding
- [ ] Implement A* algorithm
- [ ] Create NavMesh for current map
- [ ] Implement obstacle detection
- [ ] Implement FindPath(start, end)
- [ ] Implement FollowPath()
- [ ] Test pathfinding

**Files**:
- `core_dll/src/bot/PathFinder.h`
- `core_dll/src/bot/PathFinder.cpp`
- `core_dll/src/utils/AStar.h`

### 7.6 Quest Bot
- [ ] Detect available quests
- [ ] Parse quest objectives
- [ ] Implement auto-accept quest
- [ ] Implement quest objective tracking
- [ ] Implement auto-complete quest
- [ ] Test with simple quests

**Files**:
- `core_dll/src/bot/QuestBot.h`
- `core_dll/src/bot/QuestBot.cpp`

**Deliverable**: Fully functional bot that can farm autonomously

---

## Phase 8: External Launcher GUI (Week 14-16)

### 8.1 Choose Framework & Setup
- [ ] Decide: Electron vs WPF vs Qt
- [ ] Create new project
- [ ] Setup build system
- [ ] Design UI mockups (Figma/Sketch)

### 8.2 Injector Module
- [ ] Implement DLL injector (CreateRemoteThread or Manual Map)
- [ ] Detect JX1 process
- [ ] Inject core DLL
- [ ] Handle injection errors
- [ ] Implement DLL ejection

**Files**:
- `launcher/src/injector/Injector.cpp` (if C++)
- `launcher/src/injector/injector.js` (if Electron)

### 8.3 IPC Client
- [ ] Implement Named Pipe client
- [ ] Connect to DLL IPC server
- [ ] Send commands to DLL
- [ ] Receive updates from DLL
- [ ] Handle disconnection/reconnection

**Files**:
- `launcher/src/ipc/IPCClient.cpp`

### 8.4 Main Window UI
- [ ] Create main window layout
- [ ] Add process list/selector
- [ ] Add Inject/Eject buttons
- [ ] Add status indicators
- [ ] Add log viewer

### 8.5 Configuration Panel
- [ ] Create settings tab
- [ ] Add bot configuration (Combat, Farming, Loot)
- [ ] Add profile system (save/load profiles)
- [ ] Add hotkey configuration
- [ ] Implement save/load to JSON files

### 8.6 Statistics Dashboard
- [ ] Create stats tab
- [ ] Display real-time stats (HP/MP/Level/XP/Gold)
- [ ] Show XP/hour chart
- [ ] Show Gold/hour chart
- [ ] Show items looted list
- [ ] Show session time

### 8.7 Multi-Client Manager
- [ ] Detect multiple JX1 instances
- [ ] List all running instances
- [ ] Allow individual control per instance
- [ ] Sync bot settings across instances (optional)

**Files**:
- `launcher/src/ui/` (UI components)
- `launcher/src/renderer/` (if Electron)

**Deliverable**: Polished GUI launcher with full control

---

## Phase 9: Multi-Client Support (Week 17-18)

### 9.1 Multi-Process Handling in DLL
- [ ] Each injected DLL instance is independent
- [ ] Ensure no shared memory conflicts
- [ ] Test stability with 5+ game instances

### 9.2 Client Manager in Launcher
- [ ] Implement ClientManager class
- [ ] Track multiple IPC connections
- [ ] Route commands to specific client
- [ ] Display status for each client
- [ ] Broadcast commands to all clients

### 9.3 Bot Synchronization (Optional)
- [ ] Implement party follow mode
- [ ] Synchronize bot start/stop
- [ ] Leader-follower system
- [ ] Test team farming

**Deliverable**: Can run bot on 5+ game instances simultaneously

---

## Phase 10: Advanced Features (Week 19)

### 10.1 Macro System
- [ ] Record user actions (clicks, key presses, delays)
- [ ] Save macro to file
- [ ] Playback macro
- [ ] Macro editor UI
- [ ] Loop macro with conditions

### 10.2 Script System (Optional)
- [ ] Design simple scripting language (Lua or custom DSL)
- [ ] Implement script parser
- [ ] Implement script executor
- [ ] Expose game API to scripts
- [ ] Script editor with syntax highlighting

### 10.3 AI Learning (Future)
- [ ] Collect bot behavior data
- [ ] Train ML model for optimal play
- [ ] Implement AI-driven decision making

---

## Phase 11: Testing, Polish & Documentation (Week 20)

### 11.1 Bug Fixes
- [ ] Fix crash bugs (highest priority)
- [ ] Fix memory leaks
- [ ] Fix hook failures
- [ ] Fix IPC communication issues
- [ ] Stress test with long sessions (24h+)

### 11.2 Performance Optimization
- [ ] Profile CPU usage
- [ ] Profile memory usage
- [ ] Optimize hot paths
- [ ] Reduce overlay rendering cost
- [ ] Optimize packet handling

### 11.3 User Documentation
- [ ] Write installation guide
- [ ] Write user manual
- [ ] Create video tutorials
- [ ] Write FAQ
- [ ] Write troubleshooting guide

### 11.4 Developer Documentation
- [ ] Document all APIs
- [ ] Create architecture diagrams
- [ ] Document memory structures
- [ ] Document hook points
- [ ] Create contributing guide

### 11.5 Release Preparation
- [ ] Create installers (NSIS/WiX)
- [ ] Setup auto-updater
- [ ] Create license validation system
- [ ] Setup crash reporting (Sentry)
- [ ] Final QA testing

**Deliverable**: Production-ready release

---

## Continuous Tasks

### Throughout Development:
- [ ] Commit code regularly (semantic commits)
- [ ] Write unit tests for critical functions
- [ ] Update documentation when code changes
- [ ] Code review (if team)
- [ ] Backup work daily
- [ ] Test on multiple JX1 versions/servers

### Weekly:
- [ ] Update MEMORY_OFFSETS.md if game patches
- [ ] Test pattern stability
- [ ] Performance profiling
- [ ] User feedback integration

---

## Estimated Effort

| Phase | Duration | Complexity | Critical? |
|-------|----------|------------|-----------|
| Phase 0: Setup | 1 week | Low | Yes |
| Phase 1: Core Infrastructure | 2 weeks | Medium | Yes |
| Phase 2: Reverse Engineering | 2 weeks | High | Yes |
| Phase 3: Game State Reading | 1 week | Medium | Yes |
| Phase 4: DirectX Hook | 1 week | High | Yes |
| Phase 5: Game Function Hooks | 2 weeks | High | Yes |
| Phase 6: Network Hook | 1 week | High | Medium |
| Phase 7: Bot Logic | 3 weeks | Medium | Yes |
| Phase 8: External GUI | 3 weeks | Medium | Yes |
| Phase 9: Multi-Client | 2 weeks | Medium | High |
| Phase 10: Advanced Features | 1 week | Low | No |
| Phase 11: Testing & Polish | 1 week | Medium | Yes |
| **TOTAL** | **20 weeks** | - | - |

---

## Risk Mitigation

### Technical Risks:
1. **Game updates breaking offsets** → Pattern scanning, offset auto-discovery
2. **Anti-cheat detection** → Obfuscation, human-like behavior
3. **Crashes from bad hooks** → Error handling, safe memory access
4. **Performance issues** → Profiling, optimization

### Project Risks:
1. **Underestimated time** → Buffer time in schedule
2. **Scope creep** → Stick to MVP, defer advanced features
3. **Lack of reverse engineering skills** → Learning resources, community help

---

## Success Criteria

- [ ] DLL injects successfully and doesn't crash
- [ ] Can read all game state accurately
- [ ] Can control player actions programmatically
- [ ] Bot can farm autonomously for 8+ hours without issues
- [ ] GUI is responsive and user-friendly
- [ ] Multi-client support works with 5+ instances
- [ ] CPU usage < 10% per client
- [ ] Memory usage < 100MB per client
- [ ] User satisfaction: 4.5/5 stars

---

**Last Updated**: 2025-10-14
**Approach**: Full Hook/Injection
**Estimated Completion**: ~5 months
**Maintainer**: Quoc Chien Kieu
