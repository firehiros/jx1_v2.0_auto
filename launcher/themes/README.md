# Launcher Themes

Pre-made design themes for the JX1 Launcher.

## 📦 Available Themes

### 1. **Dark Red Theme** - Gaming Style
- **File:** [dark_red_theme.txt](dark_red_theme.txt)
- **Style:** Dark with red accents, gaming aesthetic
- **Colors:** Black/Red with bright green accents
- **Best for:** Gamers, aggressive look

### 2. **Light Blue Theme** - Professional Style
- **File:** [light_blue_theme.txt](light_blue_theme.txt)
- **Style:** Light background with blue accents
- **Colors:** White/Blue material design
- **Best for:** Professional use, daytime

## 🎨 How to Use

### Method 1: Copy entire theme
```powershell
# Copy theme to main design file
Copy-Item themes/dark_red_theme.txt ../LAUNCHER_DESIGN.txt

# Preview
cd ..
.\preview_design.ps1

# Apply
.\apply_design.ps1
```

### Method 2: Copy specific sections
1. Open `themes/dark_red_theme.txt`
2. Copy the `[Colors]` section
3. Paste into `../LAUNCHER_DESIGN.txt`
4. Apply changes

### Method 3: Mix themes
```ini
# In LAUNCHER_DESIGN.txt
# Use Dark Red header
[Header]
BackgroundColor = #1A0000
TitleColor = #FF0000

# But Light Blue buttons
[Colors]
ButtonBackground = #2196F3
ButtonHover = #1976D2
```

## 🆕 Creating Your Own Theme

### 1. Start from template
```powershell
Copy-Item themes/dark_red_theme.txt themes/my_theme.txt
```

### 2. Edit colors
Open `themes/my_theme.txt` and change:
```ini
[Colors]
PrimaryAccent = #YOUR_COLOR
ButtonBackground = #YOUR_COLOR
```

### 3. Test it
```powershell
Copy-Item themes/my_theme.txt ../LAUNCHER_DESIGN.txt
cd ..
.\preview_design.ps1
```

### 4. Share it
Commit to git or share the file!

## 🎨 Color Inspiration

### Gaming Themes
- **Red/Black:** #FF0000, #000000
- **Green/Black:** #00FF00, #000000
- **Purple/Black:** #9B59B6, #000000
- **Cyan/Black:** #00FFFF, #000000

### Professional Themes
- **Blue/White:** #2196F3, #FFFFFF
- **Gray/White:** #607D8B, #FFFFFF
- **Teal/White:** #009688, #FFFFFF

### Material Design Colors
- **Red:** #F44336
- **Pink:** #E91E63
- **Purple:** #9C27B0
- **Deep Purple:** #673AB7
- **Indigo:** #3F51B5
- **Blue:** #2196F3
- **Light Blue:** #03A9F4
- **Cyan:** #00BCD4
- **Teal:** #009688
- **Green:** #4CAF50
- **Light Green:** #8BC34A
- **Lime:** #CDDC39
- **Yellow:** #FFEB3B
- **Amber:** #FFC107
- **Orange:** #FF9800
- **Deep Orange:** #FF5722

## 📋 Theme Template

```ini
===========================================
   YOUR THEME NAME
===========================================

[Window]
Title = JX1 Auto v2.0 - Your Edition
Width = 900
Height = 600

[Header]
BackgroundColor = #COLOR
TitleColor = #COLOR
SubtitleColor = #COLOR

[Colors]
Background = #COLOR
PrimaryAccent = #COLOR
ButtonBackground = #COLOR
SuccessColor = #COLOR
ErrorColor = #COLOR

[HomeTab.QuickActions]
InjectButtonText = Your Text
EjectButtonText = Your Text
```

## 🎯 Tips

1. **Test in preview first**
   ```powershell
   .\preview_design.ps1
   ```

2. **Keep original as backup**
   ```powershell
   Copy-Item LAUNCHER_DESIGN.txt LAUNCHER_DESIGN.backup.txt
   ```

3. **Mix and match**
   - Combine colors from different themes
   - Use one theme's colors, another's text

4. **Consider contrast**
   - Light text on dark background
   - Dark text on light background
   - Test readability

5. **Stay consistent**
   - Use same accent color throughout
   - Match button styles
   - Keep similar spacing

## 🚀 Quick Apply

```powershell
# Dark Red Theme
Copy-Item themes/dark_red_theme.txt LAUNCHER_DESIGN.txt
.\apply_design.ps1

# Light Blue Theme
Copy-Item themes/light_blue_theme.txt LAUNCHER_DESIGN.txt
.\apply_design.ps1
```

---

**Create your own theme and share it!** 🎨
