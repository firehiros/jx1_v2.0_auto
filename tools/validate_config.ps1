# Memory Config Validator
# Validates memory_config.json for completeness and correctness

param(
    [string]$ConfigPath = "../config/memory_config.json"
)

$ErrorCount = 0
$WarningCount = 0

function Write-Error-Message {
    param([string]$Message)
    Write-Host "  [ERROR] $Message" -ForegroundColor Red
    $script:ErrorCount++
}

function Write-Warning-Message {
    param([string]$Message)
    Write-Host "  [WARN]  $Message" -ForegroundColor Yellow
    $script:WarningCount++
}

function Write-Success-Message {
    param([string]$Message)
    Write-Host "  [OK]    $Message" -ForegroundColor Green
}

function Validate-HexAddress {
    param([string]$Address)

    if ($Address -match '^0x[0-9A-Fa-f]+$') {
        $value = [Convert]::ToInt64($Address, 16)
        return $value -ne 0
    }
    return $false
}

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "   Memory Config Validator" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Load config
if (-not (Test-Path $ConfigPath)) {
    Write-Host "ERROR: Config file not found: $ConfigPath" -ForegroundColor Red
    exit 1
}

Write-Host "Loading config: $ConfigPath" -ForegroundColor White
$config = Get-Content $ConfigPath -Raw | ConvertFrom-Json

Write-Host ""

# Validate metadata
Write-Host "Validating Metadata..." -ForegroundColor Cyan
if ($config._metadata.version) {
    Write-Success-Message "Version: $($config._metadata.version)"
}
else {
    Write-Error-Message "Version is missing"
}

if ($config._metadata.generated_at) {
    Write-Success-Message "Generated at: $($config._metadata.generated_at)"
}
else {
    Write-Warning-Message "Generated timestamp is empty"
}

Write-Host ""

# Validate game info
Write-Host "Validating Game Info..." -ForegroundColor Cyan
if ($config.game_info.process_name) {
    Write-Success-Message "Process name: $($config.game_info.process_name)"
}
else {
    Write-Error-Message "Process name is missing"
}

Write-Host ""

# Validate base addresses
Write-Host "Validating Base Addresses..." -ForegroundColor Cyan
$baseAddresses = @("player_base", "entity_manager_base", "inventory_base")

foreach ($baseName in $baseAddresses) {
    $base = $config.base_addresses.$baseName

    if ($base) {
        $isValid = Validate-HexAddress $base.address

        if ($isValid) {
            Write-Success-Message "$baseName : $($base.address) [Verified: $($base.verified)]"
        }
        elseif ($base.address -eq "0x0") {
            Write-Warning-Message "$baseName : Not set (0x0)"
        }
        else {
            Write-Error-Message "$baseName : Invalid address format"
        }
    }
    else {
        Write-Error-Message "$baseName : Missing from config"
    }
}

Write-Host ""

# Validate player offsets
Write-Host "Validating Player Offsets..." -ForegroundColor Cyan

$playerCategories = @{
    "health" = @("hp", "max_hp", "mp", "max_mp")
    "position" = @("x", "y", "z", "direction")
    "character" = @("level", "experience", "gold", "name", "id")
    "state" = @("is_dead", "in_combat", "is_moving")
}

foreach ($category in $playerCategories.Keys) {
    Write-Host "  Category: $category" -ForegroundColor Gray

    foreach ($field in $playerCategories[$category]) {
        $offset = $config.player_offsets.$category.$field

        if ($offset) {
            $isValid = Validate-HexAddress $offset.offset

            if ($isValid) {
                $verifiedStr = if ($offset.verified) { "[✓]" } else { "[?]" }
                Write-Success-Message "    $field : $($offset.offset) $verifiedStr"
            }
            elseif ($offset.offset -eq "0x0") {
                Write-Warning-Message "    $field : Not set (0x0)"
            }
            else {
                Write-Error-Message "    $field : Invalid offset format"
            }

            # Check type field
            if (-not $offset.type) {
                Write-Warning-Message "    $field : Type field is missing"
            }
        }
        else {
            Write-Error-Message "    $field : Missing from config"
        }
    }
}

Write-Host ""

# Validate patterns
Write-Host "Validating Patterns..." -ForegroundColor Cyan
$patternNames = @("player_base", "entity_manager", "attack_function", "moveto_function", "useskill_function")

foreach ($patternName in $patternNames) {
    $pattern = $config.patterns.$patternName

    if ($pattern) {
        if ($pattern.pattern -and $pattern.pattern -ne "") {
            Write-Success-Message "$patternName : Pattern defined [Verified: $($pattern.verified)]"

            # Check if mask is present
            if (-not $pattern.mask -or $pattern.mask -eq "") {
                Write-Warning-Message "$patternName : Mask is empty"
            }

            # Check if pattern and mask lengths match
            $patternLength = ($pattern.pattern -replace '\\x', '').Length / 2
            $maskLength = $pattern.mask.Length

            if ($patternLength -ne $maskLength) {
                Write-Error-Message "$patternName : Pattern length ($patternLength) != Mask length ($maskLength)"
            }
        }
        else {
            Write-Warning-Message "$patternName : Pattern not defined"
        }
    }
    else {
        Write-Error-Message "$patternName : Missing from config"
    }
}

Write-Host ""

# Check scan history
Write-Host "Validating Scan History..." -ForegroundColor Cyan
if ($config.scan_history -and $config.scan_history.Count -gt 0) {
    Write-Success-Message "Found $($config.scan_history.Count) scan(s)"

    $verifiedScans = ($config.scan_history | Where-Object { $_.verified -eq $true }).Count
    Write-Host "    Verified scans: $verifiedScans / $($config.scan_history.Count)" -ForegroundColor Gray
}
else {
    Write-Warning-Message "No scan history found (config might be template)"
}

Write-Host ""

# Summary
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "   Validation Summary" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

if ($ErrorCount -eq 0 -and $WarningCount -eq 0) {
    Write-Host "  ✅ All checks passed!" -ForegroundColor Green
}
elseif ($ErrorCount -eq 0) {
    Write-Host "  ⚠️  $WarningCount warning(s) found" -ForegroundColor Yellow
}
else {
    Write-Host "  ❌ $ErrorCount error(s) and $WarningCount warning(s) found" -ForegroundColor Red
}

Write-Host ""

# Readiness check
Write-Host "Configuration Readiness:" -ForegroundColor Cyan

$totalOffsets = 0
$verifiedOffsets = 0

# Count player offsets
foreach ($category in $playerCategories.Keys) {
    foreach ($field in $playerCategories[$category]) {
        $offset = $config.player_offsets.$category.$field
        if ($offset) {
            $totalOffsets++
            if ($offset.verified -and $offset.offset -ne "0x0") {
                $verifiedOffsets++
            }
        }
    }
}

$percentage = if ($totalOffsets -gt 0) { [math]::Round(($verifiedOffsets / $totalOffsets) * 100, 1) } else { 0 }

Write-Host "  Player Offsets: $verifiedOffsets / $totalOffsets verified ($percentage%)" -ForegroundColor $(
    if ($percentage -ge 80) { "Green" }
    elseif ($percentage -ge 50) { "Yellow" }
    else { "Red" }
)

# Recommendations
Write-Host ""
Write-Host "Recommendations:" -ForegroundColor Cyan

if ($verifiedOffsets -lt $totalOffsets) {
    Write-Host "  • Run AutoScanMemory_v2.ps1 to find missing offsets" -ForegroundColor White
}

if ($config.base_addresses.player_base.address -eq "0x0") {
    Write-Host "  • Player base address not set - critical for bot operation" -ForegroundColor White
}

$unverifiedPatterns = ($patternNames | Where-Object {
    $pattern = $config.patterns.$_
    -not $pattern -or -not $pattern.pattern -or $pattern.pattern -eq ""
}).Count

if ($unverifiedPatterns -gt 0) {
    Write-Host "  • $unverifiedPatterns pattern(s) not defined - needed for dynamic address resolution" -ForegroundColor White
}

if ($percentage -ge 80) {
    Write-Host ""
    Write-Host "  ✅ Config is ready for testing!" -ForegroundColor Green
}
elseif ($percentage -ge 50) {
    Write-Host ""
    Write-Host "  ⚠️  Config is partially ready - more offsets needed" -ForegroundColor Yellow
}
else {
    Write-Host ""
    Write-Host "  ❌ Config is not ready - most offsets are missing" -ForegroundColor Red
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan

# Exit code
if ($ErrorCount -gt 0) {
    exit 1
}
exit 0
