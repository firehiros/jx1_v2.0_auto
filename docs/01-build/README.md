# Build Documentation

Tài liệu về build process và commands.

---

## 📚 Documents

### [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) ⭐ CHI TIẾT NHẤT
- Hướng dẫn build chi tiết
- Build Core DLL
- Build Injector
- Verify build outputs
- Troubleshooting build issues

**Dành cho:** Build lần đầu, cần hiểu rõ process

---

### [WINDOWS_BUILD_GUIDE.md](WINDOWS_BUILD_GUIDE.md)
- Windows-specific build guide
- Visual Studio instructions
- MSBuild commands
- PowerShell scripts

**Dành cho:** Windows developers

---

### [COMMANDS.md](COMMANDS.md) ⭐ THAM KHẢO NHANH
- Quick reference commands
- Build commands
- Test commands
- Verification commands
- Daily workflow

**Dành cho:** Tham khảo nhanh khi làm việc

---

## 🚀 Quick Build

```bash
# Debug build
scripts/build.bat Debug

# Build injector
scripts/build_injector.bat

# Test
cd bin
SimpleInjector.exe JX1.exe JX1AutoCore.dll
```

---

## 📁 Build Outputs

```
bin/
├── JX1AutoCore.dll          # Core DLL
├── SimpleInjector.exe       # Injector
├── libMinHook.x86.dll       # MinHook dependency
└── logs/                    # Log files
```

---

## 🐛 Common Issues

### "DirectX SDK not found"
→ Install DirectX SDK or edit CMakeLists.txt

### "MinHook not found"
→ Download MinHook to core_dll/third_party/

### "Architecture mismatch"
→ Make sure building x86 (32-bit)

**Full troubleshooting:** See [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md)

---

## 🔗 Related

- **Getting Started**: [../00-getting-started/](../00-getting-started/)
- **Architecture**: [../02-architecture/](../02-architecture/)

---

**Back to:** [Main Documentation](../)
