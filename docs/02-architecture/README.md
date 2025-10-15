# Architecture Documentation

Tài liệu về kiến trúc và thiết kế hệ thống.

---

## 📚 Documents

### [ARCHITECTURE_HOOK.md](ARCHITECTURE_HOOK.md) ⭐ KIẾN TRÚC CHÍNH
- System architecture overview
- Core components
- Hook system design
- Memory management
- Module interactions
- Data flow

**Dành cho:** Hiểu kiến trúc tổng thể hệ thống

---

### [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)
- Folder structure
- File organization
- Module layout
- Third-party libraries
- Build structure

**Dành cho:** Navigate codebase, understand organization

---

### [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)
- Project overview
- Technology stack
- Key features
- Current status
- Roadmap

**Dành cho:** High-level overview, onboarding

---

## 🏗️ System Components

```
┌─────────────────────────────────────┐
│         WPF Launcher (C#)           │
│         (Future - Phase 8)          │
└─────────────────────────────────────┘
              ↕ IPC
┌─────────────────────────────────────┐
│      JX1AutoCore.dll (C++)          │
│  ┌───────────────────────────────┐  │
│  │  Core Systems                 │  │
│  │  - Logger                     │  │
│  │  - MemoryManager              │  │
│  │  - HookManager                │  │
│  │  - ConfigManager              │  │
│  └───────────────────────────────┘  │
│  ┌───────────────────────────────┐  │
│  │  Game Interface               │  │
│  │  - Player, Entity             │  │
│  │  - Inventory, Skills          │  │
│  └───────────────────────────────┘  │
│  ┌───────────────────────────────┐  │
│  │  Hooks                        │  │
│  │  - D3D9Hook (Overlay)         │  │
│  │  - NetworkHook                │  │
│  │  - GameFunctionHooks          │  │
│  └───────────────────────────────┘  │
│  ┌───────────────────────────────┐  │
│  │  Bot Engine                   │  │
│  │  - Combat, Farming, Quest     │  │
│  └───────────────────────────────┘  │
└─────────────────────────────────────┘
              ↕ Hooks
┌─────────────────────────────────────┐
│           JX1 Game                  │
└─────────────────────────────────────┘
```

---

## 📖 Reading Order

```
1. PROJECT_SUMMARY.md     ← Start here for overview
   ↓
2. PROJECT_STRUCTURE.md   ← Understand organization
   ↓
3. ARCHITECTURE_HOOK.md   ← Deep dive into design
```

---

## 🔗 Related

- **Development**: [../04-development/](../04-development/)
- **Implementation**: See core_dll/src/ folder

---

**Back to:** [Main Documentation](../)
