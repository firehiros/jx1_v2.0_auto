# JX1 Launcher Design Preview
# Generates ASCII art preview of the launcher based on LAUNCHER_DESIGN.txt

param(
    [string]$DesignFile = "LAUNCHER_DESIGN.txt"
)

# Check if design file exists
if (-not (Test-Path $DesignFile)) {
    Write-Host "ERROR: Design file not found: $DesignFile" -ForegroundColor Red
    exit 1
}

# Parse design file
$design = @{}
$currentSection = ""

Get-Content $DesignFile | ForEach-Object {
    $line = $_.Trim()

    if ($line -eq "" -or $line.StartsWith("#")) {
        return
    }

    if ($line.StartsWith("[") -and $line.EndsWith("]")) {
        $currentSection = $line.Substring(1, $line.Length - 2)
        $design[$currentSection] = @{}
        return
    }

    if ($line.Contains("=") -and $currentSection -ne "") {
        $parts = $line.Split("=", 2)
        $key = $parts[0].Trim()
        $value = $parts[1].Trim()

        if ($value.Contains("#")) {
            $value = $value.Substring(0, $value.IndexOf("#")).Trim()
        }

        # Remove parentheses comments
        if ($value.Contains("(")) {
            $value = $value.Substring(0, $value.IndexOf("(")).Trim()
        }

        $design[$currentSection][$key] = $value
    }
}

Clear-Host

# Get window size
$width = [int]$design['Window']['Width']
$height = [int]$design['Window']['Height']
$previewWidth = 80  # Console width for preview

# Function to center text
function Center-Text {
    param([string]$Text, [int]$Width = 80)
    $spaces = [Math]::Max(0, ($Width - $Text.Length) / 2)
    return (" " * [int]$spaces) + $Text
}

# Function to draw border
function Draw-Border {
    param([int]$Width = 80, [string]$Char = "═")
    return $Char * $Width
}

# Header
Write-Host ""
Write-Host (Draw-Border -Char "=") -ForegroundColor Cyan
Write-Host (Center-Text "JX1 LAUNCHER DESIGN PREVIEW") -ForegroundColor Cyan
Write-Host (Draw-Border -Char "=") -ForegroundColor Cyan
Write-Host ""

# Window info
Write-Host "Window: $($design['Window']['Title'])" -ForegroundColor Yellow
Write-Host "Size: $($design['Window']['Width']) x $($design['Window']['Height'])" -ForegroundColor Gray
Write-Host ""

# Draw preview
Write-Host (Draw-Border -Width $previewWidth -Char "┌" + "┐") -ForegroundColor White

# Header bar
$headerTitle = $design['Header']['Title']
$headerSubtitle = $design['Header']['Subtitle']
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host (Center-Text "$headerTitle - $headerSubtitle" ($previewWidth - 2)) -ForegroundColor White
Write-Host "║" -ForegroundColor White
Write-Host "║" + (" " * ($previewWidth - 2)) + "║" -ForegroundColor White
Write-Host "║" + (Draw-Border -Width ($previewWidth - 2) -Char "─") + "║" -ForegroundColor White

# Tabs
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " [ Home ] [ Settings ] [ Profiles ] [ About ]" + (" " * ($previewWidth - 48)) + "║" -ForegroundColor Cyan

Write-Host "║" + (Draw-Border -Width ($previewWidth - 2) -Char "─") + "║" -ForegroundColor White

# Home Tab - Status
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " ┌─ Status " + ("─" * ($previewWidth - 14)) + "┐ " -NoNewline -ForegroundColor Yellow
Write-Host "║" -ForegroundColor White

$statusMsg = $design['HomeTab.Status']['DefaultMessage']
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │ $statusMsg" + (" " * ($previewWidth - 4 - $statusMsg.Length)) + "│ " -NoNewline -ForegroundColor White
Write-Host "║" -ForegroundColor White

$gameMsg = $design['HomeTab.Status']['GameNotFound']
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │ $gameMsg" + (" " * ($previewWidth - 4 - $gameMsg.Length)) + "│ " -NoNewline -ForegroundColor White
Write-Host "║" -ForegroundColor White

$dllMsg = $design['HomeTab.Status']['DllNotInjected']
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │ $dllMsg" + (" " * ($previewWidth - 4 - $dllMsg.Length)) + "│ " -NoNewline -ForegroundColor White
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " └" + ("─" * ($previewWidth - 4)) + "┘ " -NoNewline -ForegroundColor Yellow
Write-Host "║" -ForegroundColor White

Write-Host "║" + (" " * ($previewWidth - 2)) + "║" -ForegroundColor White

# Quick Actions
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " ┌─ Quick Actions " + ("─" * ($previewWidth - 20)) + "┐ " -NoNewline -ForegroundColor Yellow
Write-Host "║" -ForegroundColor White

$injectBtn = $design['HomeTab.QuickActions']['InjectButtonText']
$ejectBtn = $design['HomeTab.QuickActions']['EjectButtonText']

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │  [ $injectBtn ]" + (" " * ($previewWidth - 6 - $injectBtn.Length)) + "│ " -NoNewline -ForegroundColor Green
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │  [ $ejectBtn ]" + (" " * ($previewWidth - 6 - $ejectBtn.Length)) + "│ " -NoNewline -ForegroundColor Gray
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " └" + ("─" * ($previewWidth - 4)) + "┘ " -NoNewline -ForegroundColor Yellow
Write-Host "║" -ForegroundColor White

Write-Host "║" + (" " * ($previewWidth - 2)) + "║" -ForegroundColor White

# Bot Control
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " ┌─ Bot Control " + ("─" * ($previewWidth - 17)) + "┐ " -NoNewline -ForegroundColor Yellow
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │ Bot Mode:              Quick Settings:              │ " -NoNewline -ForegroundColor White
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │ ● Disabled (✓)         ☐ Auto Combat                │ " -NoNewline -ForegroundColor White
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │ ○ Combat Only          ☐ Auto Loot                  │ " -NoNewline -ForegroundColor White
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │ ○ Farming Mode         ☐ Auto Heal                  │ " -NoNewline -ForegroundColor White
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │ ○ Questing Mode        ☑ Enable ESP                 │ " -NoNewline -ForegroundColor Cyan
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " └" + ("─" * ($previewWidth - 4)) + "┘ " -NoNewline -ForegroundColor Yellow
Write-Host "║" -ForegroundColor White

Write-Host "║" + (" " * ($previewWidth - 2)) + "║" -ForegroundColor White

# Statistics
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " ┌─ Statistics " + ("─" * ($previewWidth - 16)) + "┐ " -NoNewline -ForegroundColor Yellow
Write-Host "║" -ForegroundColor White

$stat1 = $design['HomeTab.Statistics']['Stat1Label']
$stat2 = $design['HomeTab.Statistics']['Stat2Label']
$stat3 = $design['HomeTab.Statistics']['Stat3Label']

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │ $stat1     $stat2      $stat3    │ " -NoNewline -ForegroundColor Gray
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " │      0              0            00:00:00       │ " -NoNewline -ForegroundColor White
Write-Host "║" -ForegroundColor White

Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " └" + ("─" * ($previewWidth - 4)) + "┘ " -NoNewline -ForegroundColor Yellow
Write-Host "║" -ForegroundColor White

# Footer
Write-Host "║" + (Draw-Border -Width ($previewWidth - 2) -Char "─") + "║" -ForegroundColor White
Write-Host "║" -NoNewline -ForegroundColor White
Write-Host " Ready" + (" " * ($previewWidth - 50)) + "v$($design['AboutTab.Info']['Version'])" + (" " * 35) + "║" -ForegroundColor Gray

# Bottom border
Write-Host (Draw-Border -Width $previewWidth -Char "└" + "┘") -ForegroundColor White

Write-Host ""
Write-Host (Draw-Border -Char "=") -ForegroundColor Cyan

# Color palette
Write-Host ""
Write-Host "Color Palette:" -ForegroundColor Yellow
Write-Host "  Primary Accent:    $($design['Colors']['PrimaryAccent'])" -ForegroundColor White
Write-Host "  Header Background: $($design['Colors']['HeaderBackground'])" -ForegroundColor White
Write-Host "  Button Background: $($design['Colors']['ButtonBackground'])" -ForegroundColor White
Write-Host "  Success Color:     $($design['Colors']['SuccessColor'])" -ForegroundColor White
Write-Host "  Error Color:       $($design['Colors']['ErrorColor'])" -ForegroundColor White

Write-Host ""
Write-Host "To apply this design, run: .\apply_design.ps1" -ForegroundColor Cyan
Write-Host ""
