# JX1 V2.0 Auto - Full Hook Architecture

**Official Partnership with Publisher**
**Approach**: DLL Injection + Game Hooking for Maximum Performance

---

## Table of Contents
1. [Architecture Overview](#architecture-overview)
2. [Technology Stack](#technology-stack)
3. [Component Design](#component-design)
4. [Hook Points](#hook-points)
5. [Memory Structure](#memory-structure)
6. [Communication Flow](#communication-flow)
7. [Security & Anti-Detection](#security--anti-detection)
8. [Development Roadmap](#development-roadmap)

---

## Architecture Overview

### High-Level Design

```
┌─────────────────────────────────────────────────────────────┐
│                    External Launcher (GUI)                   │
│  - Modern UI (Electron/WPF)                                 │
│  - Configuration Management                                  │
│  - Statistics Dashboard                                      │
│  - Process Manager                                           │
└─────────────────┬───────────────────────────────────────────┘
                  │
                  │ IPC (Named Pipes / Shared Memory)
                  │
┌─────────────────▼───────────────────────────────────────────┐
│                     Injector Module                          │
│  - DLL Injection (CreateRemoteThread/Manual Map)            │
│  - Process Attachment                                        │
│  - Multi-client Management                                   │
└─────────────────┬───────────────────────────────────────────┘
                  │
                  │ Inject DLL
                  │
┌─────────────────▼───────────────────────────────────────────┐
│              JX1 Game Process Space                          │
│  ┌─────────────────────────────────────────────────────┐   │
│  │         Core Hook DLL (Main Component)              │   │
│  │                                                       │   │
│  │  ┌──────────────────────────────────────────────┐  │   │
│  │  │  Hook Manager                                 │  │   │
│  │  │  - DirectX Hook (Rendering)                   │  │   │
│  │  │  - WinAPI Hook (Input)                        │  │   │
│  │  │  - Network Hook (Packets)                     │  │   │
│  │  │  - Game Function Hook (Combat, Movement...)   │  │   │
│  │  └──────────────────────────────────────────────┘  │   │
│  │                                                       │   │
│  │  ┌──────────────────────────────────────────────┐  │   │
│  │  │  Memory Manager                               │  │   │
│  │  │  - Pattern Scanner                            │  │   │
│  │  │  - Offset Manager                             │  │   │
│  │  │  - Pointer Chain Resolver                     │  │   │
│  │  └──────────────────────────────────────────────┘  │   │
│  │                                                       │   │
│  │  ┌──────────────────────────────────────────────┐  │   │
│  │  │  Game State Manager                           │  │   │
│  │  │  - Player State (HP/MP/Position...)           │  │   │
│  │  │  - Entity Manager (NPCs/Monsters/Players)     │  │   │
│  │  │  - Inventory Manager                          │  │   │
│  │  │  - Skill Manager                              │  │   │
│  │  └──────────────────────────────────────────────┘  │   │
│  │                                                       │   │
│  │  ┌──────────────────────────────────────────────┐  │   │
│  │  │  Bot Logic Engine                             │  │   │
│  │  │  - Combat Bot                                 │  │   │
│  │  │  - Farming Bot                                │  │   │
│  │  │  - Quest Bot                                  │  │   │
│  │  │  - Trading Bot                                │  │   │
│  │  └──────────────────────────────────────────────┘  │   │
│  │                                                       │   │
│  │  ┌──────────────────────────────────────────────┐  │   │
│  │  │  Overlay Renderer                             │  │   │
│  │  │  - ImGui Integration                          │  │   │
│  │  │  - ESP (Entity positions)                     │  │   │
│  │  │  - In-game Menu                               │  │   │
│  │  │  - Debug Console                              │  │   │
│  │  └──────────────────────────────────────────────┘  │   │
│  │                                                       │   │
│  └───────────────────────────────────────────────────┘   │
│                                                            │
│  Original Game Code                                       │
└────────────────────────────────────────────────────────────┘
```

---

## Technology Stack

### Core DLL (Injected Component)
- **Language**: C++17/20
- **Build System**: CMake
- **Hooking Library**:
  - **MinHook** (recommended for inline hooks)
  - **Microsoft Detours** (alternative, official MS library)
- **Graphics**:
  - **DirectX 9/11** (depends on JX1 version)
  - **ImGui** (for overlay rendering)
- **Networking**:
  - **WinSock hooks** (send/recv/WSASend/WSARecv)
  - **Packet encryption/decryption** (reverse engineered from game)

### External Launcher
**Option 1: Electron (Cross-platform, Modern)**
- React/Vue for UI
- Node.js for backend
- Native modules for injection

**Option 2: WPF (Windows-only, Native)**
- C# + .NET Framework/Core
- MVVM architecture
- Native Windows API access

**Option 3: Qt (Cross-platform, Native)**
- C++ or Python (PyQt)
- Native performance
- Good for complex UIs

### Communication
- **IPC Method**: Named Pipes or Shared Memory
- **Data Format**: Protocol Buffers or MessagePack
- **Real-time Updates**: Event-driven architecture

---

## Component Design

### 1. Hook Manager

```cpp
class HookManager {
public:
    static HookManager& Get();

    bool Initialize();
    void Shutdown();

    // DirectX Hooks
    bool HookDirectX();
    void UnhookDirectX();

    // Game Function Hooks
    bool HookGameFunctions();
    void UnhookGameFunctions();

    // Network Hooks
    bool HookNetwork();
    void UnhookNetwork();

private:
    std::vector<Hook*> m_hooks;
    bool m_initialized = false;
};

// Hook structure
struct Hook {
    void* targetAddress;
    void* detourFunction;
    void* originalFunction;
    std::string name;
    bool enabled;
};
```

### 2. Memory Manager

```cpp
class MemoryManager {
public:
    // Pattern scanning
    uintptr_t FindPattern(const char* pattern, const char* mask);
    uintptr_t FindPattern(const std::string& moduleName, const char* pattern);

    // Multi-level pointer
    template<typename T>
    T ReadPointerChain(uintptr_t baseAddress, std::vector<uintptr_t> offsets);

    // Safe read/write
    template<typename T>
    T Read(uintptr_t address);

    template<typename T>
    void Write(uintptr_t address, T value);

    // Module info
    uintptr_t GetModuleBase(const char* moduleName);
    size_t GetModuleSize(const char* moduleName);
};

// Example usage
auto memMgr = MemoryManager::Get();
uintptr_t playerBase = memMgr.FindPattern("JX1Client.exe", "\x8B\x0D\x00\x00\x00\x00\x85\xC9", "xx????xx");
int hp = memMgr.Read<int>(playerBase + 0x10);
```

### 3. Game State Manager

```cpp
// Player class
class Player {
public:
    int GetHP();
    int GetMaxHP();
    int GetMP();
    int GetMaxMP();
    int GetLevel();
    Vector3 GetPosition();
    float GetDirection();

    void SetPosition(Vector3 pos);
    void UseSkill(int skillID, Entity* target);
    void Attack(Entity* target);
    void PickupItem(int itemID);

private:
    uintptr_t m_baseAddress;

    // Offsets (from reverse engineering)
    static constexpr int OFFSET_HP = 0x10;
    static constexpr int OFFSET_MP = 0x14;
    static constexpr int OFFSET_LEVEL = 0x20;
    static constexpr int OFFSET_POS_X = 0x30;
    static constexpr int OFFSET_POS_Y = 0x34;
    static constexpr int OFFSET_POS_Z = 0x38;
};

// Entity Manager
class EntityManager {
public:
    std::vector<Entity*> GetAllEntities();
    std::vector<Entity*> GetNearbyEnemies(float radius);
    std::vector<Entity*> GetNearbyItems(float radius);
    Entity* GetEntityByID(int id);
    Entity* GetClosestEnemy();

private:
    uintptr_t m_entityListAddress;
    int m_entityCount;
};
```

### 4. DirectX Hook (Overlay)

```cpp
// DirectX 9 Present Hook
typedef HRESULT(STDMETHODCALLTYPE* Present_t)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
Present_t oPresent = nullptr;

HRESULT STDMETHODCALLTYPE hkPresent(
    IDirect3DDevice9* pDevice,
    CONST RECT* pSourceRect,
    CONST RECT* pDestRect,
    HWND hDestWindowOverride,
    CONST RGNDATA* pDirtyRegion
) {
    // First time init
    static bool imguiInitialized = false;
    if (!imguiInitialized) {
        ImGui::CreateContext();
        ImGui_ImplDX9_Init(pDevice);
        imguiInitialized = true;
    }

    // Start ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui::NewFrame();

    // Render overlay UI
    OverlayRenderer::Get().Render();

    // Draw ESP
    ESPRenderer::Get().DrawESP();

    // End ImGui frame
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    // Call original
    return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

// Setup hook
void SetupD3D9Hook() {
    // Find Present function in vtable
    uintptr_t* d3d9VTable = GetD3D9VTable();
    oPresent = (Present_t)d3d9VTable[17]; // Present is at index 17

    // Hook it
    MH_CreateHook(oPresent, &hkPresent, reinterpret_cast<LPVOID*>(&oPresent));
    MH_EnableHook(oPresent);
}
```

### 5. Network Hook (Packet Interception)

```cpp
// Hook send function
typedef int (WINAPI* send_t)(SOCKET s, const char* buf, int len, int flags);
send_t oSend = nullptr;

int WINAPI hkSend(SOCKET s, const char* buf, int len, int flags) {
    // Parse packet
    PacketHeader* header = (PacketHeader*)buf;

    Logger::Log("Outgoing packet: ID=0x%X, Size=%d", header->packetID, len);

    // Handle specific packets
    switch (header->packetID) {
        case PACKET_ATTACK:
            OnAttackPacket((AttackPacket*)buf);
            break;
        case PACKET_MOVE:
            OnMovePacket((MovePacket*)buf);
            break;
        case PACKET_USE_SKILL:
            OnSkillPacket((SkillPacket*)buf);
            break;
    }

    // Call original
    return oSend(s, buf, len, flags);
}

// Hook recv function
typedef int (WINAPI* recv_t)(SOCKET s, char* buf, int len, int flags);
recv_t oRecv = nullptr;

int WINAPI hkRecv(SOCKET s, char* buf, int len, int flags) {
    int result = oRecv(s, buf, len, flags);

    if (result > 0) {
        PacketHeader* header = (PacketHeader*)buf;
        Logger::Log("Incoming packet: ID=0x%X, Size=%d", header->packetID, result);

        // Parse server packets
        PacketHandler::HandleServerPacket(buf, result);
    }

    return result;
}
```

### 6. Bot Logic Engine

```cpp
class CombatBot {
public:
    void Update(float deltaTime);

    void SetEnabled(bool enabled);
    bool IsEnabled() const { return m_enabled; }

    void SetTargetPriority(TargetPriority priority);
    void SetSkillRotation(std::vector<int> skills);
    void SetHealThreshold(float hpPercent);

private:
    bool m_enabled = false;
    TargetPriority m_targetPriority = TargetPriority::Nearest;
    std::vector<int> m_skillRotation;
    float m_healThreshold = 0.5f;

    Entity* m_currentTarget = nullptr;
    float m_attackCooldown = 0.0f;

    void SelectTarget();
    void AttackTarget();
    void UseSkills();
    void CheckHealth();
};

// Example bot update loop
void CombatBot::Update(float deltaTime) {
    if (!m_enabled) return;

    Player* player = GameState::GetPlayer();
    if (!player || player->IsDead()) return;

    // Check health
    if (player->GetHP() < player->GetMaxHP() * m_healThreshold) {
        player->UseHealPotion();
    }

    // Select target
    if (!m_currentTarget || m_currentTarget->IsDead()) {
        SelectTarget();
    }

    // Attack
    if (m_currentTarget) {
        m_attackCooldown -= deltaTime;
        if (m_attackCooldown <= 0.0f) {
            UseSkills();
            m_attackCooldown = 1.0f; // 1 second cooldown
        }
    }
}
```

---

## Hook Points

### Critical Game Functions to Hook

#### 1. **Rendering** (DirectX/OpenGL)
```cpp
// DirectX 9
- IDirect3DDevice9::Present
- IDirect3DDevice9::EndScene
- IDirect3DDevice9::Reset

// DirectX 11
- IDXGISwapChain::Present
- IDXGISwapChain::ResizeBuffers
```

#### 2. **Player Actions**
```cpp
// Movement
- Player::SetPosition(float x, float y, float z)
- Player::MoveTo(Vector3 destination)
- Player::Teleport(int mapID, Vector3 pos)

// Combat
- Player::Attack(Entity* target)
- Player::UseSkill(int skillID, Entity* target)
- Player::StopAttack()

// Items
- Player::UseItem(int itemID, int slot)
- Player::PickupItem(int itemID)
- Player::DropItem(int slot, int quantity)

// Inventory
- Inventory::AddItem(Item* item)
- Inventory::RemoveItem(int slot)
- Inventory::SortInventory()
```

#### 3. **Game Loop**
```cpp
// Main game loop - Perfect place to inject bot logic
- Game::Update(float deltaTime)
- Game::Render()

// Entity updates
- Entity::Update(float deltaTime)
- EntityManager::UpdateAll()
```

#### 4. **Network**
```cpp
// WinSock
- send / WSASend
- recv / WSARecv
- sendto / recvfrom

// High-level network functions (if game uses wrapper)
- NetworkManager::SendPacket(Packet* packet)
- NetworkManager::ProcessPacket(Packet* packet)
```

#### 5. **UI**
```cpp
// Dialog handlers
- UI::ShowDialog(int dialogID)
- UI::OnButtonClick(int buttonID)
- UI::OnTextInput(const char* text)

// Chat
- Chat::SendMessage(const char* message)
- Chat::ReceiveMessage(const char* sender, const char* message)
```

---

## Memory Structure

### Typical VLTK Memory Layout (Example)

```
Base Address: 0x00400000 (JX1Client.exe)

Important Pointers:
├─ PlayerBase: [0x00400000 + 0x123456]
│  ├─ +0x00: vtable
│  ├─ +0x10: HP (int)
│  ├─ +0x14: MaxHP (int)
│  ├─ +0x18: MP (int)
│  ├─ +0x1C: MaxMP (int)
│  ├─ +0x20: Level (int)
│  ├─ +0x30: Position X (float)
│  ├─ +0x34: Position Y (float)
│  ├─ +0x38: Position Z (float)
│  ├─ +0x40: Direction (float)
│  ├─ +0x50: Name (char[32])
│  └─ +0x100: Inventory (Inventory*)
│
├─ EntityList: [0x00400000 + 0x234567]
│  ├─ +0x00: Count (int)
│  ├─ +0x04: Capacity (int)
│  └─ +0x08: Array (Entity**)
│     ├─ [0]: Entity*
│     ├─ [1]: Entity*
│     └─ ...
│
├─ SkillManager: [0x00400000 + 0x345678]
│  ├─ +0x00: SkillCount (int)
│  └─ +0x04: Skills (Skill[100])
│     └─ Each Skill:
│        ├─ +0x00: SkillID (int)
│        ├─ +0x04: Level (int)
│        ├─ +0x08: Cooldown (float)
│        └─ +0x0C: ManaCost (int)
│
└─ UIManager: [0x00400000 + 0x456789]
   ├─ +0x00: IsInventoryOpen (bool)
   ├─ +0x04: IsQuestDialogOpen (bool)
   └─ +0x08: CurrentDialog (Dialog*)
```

### Pattern Scanning Examples

```cpp
// Finding player pointer
// Pattern: 8B 0D ?? ?? ?? ?? 85 C9 74 ?? 8B 01
// This is "mov ecx, [address]" instruction
uintptr_t FindPlayerPointer() {
    const char* pattern = "\x8B\x0D\x00\x00\x00\x00\x85\xC9\x74";
    const char* mask = "xx????xxx";

    uintptr_t address = MemoryManager::Get().FindPattern("JX1Client.exe", pattern, mask);
    if (address) {
        // Read the address from instruction
        uintptr_t* ptrAddress = (uintptr_t*)(address + 2);
        return *ptrAddress;
    }
    return 0;
}

// Finding function by signature
uintptr_t FindAttackFunction() {
    // Attack function signature: 55 8B EC 83 EC ?? 56 8B F1 89 75 ??
    const char* pattern = "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1\x89\x75";
    const char* mask = "xxxxx?xxxxx";

    return MemoryManager::Get().FindPattern("JX1Client.exe", pattern, mask);
}
```

---

## Communication Flow

### External GUI ↔ Injected DLL

#### Using Named Pipes:

```cpp
// In DLL (Server side)
class IPCServer {
public:
    void Start() {
        m_pipe = CreateNamedPipe(
            TEXT("\\\\.\\pipe\\JX1AutoPipe"),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            1, 4096, 4096, 0, NULL
        );

        // Listen thread
        std::thread(&IPCServer::ListenThread, this).detach();
    }

    void SendMessage(const std::string& message) {
        DWORD written;
        WriteFile(m_pipe, message.c_str(), message.size(), &written, NULL);
    }

private:
    HANDLE m_pipe;

    void ListenThread() {
        while (true) {
            char buffer[4096];
            DWORD read;

            if (ReadFile(m_pipe, buffer, sizeof(buffer), &read, NULL)) {
                HandleMessage(std::string(buffer, read));
            }
        }
    }

    void HandleMessage(const std::string& msg) {
        // Parse JSON command
        auto json = nlohmann::json::parse(msg);
        std::string cmd = json["command"];

        if (cmd == "start_bot") {
            BotEngine::Get().Start();
        } else if (cmd == "stop_bot") {
            BotEngine::Get().Stop();
        } else if (cmd == "get_stats") {
            SendStats();
        }
    }
};

// In External GUI (Client side)
class IPCClient {
public:
    void Connect() {
        m_pipe = CreateFile(
            TEXT("\\\\.\\pipe\\JX1AutoPipe"),
            GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING, 0, NULL
        );
    }

    void SendCommand(const std::string& command, const nlohmann::json& params) {
        nlohmann::json msg;
        msg["command"] = command;
        msg["params"] = params;

        std::string msgStr = msg.dump();
        DWORD written;
        WriteFile(m_pipe, msgStr.c_str(), msgStr.size(), &written, NULL);
    }

private:
    HANDLE m_pipe;
};
```

### Protocol Messages

```json
// Start combat bot
{
    "command": "start_bot",
    "params": {
        "type": "combat",
        "target_priority": "nearest",
        "skill_rotation": [1001, 1002, 1003],
        "heal_threshold": 0.5
    }
}

// Get statistics
{
    "command": "get_stats",
    "params": {}
}

// Response
{
    "type": "stats_update",
    "data": {
        "hp": 1500,
        "max_hp": 2000,
        "mp": 800,
        "max_mp": 1000,
        "level": 50,
        "xp": 123456,
        "gold": 50000,
        "position": {"x": 100.5, "y": 200.3, "z": 0},
        "target": "Monster_001",
        "bot_status": "running"
    }
}
```

---

## Security & Anti-Detection

### Even with Official Support, Best Practices:

#### 1. **Anti-Debug Protection**
```cpp
bool IsDebuggerPresent() {
    // Check for debugger
    if (::IsDebuggerPresent()) return true;

    // Check for remote debugger
    BOOL isRemoteDebuggerPresent = FALSE;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &isRemoteDebuggerPresent);
    if (isRemoteDebuggerPresent) return true;

    // Check for hardware breakpoints
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    GetThreadContext(GetCurrentThread(), &ctx);
    if (ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3) return true;

    return false;
}
```

#### 2. **String Encryption**
```cpp
// Encrypt important strings at compile time
constexpr auto encryptedString = XOR_STR("PlayerBase");
```

#### 3. **Code Obfuscation**
- Use VMProtect or Themida for DLL protection
- Obfuscate critical functions
- Use control flow flattening

#### 4. **License Validation**
```cpp
class LicenseManager {
public:
    bool ValidateLicense(const std::string& key) {
        // Contact server to validate
        std::string response = HTTPClient::Post(
            "https://api.jx1auto.com/validate",
            {{"key", key}, {"hwid", GetHWID()}}
        );

        return ParseValidationResponse(response);
    }

private:
    std::string GetHWID() {
        // Generate hardware ID
        // CPU serial + Disk serial + MAC address
    }
};
```

---

## Development Roadmap

### Phase 0: Setup (Week 1)
- [ ] Setup C++ development environment (Visual Studio 2022)
- [ ] Install and configure CMake
- [ ] Setup MinHook library
- [ ] Setup ImGui
- [ ] Create basic DLL project structure
- [ ] Create basic injector project

### Phase 1: Core Infrastructure (Week 2-3)
- [ ] Implement Memory Manager (pattern scanner, read/write)
- [ ] Implement Hook Manager (setup/teardown hooks)
- [ ] Implement Logger (file + console)
- [ ] Implement IPC system (Named Pipes)
- [ ] Create basic DLL entry point

### Phase 2: Reverse Engineering (Week 4-5)
- [ ] Analyze JX1 with Cheat Engine
- [ ] Find player base address
- [ ] Find entity list
- [ ] Find critical functions (attack, move, use skill)
- [ ] Document memory structures
- [ ] Create offset configuration system

### Phase 3: Game State Reading (Week 6)
- [ ] Implement Player class (read HP/MP/position/etc)
- [ ] Implement Entity Manager (read nearby entities)
- [ ] Implement Inventory Manager
- [ ] Implement Skill Manager
- [ ] Test data accuracy

### Phase 4: DirectX Hook (Week 7)
- [ ] Find DirectX device
- [ ] Hook Present/EndScene
- [ ] Setup ImGui rendering
- [ ] Create basic overlay menu
- [ ] Implement ESP (entity positions)

### Phase 5: Game Function Hooks (Week 8-9)
- [ ] Hook attack function
- [ ] Hook movement function
- [ ] Hook skill usage function
- [ ] Hook item usage function
- [ ] Test function calls

### Phase 6: Network Hook (Week 10)
- [ ] Hook send/recv functions
- [ ] Log all packets
- [ ] Reverse engineer packet structure
- [ ] Implement packet encryption/decryption
- [ ] Create packet sender/receiver

### Phase 7: Bot Logic (Week 11-13)
- [ ] Implement Combat Bot
- [ ] Implement Farming Bot
- [ ] Implement Auto Loot
- [ ] Implement Auto Heal
- [ ] Implement Path Finding
- [ ] Implement Quest Bot

### Phase 8: External GUI (Week 14-16)
- [ ] Design UI mockups
- [ ] Implement Electron/WPF application
- [ ] Create configuration panels
- [ ] Create statistics dashboard
- [ ] Implement IPC client
- [ ] Test GUI ↔ DLL communication

### Phase 9: Multi-Client (Week 17-18)
- [ ] Implement multi-process detection
- [ ] Create client manager
- [ ] Synchronize bot actions across clients
- [ ] Test stability with 5+ clients

### Phase 10: Testing & Polish (Week 19-20)
- [ ] Fix bugs
- [ ] Optimize performance
- [ ] Add error handling
- [ ] Create user documentation
- [ ] Create video tutorials

---

## File Structure

```
jx1_v2.0_auto/
├── launcher/              # External GUI application
│   ├── src/
│   │   ├── main.js       # Electron main process
│   │   ├── renderer/     # React/Vue UI
│   │   ├── ipc/          # IPC client
│   │   └── injector/     # DLL injector
│   └── package.json
│
├── core_dll/             # Main hook DLL
│   ├── src/
│   │   ├── dllmain.cpp   # DLL entry point
│   │   ├── core/
│   │   │   ├── HookManager.cpp/h
│   │   │   ├── MemoryManager.cpp/h
│   │   │   ├── Logger.cpp/h
│   │   │   └── IPCServer.cpp/h
│   │   ├── hooks/
│   │   │   ├── DirectXHook.cpp/h
│   │   │   ├── NetworkHook.cpp/h
│   │   │   └── GameFunctionHooks.cpp/h
│   │   ├── game/
│   │   │   ├── Player.cpp/h
│   │   │   ├── Entity.cpp/h
│   │   │   ├── EntityManager.cpp/h
│   │   │   ├── Inventory.cpp/h
│   │   │   └── SkillManager.cpp/h
│   │   ├── bot/
│   │   │   ├── BotEngine.cpp/h
│   │   │   ├── CombatBot.cpp/h
│   │   │   ├── FarmingBot.cpp/h
│   │   │   └── QuestBot.cpp/h
│   │   ├── overlay/
│   │   │   ├── OverlayRenderer.cpp/h
│   │   │   ├── ESPRenderer.cpp/h
│   │   │   └── Menu.cpp/h
│   │   └── utils/
│   │       ├── Math.cpp/h
│   │       └── Patterns.h
│   ├── third_party/
│   │   ├── minhook/
│   │   ├── imgui/
│   │   └── json/
│   └── CMakeLists.txt
│
├── docs/
│   ├── ARCHITECTURE_HOOK.md
│   ├── MEMORY_OFFSETS.md
│   ├── FUNCTION_SIGNATURES.md
│   └── PACKET_PROTOCOL.md
│
├── config/
│   ├── offsets.json      # Memory offsets (auto-updated)
│   ├── patterns.json     # Memory patterns
│   └── skills.json       # Skill configurations
│
└── README.md
```

---

## Next Steps

1. **Confirm JX1 DirectX Version**
   - DirectX 9 or DirectX 11?
   - 32-bit or 64-bit executable?

2. **Choose External GUI Framework**
   - Electron (modern, cross-platform)
   - WPF (Windows-only, native)
   - Qt (cross-platform, native)

3. **Start Reverse Engineering**
   - Download Cheat Engine
   - Find player HP/MP addresses
   - Find entity list
   - Document findings

4. **Create Proof of Concept**
   - Simple DLL that hooks Present
   - Draws "Hello World" on screen
   - Validates injection works

---

**Last Updated**: 2025-10-14
**Architecture**: Full Hook/Injection Approach
**Status**: Planning Phase
