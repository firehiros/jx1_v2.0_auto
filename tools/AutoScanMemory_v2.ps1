# JX1 Auto Memory Scanner v2 - With JSON Config Generation
# Auto-generates memory_config.json for the bot
# Usage: Right-click -> Run with PowerShell

param(
    [string]$ProcessName = "JX1",
    [string]$ConfigPath = "../config/memory_config.json"
)

# Global variables
$script:ConfigData = $null
$script:ScanResults = @{}

# Check if running as Administrator
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Host "WARNING: Not running as Administrator. Some memory regions may not be accessible." -ForegroundColor Yellow
    Write-Host "For best results, right-click and select 'Run as Administrator'" -ForegroundColor Yellow
    Write-Host ""
}

# Windows API definitions
Add-Type @"
using System;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Collections.Generic;

public class MemoryScanner {
    [DllImport("kernel32.dll")]
    public static extern IntPtr OpenProcess(int dwDesiredAccess, bool bInheritHandle, int dwProcessId);

    [DllImport("kernel32.dll")]
    public static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, ref int lpNumberOfBytesRead);

    [DllImport("kernel32.dll")]
    public static extern bool CloseHandle(IntPtr hObject);

    [DllImport("kernel32.dll")]
    public static extern int VirtualQueryEx(IntPtr hProcess, IntPtr lpAddress, out MEMORY_BASIC_INFORMATION lpBuffer, uint dwLength);

    [StructLayout(LayoutKind.Sequential)]
    public struct MEMORY_BASIC_INFORMATION {
        public IntPtr BaseAddress;
        public IntPtr AllocationBase;
        public uint AllocationProtect;
        public IntPtr RegionSize;
        public uint State;
        public uint Protect;
        public uint Type;
    }

    public const int PROCESS_QUERY_INFORMATION = 0x0400;
    public const int PROCESS_VM_READ = 0x0010;
    public const uint PAGE_READONLY = 0x02;
    public const uint PAGE_READWRITE = 0x04;
    public const uint PAGE_WRITECOPY = 0x08;
    public const uint PAGE_EXECUTE_READ = 0x20;
    public const uint PAGE_EXECUTE_READWRITE = 0x40;
    public const uint MEM_COMMIT = 0x1000;
    public const uint MEM_PRIVATE = 0x20000;

    public static List<long> ScanMemoryForInt32(int processId, int value) {
        List<long> results = new List<long>();
        IntPtr processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processId);

        if (processHandle == IntPtr.Zero) {
            return results;
        }

        try {
            IntPtr address = IntPtr.Zero;
            MEMORY_BASIC_INFORMATION mbi = new MEMORY_BASIC_INFORMATION();

            while (VirtualQueryEx(processHandle, address, out mbi, (uint)Marshal.SizeOf(typeof(MEMORY_BASIC_INFORMATION))) != 0) {
                if (mbi.State == MEM_COMMIT &&
                    (mbi.Protect == PAGE_READWRITE ||
                     mbi.Protect == PAGE_READONLY ||
                     mbi.Protect == PAGE_EXECUTE_READWRITE)) {

                    byte[] buffer = new byte[(int)mbi.RegionSize];
                    int bytesRead = 0;

                    if (ReadProcessMemory(processHandle, mbi.BaseAddress, buffer, buffer.Length, ref bytesRead)) {
                        for (int i = 0; i < bytesRead - 3; i += 4) {
                            int currentValue = BitConverter.ToInt32(buffer, i);
                            if (currentValue == value) {
                                long foundAddress = (long)mbi.BaseAddress + i;
                                results.Add(foundAddress);
                            }
                        }
                    }
                }

                address = new IntPtr((long)mbi.BaseAddress + (long)mbi.RegionSize);
                if ((long)address >= 0x7FFFFFFFFFFF) break;
            }
        }
        finally {
            CloseHandle(processHandle);
        }

        return results;
    }

    public static int ReadInt32(int processId, long address) {
        IntPtr processHandle = OpenProcess(PROCESS_VM_READ, false, processId);
        if (processHandle == IntPtr.Zero) return 0;

        try {
            byte[] buffer = new byte[4];
            int bytesRead = 0;
            if (ReadProcessMemory(processHandle, new IntPtr(address), buffer, 4, ref bytesRead)) {
                return BitConverter.ToInt32(buffer, 0);
            }
        }
        finally {
            CloseHandle(processHandle);
        }

        return 0;
    }
}
"@

#region JSON Configuration Functions

function Load-ConfigTemplate {
    $configPath = Join-Path $PSScriptRoot $ConfigPath

    if (Test-Path $configPath) {
        Write-Host "Loading existing config: $configPath" -ForegroundColor Cyan
        $script:ConfigData = Get-Content $configPath -Raw | ConvertFrom-Json
    }
    else {
        Write-Host "Config not found, will create new one" -ForegroundColor Yellow
        # Will load from the template we created
        $templatePath = Join-Path $PSScriptRoot "../config/memory_config.json"
        if (Test-Path $templatePath) {
            $script:ConfigData = Get-Content $templatePath -Raw | ConvertFrom-Json
        }
        else {
            Write-Host "ERROR: Template config not found!" -ForegroundColor Red
            exit 1
        }
    }
}

function Save-Config {
    param([string]$OutputPath)

    if (-not $OutputPath) {
        $OutputPath = Join-Path $PSScriptRoot $ConfigPath
    }

    # Update metadata
    $script:ConfigData._metadata.generated_at = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $script:ConfigData._metadata.generated_by = "AutoScanMemory v2.0"

    # Convert to JSON with proper formatting
    $jsonContent = $script:ConfigData | ConvertTo-Json -Depth 10

    # Save to file
    $jsonContent | Out-File -FilePath $OutputPath -Encoding UTF8
    Write-Host ""
    Write-Host "============================================" -ForegroundColor Green
    Write-Host "Config saved to: $OutputPath" -ForegroundColor Green
    Write-Host "============================================" -ForegroundColor Green
}

function Update-ConfigValue {
    param(
        [string]$Section,
        [string]$Key,
        [string]$Property,
        [string]$Value,
        [bool]$Verified = $true
    )

    try {
        # Navigate the config structure
        $target = $script:ConfigData.$Section.$Key

        if ($target -and $target.PSObject.Properties[$Property]) {
            $target.$Property = $Value
            if ($target.PSObject.Properties["verified"]) {
                $target.verified = $Verified
            }
            Write-Host "  Updated: $Section.$Key.$Property = $Value" -ForegroundColor Gray
        }
        else {
            Write-Host "  Warning: Could not find $Section.$Key.$Property" -ForegroundColor Yellow
        }
    }
    catch {
        Write-Host "  Error updating config: $_" -ForegroundColor Red
    }
}

function Add-ScanHistory {
    param(
        [string]$ValueType,
        [array]$Addresses,
        [int]$FiltersApplied,
        [string]$FinalAddress,
        [bool]$Verified
    )

    $historyEntry = @{
        timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
        value_type = $ValueType
        addresses_found = $Addresses
        filters_applied = $FiltersApplied
        final_address = $FinalAddress
        verified = $Verified
    }

    # Add to scan history
    if (-not $script:ConfigData.scan_history) {
        $script:ConfigData.scan_history = @()
    }

    $script:ConfigData.scan_history += $historyEntry
}

#endregion

#region Memory Scanning Functions

function Find-Process {
    param([string]$Name)

    $processes = Get-Process -Name $Name -ErrorAction SilentlyContinue
    if ($processes) {
        return $processes[0]
    }
    return $null
}

function Scan-Memory {
    param(
        [int]$ProcessId,
        [int]$Value
    )

    Write-Host "Scanning memory for value: $Value" -ForegroundColor Cyan
    Write-Host "This may take a few minutes..." -ForegroundColor Yellow

    try {
        $results = [MemoryScanner]::ScanMemoryForInt32($ProcessId, $Value)
        return $results
    }
    catch {
        Write-Host "Error during scan: $_" -ForegroundColor Red
        return @()
    }
}

function Filter-Results {
    param(
        [int]$ProcessId,
        [array]$Addresses,
        [int]$NewValue
    )

    Write-Host "Filtering results for value: $NewValue" -ForegroundColor Cyan

    $filtered = @()
    $count = 0

    foreach ($addr in $Addresses) {
        $count++
        if ($count % 100 -eq 0) {
            Write-Progress -Activity "Filtering addresses" -Status "$count / $($Addresses.Count)" -PercentComplete (($count / $Addresses.Count) * 100)
        }

        $currentValue = [MemoryScanner]::ReadInt32($ProcessId, $addr)
        if ($currentValue -eq $NewValue) {
            $filtered += $addr
        }
    }

    Write-Progress -Activity "Filtering addresses" -Completed
    return $filtered
}

function Analyze-MemoryRegion {
    param(
        [int]$ProcessId,
        [long]$BaseAddress,
        [int]$Range = 256
    )

    Write-Host ""
    Write-Host "Analyzing memory region around 0x$($BaseAddress.ToString('X'))..." -ForegroundColor Cyan
    Write-Host "Offset  | Hex Value    | Dec Value   | Type Guess" -ForegroundColor Gray
    Write-Host "--------|--------------|-------------|------------" -ForegroundColor Gray

    $nearbyValues = @{}

    for ($offset = -64; $offset -lt $Range; $offset += 4) {
        $addr = $BaseAddress + $offset
        $value = [MemoryScanner]::ReadInt32($ProcessId, $addr)

        if ($value -ne 0) {
            $hex = "0x{0:X8}" -f $value
            $offsetStr = "{0:+#;-#;+0}" -f $offset

            # Guess data type
            $guess = ""
            if ($value -ge 0 -and $value -le 200) { $guess = "Level/Count?" }
            elseif ($value -ge 1000 -and $value -le 50000) { $guess = "HP/MP?" }
            elseif ($value -gt 100000) { $guess = "Gold/Exp?" }

            Write-Host ("{0,7} | {1,-12} | {2,11} | {3}" -f $offsetStr, $hex, $value, $guess)

            # Store for later
            $nearbyValues[$offset] = $value
        }
    }

    return $nearbyValues
}

function Suggest-Offsets {
    param(
        [hashtable]$MemoryRegion,
        [long]$BaseAddress,
        [string]$TargetValueType
    )

    Write-Host ""
    Write-Host "Suggested offsets for nearby values:" -ForegroundColor Cyan

    foreach ($offset in $MemoryRegion.Keys | Sort-Object) {
        $value = $MemoryRegion[$offset]
        $hexOffset = "0x{0:X}" -f [Math]::Abs($offset)

        if ($offset -eq 0) {
            Write-Host "  $hexOffset -> $value (This is the scanned value: $TargetValueType)" -ForegroundColor Green
        }
        else {
            Write-Host "  $hexOffset -> $value" -ForegroundColor White
        }
    }
}

#endregion

#region Automated Scan Workflows

function Auto-ScanHP {
    Write-Host "============================================" -ForegroundColor Cyan
    Write-Host "   Auto HP Scanner (Config Generator)" -ForegroundColor Cyan
    Write-Host "============================================" -ForegroundColor Cyan
    Write-Host ""

    # Find process
    $process = Find-Process -Name $ProcessName
    if (-not $process) {
        Write-Host "ERROR: Process '$ProcessName' not found!" -ForegroundColor Red
        Write-Host "Make sure the game is running." -ForegroundColor Yellow
        return
    }

    Write-Host "Found process: $($process.ProcessName) (PID: $($process.Id))" -ForegroundColor Green
    Write-Host ""

    # Step 1: Initial scan
    $hp1 = Read-Host "[Step 1] Enter your CURRENT HP value"
    if (-not $hp1) { return }

    $results = Scan-Memory -ProcessId $process.Id -Value ([int]$hp1)
    Write-Host "Found $($results.Count) addresses" -ForegroundColor Yellow
    Write-Host ""

    $filtersApplied = 0

    # Step 2: Filter results
    while ($results.Count -gt 10 -and $filtersApplied -lt 5) {
        $filtersApplied++
        Write-Host "[Step $($filtersApplied + 1)] Change your HP (take damage or heal)..." -ForegroundColor Cyan
        $hp2 = Read-Host "Enter your NEW HP value (or 'q' to stop)"
        if ($hp2 -eq 'q') { break }

        $results = Filter-Results -ProcessId $process.Id -Addresses $results -NewValue ([int]$hp2)
        Write-Host "Remaining: $($results.Count) addresses" -ForegroundColor Yellow
        Write-Host ""
    }

    # Display results
    Write-Host "============================================" -ForegroundColor Green
    if ($results.Count -eq 0) {
        Write-Host "No addresses found!" -ForegroundColor Red
        Add-ScanHistory -ValueType "hp" -Addresses @() -FiltersApplied $filtersApplied -FinalAddress "" -Verified $false
    }
    elseif ($results.Count -le 10) {
        Write-Host "SUCCESS! Found HP address(es):" -ForegroundColor Green
        Write-Host ""

        $finalAddress = $results[0]
        foreach ($addr in $results) {
            Write-Host ("  0x{0:X}" -f $addr) -ForegroundColor White
        }

        # Analyze memory region
        $memoryRegion = Analyze-MemoryRegion -ProcessId $process.Id -BaseAddress $finalAddress
        Suggest-Offsets -MemoryRegion $memoryRegion -BaseAddress $finalAddress -TargetValueType "HP"

        # Ask user if this is correct
        Write-Host ""
        $confirm = Read-Host "Is the first address (0x$($finalAddress.ToString('X'))) the correct HP address? (y/n)"

        if ($confirm -eq 'y') {
            # Update config
            Write-Host ""
            Write-Host "Updating config..." -ForegroundColor Cyan

            $hexAddr = "0x{0:X}" -f $finalAddress
            Update-ConfigValue -Section "player_offsets" -Key "health" -Property "offset" -Value $hexAddr -Verified $true
            $script:ConfigData.player_offsets.health.hp.offset = "0x10"  # Assume HP is at offset 0x10 from base
            $script:ConfigData.player_offsets.health.hp.verified = $true
            $script:ConfigData.player_offsets.health.hp.scan_value = [int]$hp2

            # Try to guess Max HP (usually right after HP)
            $maxHpAddr = $finalAddress + 4
            $maxHpValue = [MemoryScanner]::ReadInt32($process.Id, $maxHpAddr)
            if ($maxHpValue -gt [int]$hp2 -and $maxHpValue -lt 100000) {
                $hexMaxHp = "0x{0:X}" -f $maxHpAddr
                Write-Host "  Detected Max HP at +4 bytes: $maxHpValue" -ForegroundColor Green
                $script:ConfigData.player_offsets.health.max_hp.offset = "0x14"
                $script:ConfigData.player_offsets.health.max_hp.verified = $true
                $script:ConfigData.player_offsets.health.max_hp.scan_value = $maxHpValue
            }

            Add-ScanHistory -ValueType "hp" -Addresses $results -FiltersApplied $filtersApplied -FinalAddress $hexAddr -Verified $true

            Write-Host "  Config updated successfully!" -ForegroundColor Green
        }
        else {
            Add-ScanHistory -ValueType "hp" -Addresses $results -FiltersApplied $filtersApplied -FinalAddress "" -Verified $false
        }
    }
    else {
        Write-Host "Too many results ($($results.Count)). Try more iterations." -ForegroundColor Yellow
        Add-ScanHistory -ValueType "hp" -Addresses $results -FiltersApplied $filtersApplied -FinalAddress "" -Verified $false
    }
    Write-Host "============================================" -ForegroundColor Green
    Write-Host ""
    Read-Host "Press Enter to continue"
}

function Auto-ScanMP {
    Write-Host "MP Scanner - Similar to HP Scanner" -ForegroundColor Yellow
    # Similar implementation for MP
    # ... (similar code to Auto-ScanHP)
}

function Auto-ScanGold {
    Write-Host "Gold Scanner - Similar to HP Scanner" -ForegroundColor Yellow
    # Similar implementation for Gold
}

function Auto-ScanLevel {
    Write-Host "Level Scanner" -ForegroundColor Cyan
    # ... implementation
}

function Batch-ScanAll {
    Write-Host "============================================" -ForegroundColor Cyan
    Write-Host "   Batch Scan All Values" -ForegroundColor Cyan
    Write-Host "============================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "This will scan for all common values in sequence." -ForegroundColor White
    Write-Host "You'll need to change each value during the scan." -ForegroundColor White
    Write-Host ""

    $process = Find-Process -Name $ProcessName
    if (-not $process) {
        Write-Host "ERROR: Process not found!" -ForegroundColor Red
        return
    }

    # Scan sequence
    $scanSequence = @("HP", "MP", "Level", "Gold")

    foreach ($valueType in $scanSequence) {
        Write-Host ""
        Write-Host "--- Scanning for: $valueType ---" -ForegroundColor Yellow

        switch ($valueType) {
            "HP" { Auto-ScanHP }
            "MP" { Auto-ScanMP }
            "Level" { Auto-ScanLevel }
            "Gold" { Auto-ScanGold }
        }

        Write-Host ""
        $continue = Read-Host "Continue to next value? (y/n)"
        if ($continue -ne 'y') { break }
    }

    Write-Host ""
    Write-Host "Batch scan completed!" -ForegroundColor Green
}

#endregion

#region Main Menu

function Show-Menu {
    Clear-Host
    Write-Host "============================================" -ForegroundColor Green
    Write-Host "   JX1 Auto Memory Scanner v2.0" -ForegroundColor Green
    Write-Host "   with JSON Config Generation" -ForegroundColor Green
    Write-Host "============================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "1) Scan for HP (Auto mode)" -ForegroundColor White
    Write-Host "2) Scan for MP (Auto mode)" -ForegroundColor White
    Write-Host "3) Scan for Level" -ForegroundColor White
    Write-Host "4) Scan for Gold/Money" -ForegroundColor White
    Write-Host "5) Batch Scan All" -ForegroundColor Cyan
    Write-Host "6) View Current Config" -ForegroundColor Yellow
    Write-Host "7) Save Config" -ForegroundColor Green
    Write-Host "8) Exit" -ForegroundColor White
    Write-Host ""
}

function Show-CurrentConfig {
    Write-Host ""
    Write-Host "============================================" -ForegroundColor Cyan
    Write-Host "   Current Configuration" -ForegroundColor Cyan
    Write-Host "============================================" -ForegroundColor Cyan

    Write-Host ""
    Write-Host "Player Offsets:" -ForegroundColor Yellow
    Write-Host "  HP:       $($script:ConfigData.player_offsets.health.hp.offset) (Verified: $($script:ConfigData.player_offsets.health.hp.verified))"
    Write-Host "  Max HP:   $($script:ConfigData.player_offsets.health.max_hp.offset) (Verified: $($script:ConfigData.player_offsets.health.max_hp.verified))"
    Write-Host "  MP:       $($script:ConfigData.player_offsets.health.mp.offset) (Verified: $($script:ConfigData.player_offsets.health.mp.verified))"
    Write-Host "  Max MP:   $($script:ConfigData.player_offsets.health.max_mp.offset) (Verified: $($script:ConfigData.player_offsets.health.max_mp.verified))"
    Write-Host "  Level:    $($script:ConfigData.player_offsets.character.level.offset) (Verified: $($script:ConfigData.player_offsets.character.level.verified))"
    Write-Host "  Gold:     $($script:ConfigData.player_offsets.character.gold.offset) (Verified: $($script:ConfigData.player_offsets.character.gold.verified))"

    Write-Host ""
    Write-Host "Scan History: $($script:ConfigData.scan_history.Count) scans" -ForegroundColor Gray

    Write-Host ""
    Read-Host "Press Enter to continue"
}

#endregion

# Main program
Write-Host ""
Write-Host "Initializing..." -ForegroundColor Cyan
Load-ConfigTemplate

do {
    Show-Menu
    $choice = Read-Host "Enter your choice"

    switch ($choice) {
        "1" { Auto-ScanHP }
        "2" { Auto-ScanMP }
        "3" { Auto-ScanLevel }
        "4" { Auto-ScanGold }
        "5" { Batch-ScanAll }
        "6" { Show-CurrentConfig }
        "7" {
            $savePath = Read-Host "Save path (Enter for default: $ConfigPath)"
            if ([string]::IsNullOrWhiteSpace($savePath)) {
                $savePath = $ConfigPath
            }
            Save-Config -OutputPath $savePath
            Read-Host "Press Enter to continue"
        }
        "8" {
            Write-Host ""
            $saveBeforeExit = Read-Host "Save config before exit? (y/n)"
            if ($saveBeforeExit -eq 'y') {
                Save-Config
            }
            Write-Host "Goodbye!" -ForegroundColor Green
            exit
        }
        default {
            Write-Host "Invalid choice!" -ForegroundColor Red
            Start-Sleep -Seconds 1
        }
    }
} while ($true)
