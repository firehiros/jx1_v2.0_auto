# Phase 2 Quick Start - 30 Phút Đầu

**Mục tiêu**: Tìm Player HP trong 30 phút đầu tiên

---

## ✅ Chuẩn Bị (5 phút)

### 1. Install Cheat Engine
- Download: https://www.cheatengine.org/downloads.php
- Install Cheat Engine 7.5

### 2. Start JX1
- Launch game
- Login vào character
- Đứng ở safe area (town)

### 3. Note Current Stats
```
Current HP: ________
Max HP: ________
Current MP: ________
Level: ________
Gold: ________
Character Name: ________
```

---

## 🔍 Tìm Player HP (30 phút)

### Step 1: First Scan (2 phút)

**Cheat Engine**:
```
1. Click "Open Process" (icon máy tính)
2. Tìm và chọn "JX1.exe"
3. Click "Open"

4. Value Type: 4 Bytes
5. Scan Type: Exact Value
6. Value: [Nhập HP hiện tại của bạn]
7. Click "First Scan"

Kết quả: Tìm được ____ addresses
```

### Step 2: Next Scan (5 phút)

**Trong game**:
```
1. Bị damage (nhảy xuống cao, hoặc để monster đánh)
2. Note HP mới: ________
```

**Cheat Engine**:
```
1. Value: [Nhập HP mới]
2. Click "Next Scan"

Kết quả: Còn ____ addresses
```

### Step 3: Lặp Lại (15 phút)

**Lặp lại Step 2** cho đến khi:
- Còn < 10 addresses
- Hoặc không thể narrow down thêm

**Tips**:
- Take damage nhiều lần
- Heal với potion
- Wait for HP regen

### Step 4: Tìm Address Đúng (5 phút)

```
1. Double-click từng address trong results
   → Address xuất hiện ở bảng dưới

2. Check box "Active" cho address đầu tiên

3. Change "Value" thành 9999

4. Check game: HP có thay đổi?
   - YES → Tìm được! Proceed to step 5
   - NO → Thử address tiếp theo

5. Khi tìm được:
   - Change description thành "Player HP"
   - Change value về HP thực
   - Uncheck "Active"
   - Note address: ________________

6. Save: File → Save → "Session1.CT"
```

### Step 5: Verify (3 phút)

```
1. Trong game: Take damage
2. Cheat Engine: HP value có update real-time?
   - YES → Perfect! ✅
   - NO → Sai address, tìm lại

3. Change HP value → game HP thay đổi?
   - YES → Confirmed! ✅
```

---

## 🎉 Success!

Nếu tìm được Player HP trong 30 phút:

✅ **Bạn đã hiểu workflow**
✅ **Ready cho các values khác**
✅ **On track để complete Phase 2**

---

## ➡️ Next Steps

### Continue Session 1

**Mở file chính**:
[docs/PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md)

**Go to**: "Session 1 → Finding Player Max HP"

**Follow**: Step-by-step từ đó

---

## 📝 Quick Tips

### For Finding Values

**Known Values** (HP, Level, Gold):
- Scan Type: "Exact Value"
- Change value in game
- Next Scan

**Unknown Values** (Entity Count):
- Scan Type: "Unknown initial value"
- Change value
- "Increased value" or "Decreased value"

**Float Values** (Position):
- Value Type: "Float"
- Move character
- "Increased" or "Decreased"

### For Finding Functions

**Method**: "Find what writes to this address"
```
1. Right-click address (e.g., HP)
2. "Find out what writes to this address"
3. Perform action (e.g., take damage)
4. Instructions appear
5. Double-click → Memory Viewer
6. Scroll up → Find function start
```

---

## ⏱️ Time Checkpoints

After 30 minutes, you should have:
- ✅ Player HP found
- ✅ CE saved ("Session1.CT")
- ✅ Verified HP updates real-time

After 1 hour, you should have:
- ✅ Player HP, Max HP, MP, Max MP

After 2 hours, you should have:
- ✅ All player stats
- ✅ Position X, Y, Z

After 3 hours, you should have:
- ✅ Player base pointer ← Milestone!

---

## 🆘 If Stuck

### Too Many Results (1000+)

**Try**:
1. Change value nhiều lần
2. Scan HP AND MP together (group scan)
3. Use more specific value type

### No Results

**Try**:
1. Wrong value type (try Float if 4 Bytes fails)
2. Value displayed differently (try ±10)
3. Unknown initial value scan

### Need Help

**Check**:
- [PHASE2_QUICK_REFERENCE.md](docs/PHASE2_QUICK_REFERENCE.md) → Common Issues
- [PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md) → Detailed steps
- [PHASE2_REVERSE_ENGINEERING.md](docs/PHASE2_REVERSE_ENGINEERING.md) → Theory

---

## ✅ First 30 Minutes Checklist

- [ ] Cheat Engine installed
- [ ] JX1 game running và logged in
- [ ] CE attached to JX1.exe
- [ ] Found Player HP address
- [ ] Verified HP updates in real-time
- [ ] Saved CE table ("Session1.CT")
- [ ] Ready to continue Session 1

**If all checked** → You're doing great! Continue! 🎉

**If stuck** → Review guide, take break, try again

---

**Start Now**: [docs/PHASE2_EXECUTION_GUIDE.md](docs/PHASE2_EXECUTION_GUIDE.md)

**Timer**: Start: ________ End: ________ (Goal: 30 min for HP)

Good luck! 🍀
