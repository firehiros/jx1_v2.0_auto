# JX1 V2.0 Auto - Documentation

Tài liệu tổng hợp cho JX1 Auto Core project.

---

## 📚 Documentation Structure

### [00-getting-started/](00-getting-started/) ⭐ BẮT ĐẦU TẠI ĐÂY
```
Tài liệu hướng dẫn bắt đầu nhanh

📄 QUICK_START.md            ⭐ Bắt đầu 5 phút
📄 SETUP_GUIDE.md               Chi tiết setup
📄 WINDOWS_QUICK_START.md       Windows-specific
```

**Dành cho:** Người mới, setup lần đầu

---

### [01-build/](01-build/)
```
Tài liệu về build process

📄 BUILD_INSTRUCTIONS.md     ⭐ Hướng dẫn build chi tiết
📄 WINDOWS_BUILD_GUIDE.md       Windows build
📄 COMMANDS.md                  Command reference
```

**Dành cho:** Build project, troubleshoot build issues

---

### [02-architecture/](02-architecture/)
```
Tài liệu về kiến trúc hệ thống

📄 ARCHITECTURE_HOOK.md      ⭐ System architecture
📄 PROJECT_STRUCTURE.md         Folder structure
📄 PROJECT_SUMMARY.md           Project overview
```

**Dành cho:** Hiểu design, navigate codebase

---

### [03-reverse-engineering/](03-reverse-engineering/) ⭐ PHASE 2
```
Tài liệu về Cheat Engine & Reverse Engineering

📄 CHEAT_ENGINE_GUIDE.md     ⭐ Complete CE tutorial
📄 CE_QUICK_REFERENCE.md        Quick reference
📄 MEMORY_MAP.md                Memory documentation
📄 PHASE2_*.md                  Phase 2 tracking
```

**Dành cho:** Phase 2 - Tìm offsets và patterns

**Thời gian:** 2-4 giờ

---

### [04-development/](04-development/)
```
Tài liệu development workflow

📄 CHECKLIST.md              ⭐ Overall progress
📄 CHECKLIST_HOOK.md            Hook progress
📄 PHASE2_READY.md              Phase 2 readiness
```

**Dành cho:** Track progress, development workflow

---

### [05-research/](05-research/)
```
Research notes và findings

📄 RESEARCH_AGTOOL.md           AGTool analysis
📄 DETECT_DIRECTX.md            DirectX detection
```

**Dành cho:** Understanding techniques, research reference

---

## 🚀 Quick Navigation

### First Time Here?
```
1. docs/00-getting-started/QUICK_START.md    ← Start
2. docs/01-build/BUILD_INSTRUCTIONS.md       ← Build
3. docs/03-reverse-engineering/              ← Phase 2
```

### Need Help With...

**Setup & Installation**
→ [00-getting-started/](00-getting-started/)

**Build Errors**
→ [01-build/](01-build/)

**Understanding Architecture**
→ [02-architecture/](02-architecture/)

**Finding Memory Offsets (Phase 2)**
→ [03-reverse-engineering/](03-reverse-engineering/)

**Development Workflow**
→ [04-development/](04-development/)

---

## 📖 Recommended Reading Paths

### Path 1: Quick Start (30 minutes)
```
1. 00-getting-started/QUICK_START.md
2. 01-build/COMMANDS.md
3. Start coding!
```

### Path 2: Complete Setup (2 hours)
```
1. 00-getting-started/SETUP_GUIDE.md
2. 01-build/BUILD_INSTRUCTIONS.md
3. 02-architecture/PROJECT_SUMMARY.md
4. 02-architecture/ARCHITECTURE_HOOK.md
5. 04-development/CHECKLIST.md
```

### Path 3: Phase 2 - Reverse Engineering (4 hours)
```
1. 04-development/PHASE2_READY.md           ← Check readiness
2. 03-reverse-engineering/CHEAT_ENGINE_GUIDE.md
3. Follow tutorials 1-5
4. Document findings in Offsets.h/Patterns.h
5. Update 04-development/CHECKLIST.md
```

---

## 📊 Project Status

### Current Phase: Phase 2 - Reverse Engineering

```
✅ Phase 1: Core Infrastructure (COMPLETE)
   - DLL injection
   - Logger, MemoryManager, HookManager
   - Basic framework

⏳ Phase 2: Reverse Engineering (CURRENT)
   - Find Player offsets
   - Find Entity offsets
   - Find Function patterns
   → See: docs/03-reverse-engineering/

🔜 Phase 3: Game State Reading
🔜 Phase 4: DirectX Hook & Overlay
🔜 Phase 5-8: See CHECKLIST.md
```

**Progress:** ~20% complete

---

## 🎯 What You Should Read Now

### If you are...

**New to the project:**
→ Start with [00-getting-started/QUICK_START.md](00-getting-started/QUICK_START.md)

**Ready for Phase 2:**
→ Read [03-reverse-engineering/CHEAT_ENGINE_GUIDE.md](03-reverse-engineering/CHEAT_ENGINE_GUIDE.md)

**Need to build:**
→ See [01-build/BUILD_INSTRUCTIONS.md](01-build/BUILD_INSTRUCTIONS.md)

**Want to understand architecture:**
→ See [02-architecture/ARCHITECTURE_HOOK.md](02-architecture/ARCHITECTURE_HOOK.md)

**Tracking development:**
→ See [04-development/CHECKLIST.md](04-development/CHECKLIST.md)

---

## 📝 Documentation Guidelines

### When Adding New Docs

1. Choose appropriate folder (00-05)
2. Use markdown format
3. Add to folder's README.md
4. Update this master README
5. Link to related docs

### Template
```markdown
# Document Title

Brief description

## Section 1
Content...

## Related
- Link to related docs

---

**Back to:** [Main Documentation](README.md)
```

---

## 🔗 External Resources

### Tools
- [Cheat Engine](https://www.cheatengine.org/) - Memory scanner
- [Visual Studio 2022](https://visualstudio.microsoft.com/) - IDE
- [CMake](https://cmake.org/) - Build system

### Libraries
- [MinHook](https://github.com/TsudaKageyu/minhook) - Hooking library
- [ImGui](https://github.com/ocornut/imgui) - GUI library
- [nlohmann/json](https://github.com/nlohmann/json) - JSON library

### Learning
- [Guided Hacking](https://guidedhacking.com/) - Game hacking tutorials
- [Fearless Revolution](https://fearlessrevolution.com/) - CE community
- [OSDev](https://wiki.osdev.org/) - Low-level programming

---

## 🆘 Getting Help

### Quick Answers
- Check relevant folder's README.md
- See COMMANDS.md for command reference
- Check CHECKLIST.md for current status

### Common Issues

**Build fails:**
→ [01-build/BUILD_INSTRUCTIONS.md](01-build/BUILD_INSTRUCTIONS.md)

**Can't inject DLL:**
→ [00-getting-started/QUICK_START.md](00-getting-started/QUICK_START.md)

**Don't know where to start:**
→ [00-getting-started/QUICK_START.md](00-getting-started/QUICK_START.md)

---

## 📈 Documentation Stats

```
Total Documents:    ~25 files
Total Categories:   6 folders
Lines of Docs:      ~3000+ lines
Last Updated:       2025-10-15
```

---

## ⭐ Most Important Documents

```
1. 00-getting-started/QUICK_START.md           ← Start here
2. 03-reverse-engineering/CHEAT_ENGINE_GUIDE.md ← Phase 2
3. 01-build/BUILD_INSTRUCTIONS.md              ← Building
4. 02-architecture/ARCHITECTURE_HOOK.md        ← Understanding
5. 04-development/CHECKLIST.md                 ← Progress
```

---

**Project:** JX1 V2.0 Auto Core
**Version:** 1.0.0
**Last Updated:** 2025-10-15
**Status:** Phase 2 - Reverse Engineering

**Back to:** [Project Root](../)
