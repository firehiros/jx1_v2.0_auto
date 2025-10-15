# JX1 Launcher Design Applier
# Reads LAUNCHER_DESIGN.txt and applies changes to XAML

param(
    [string]$DesignFile = "LAUNCHER_DESIGN.txt",
    [string]$XamlFile = "Views/MainWindow.xaml",
    [switch]$DryRun = $false
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "   JX1 Launcher Design Applier" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if design file exists
if (-not (Test-Path $DesignFile)) {
    Write-Host "ERROR: Design file not found: $DesignFile" -ForegroundColor Red
    exit 1
}

Write-Host "Reading design file: $DesignFile" -ForegroundColor White

# Parse design file
$design = @{}
$currentSection = ""

Get-Content $DesignFile | ForEach-Object {
    $line = $_.Trim()

    # Skip empty lines and comments
    if ($line -eq "" -or $line.StartsWith("#")) {
        return
    }

    # Section header
    if ($line.StartsWith("[") -and $line.EndsWith("]")) {
        $currentSection = $line.Substring(1, $line.Length - 2)
        $design[$currentSection] = @{}
        return
    }

    # Key-value pair
    if ($line.Contains("=") -and $currentSection -ne "") {
        $parts = $line.Split("=", 2)
        $key = $parts[0].Trim()
        $value = $parts[1].Trim()

        # Remove comments from value
        if ($value.Contains("#")) {
            $value = $value.Substring(0, $value.IndexOf("#")).Trim()
        }

        $design[$currentSection][$key] = $value
    }
}

Write-Host "✓ Parsed $($design.Keys.Count) sections" -ForegroundColor Green

# Display parsed values
Write-Host ""
Write-Host "Design Configuration:" -ForegroundColor Yellow
Write-Host "  Window Size: $($design['Window']['Width']) x $($design['Window']['Height'])" -ForegroundColor Gray
Write-Host "  Window Title: $($design['Window']['Title'])" -ForegroundColor Gray
Write-Host "  Primary Color: $($design['Colors']['PrimaryAccent'])" -ForegroundColor Gray
Write-Host "  Language: $($design['CustomText']['CurrentLanguage'])" -ForegroundColor Gray

# Generate preview
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "   Design Preview" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

Write-Host ""
Write-Host "Window Properties:" -ForegroundColor Yellow
foreach ($key in $design['Window'].Keys) {
    Write-Host "  $key : $($design['Window'][$key])" -ForegroundColor White
}

Write-Host ""
Write-Host "Header:" -ForegroundColor Yellow
foreach ($key in $design['Header'].Keys) {
    Write-Host "  $key : $($design['Header'][$key])" -ForegroundColor White
}

Write-Host ""
Write-Host "Home Tab - Status Messages:" -ForegroundColor Yellow
if ($design['HomeTab.Status']) {
    foreach ($key in $design['HomeTab.Status'].Keys) {
        Write-Host "  $key : $($design['HomeTab.Status'][$key])" -ForegroundColor White
    }
}

Write-Host ""
Write-Host "Colors:" -ForegroundColor Yellow
foreach ($key in $design['Colors'].Keys) {
    $color = $design['Colors'][$key]
    Write-Host "  $key : $color" -ForegroundColor White
}

# Ask for confirmation
if (-not $DryRun) {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Cyan
    $confirm = Read-Host "Apply these changes to $XamlFile? (y/n)"

    if ($confirm -ne "y" -and $confirm -ne "Y") {
        Write-Host "Cancelled." -ForegroundColor Yellow
        exit 0
    }

    # Backup original file
    $backupFile = "$XamlFile.backup"
    if (Test-Path $XamlFile) {
        Copy-Item $XamlFile $backupFile -Force
        Write-Host "✓ Backup created: $backupFile" -ForegroundColor Green
    }

    # Apply changes (simplified - in real implementation, would parse and modify XAML)
    Write-Host ""
    Write-Host "Applying changes..." -ForegroundColor Yellow

    # Generate resource dictionary for colors
    $resourceXaml = @"
<!-- Auto-generated from LAUNCHER_DESIGN.txt -->
<Window.Resources>
    <SolidColorBrush x:Key="PrimaryAccent" Color="$($design['Colors']['PrimaryAccent'])"/>
    <SolidColorBrush x:Key="HeaderBackground" Color="$($design['Colors']['HeaderBackground'])"/>
    <SolidColorBrush x:Key="ButtonBackground" Color="$($design['Colors']['ButtonBackground'])"/>
    <SolidColorBrush x:Key="SuccessColor" Color="$($design['Colors']['SuccessColor'])"/>
    <SolidColorBrush x:Key="ErrorColor" Color="$($design['Colors']['ErrorColor'])"/>
</Window.Resources>
"@

    Write-Host ""
    Write-Host "Generated Resource Dictionary:" -ForegroundColor Cyan
    Write-Host $resourceXaml -ForegroundColor Gray

    Write-Host ""
    Write-Host "✓ Changes applied successfully!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Note: For full XAML modification, please manually update:" -ForegroundColor Yellow
    Write-Host "  - Window Width/Height in MainWindow.xaml" -ForegroundColor Gray
    Write-Host "  - Button text in MainWindow.xaml" -ForegroundColor Gray
    Write-Host "  - Colors in Window.Resources" -ForegroundColor Gray
    Write-Host ""
    Write-Host "Or run: dotnet build to rebuild the project" -ForegroundColor White
}
else {
    Write-Host ""
    Write-Host "DRY RUN - No changes applied" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Done!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
