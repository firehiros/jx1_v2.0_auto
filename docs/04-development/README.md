# Development Documentation

Tài liệu cho development workflow và tracking.

---

## 📚 Documents

### [CHECKLIST.md](CHECKLIST.md)
- Overall project checklist
- All phases tracking
- Feature completion status

**Dành cho:** Track overall project progress

---

### [CHECKLIST_HOOK.md](CHECKLIST_HOOK.md)
- Hook-specific checklist
- DirectX hook implementation
- Network hook implementation
- Function hook tracking

**Dành cho:** Track hook implementation progress

---

### [PHASE2_READY.md](PHASE2_READY.md)
- Phase 2 readiness check
- Prerequisites verification
- What to expect in Phase 2

**Dành cho:** Before starting Phase 2

---

## 🚀 Development Workflow

### Daily Workflow
```
1. Pull latest
   git pull

2. Check current tasks
   See CHECKLIST.md

3. Make changes
   ... code ...

4. Build & Test
   scripts/build.bat Debug
   bin/SimpleInjector.exe JX1.exe JX1AutoCore.dll

5. Commit
   git add .
   git commit -m "feat: Description"
   git push
```

---

## 📊 Project Phases

### ✅ Phase 1: Core Infrastructure (COMPLETE)
- [x] DLL injection
- [x] Logger
- [x] MemoryManager
- [x] HookManager
- [x] Basic framework

### ⏳ Phase 2: Reverse Engineering (CURRENT)
- [ ] Find Player offsets
- [ ] Find Entity offsets
- [ ] Find Function patterns
- [ ] Document in Offsets.h/Patterns.h

**Docs:** [../03-reverse-engineering/](../03-reverse-engineering/)

### 🔜 Phase 3: Game State Reading
- [ ] Implement Player class
- [ ] Implement Entity/EntityManager
- [ ] Real-time data reading

### 🔜 Phase 4: DirectX Hook & Overlay
- [ ] Hook DirectX Present()
- [ ] Setup ImGui
- [ ] Draw overlay
- [ ] Implement ESP

### 🔜 Phase 5-8: See CHECKLIST.md

---

## ✅ Before Starting Each Phase

1. **Read phase documentation**
2. **Check prerequisites**
3. **Update checklist**
4. **Commit previous work**

---

## 📝 Commit Message Guidelines

```
feat:     New feature
fix:      Bug fix
docs:     Documentation
refactor: Code refactoring
test:     Testing
build:    Build system
```

**Examples:**
```
feat: Add Player HP reading
fix: Fix memory leak in MemoryManager
docs: Update Cheat Engine guide
refactor: Improve pointer chain reading
```

---

## 🐛 Debugging

### Build Issues
→ See [../01-build/BUILD_INSTRUCTIONS.md](../01-build/BUILD_INSTRUCTIONS.md)

### Runtime Issues
```
1. Check logs: bin/logs/jx1auto_*.log
2. Build Debug version
3. Attach Visual Studio debugger
```

### Game Crashes
```
1. Check hooks are enabled correctly
2. Verify memory addresses
3. Test in safe mode (minimal hooks)
```

---

## 📈 Progress Tracking

Update these regularly:
- [ ] CHECKLIST.md - Overall progress
- [ ] CHECKLIST_HOOK.md - Hook progress
- [ ] Phase-specific STATUS.md files

---

## 🔗 Related

- **Architecture**: [../02-architecture/](../02-architecture/)
- **Build**: [../01-build/](../01-build/)
- **Current Phase**: [../03-reverse-engineering/](../03-reverse-engineering/)

---

**Back to:** [Main Documentation](../)
