# Cheat Engine - Quick Reference

**Tham khảo nhanh cho việc tìm memory offsets và patterns**

---

## ⚡ Workflow Nhanh

```
1. 💻 Attach → JX1.exe
2. 🔍 Scan → Tìm giá trị
3. 🎯 Filter → Next Scan
4. 📌 Add → Double-click address
5. 🔗 Pointer Scan → Find stable pointer
```

---

## ⌨️ Hotkeys

```
Ctrl+Alt+L      Attach to process
Ctrl+F          First Scan
Ctrl+N          Next Scan
Space           Freeze/Unfreeze
Delete          Remove from list
F5              Refresh
```

---

## 🔍 Scan Types

```
Exact Value         HP = 2500
Unknown Initial     Không biết giá trị
Increased           Giá trị tăng
Decreased           Giá trị giảm
Changed             Thay đổi
Unchanged           Không đổi
```

---

## 📊 Value Types

```
4 Bytes         HP, MP, Level, ID
Float           Position X/Y/Z, Speed
String          Name, Text
8 Bytes         Money, Large numbers
Array of byte   Code patterns
```

---

## 🎯 Right-Click Options

```
Find what writes...         Tìm code ghi vào
Find what accesses...       Tìm code đọc
Pointer scan...             Tìm pointer chain
Browse memory...            Xem memory
Change value                Đổi giá trị
```

---

## 🔗 Pointer Scan

```
Settings:
- Max level: 5-7
- Max offsets: 3-5
✓ Static addresses only

Rescan:
1. Đóng game → Mở lại
2. Tìm lại address mới
3. Rescan memory
4. Lặp lại 2-3 lần
```

---

## 🔨 Assembly Patterns

### Common Patterns
```
Function start:
55                push ebp
8B EC             mov ebp,esp

Pointer load:
8B 0D ?? ?? ?? ?? mov ecx,[address]
8B 41 ??          mov eax,[ecx+??]

Null check:
85 C0             test eax,eax
74 ??             je short
```

### Wildcards
```
8B 45 08          Exact bytes
8B 45 ??          8B 45 + any byte
8B ?? 08          8B + any + 08
?? ?? ?? ??       Any 4 bytes
```

---

## 📝 Quick Examples

### Find HP
```
1. Scan: 4 Bytes, Exact Value, 2500
2. Change HP in game
3. Next Scan: 2200
4. Repeat until < 10 results
5. Double-click to add
```

### Find Position
```
1. Scan: Float, Unknown Initial
2. Move in game
3. Next Scan: Changed
4. Stand still
5. Next Scan: Unchanged
6. Move again
7. Next Scan: Changed
8. Found X/Y/Z coordinates
```

### Find String
```
1. Value Type: String
2. Scan: "PlayerName"
3. Or use hex:
   50 6C 61 79 65 72  (Player)
```

---

## 💾 Tips

```
Faster scan:
Settings → MEM_PRIVATE only

Save progress:
File → Save → .CT file

Freeze multiple:
Ctrl+Click → Space

Debug:
View → Show disassembler
```

---

## 📋 JX1 Auto Checklist

```
Player:
□ HP/MP (4 Bytes)
□ Position (Float x3)
□ Level (4 Bytes)
□ Name (String)

Entity:
□ List base
□ Count
□ HP
□ Position

Functions:
□ Attack pattern
□ Move pattern
□ UseItem pattern
```

---

## 🎓 Built-in Tutorial

```
Help → Cheat Engine Tutorial
→ Complete 9 lessons
→ Learn basics to advanced
```

---

**See Full Guide:** [CHEAT_ENGINE_GUIDE.md](CHEAT_ENGINE_GUIDE.md)

**Last Updated:** 2025-10-15
