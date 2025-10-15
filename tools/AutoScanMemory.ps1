# JX1 Auto Memory Scanner - PowerShell Script
# No Python or Lua required - runs natively on Windows
# Usage: Right-click -> Run with PowerShell

param(
    [string]$ProcessName = "JX1",
    [string]$ScanType = "int32"
)

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
                // Check if memory is readable and committed
                if (mbi.State == MEM_COMMIT &&
                    (mbi.Protect == PAGE_READWRITE ||
                     mbi.Protect == PAGE_READONLY ||
                     mbi.Protect == PAGE_EXECUTE_READWRITE)) {

                    byte[] buffer = new byte[(int)mbi.RegionSize];
                    int bytesRead = 0;

                    if (ReadProcessMemory(processHandle, mbi.BaseAddress, buffer, buffer.Length, ref bytesRead)) {
                        // Search for the value in the buffer
                        for (int i = 0; i < bytesRead - 3; i += 4) {
                            int currentValue = BitConverter.ToInt32(buffer, i);
                            if (currentValue == value) {
                                long foundAddress = (long)mbi.BaseAddress + i;
                                results.Add(foundAddress);
                            }
                        }
                    }
                }

                // Move to next memory region
                address = new IntPtr((long)mbi.BaseAddress + (long)mbi.RegionSize);

                // Prevent infinite loop
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

function Export-Results {
    param(
        [array]$Addresses,
        [string]$Filename = "memory_scan_results.txt"
    )

    $Addresses | ForEach-Object { "0x{0:X}" -f $_ } | Out-File -FilePath $Filename
    Write-Host "Results exported to: $Filename" -ForegroundColor Green
}

function Show-Menu {
    Clear-Host
    Write-Host "============================================" -ForegroundColor Green
    Write-Host "   JX1 Auto Memory Scanner (PowerShell)    " -ForegroundColor Green
    Write-Host "============================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "1) Scan for HP (Auto mode)" -ForegroundColor White
    Write-Host "2) Scan for MP (Auto mode)" -ForegroundColor White
    Write-Host "3) Scan for custom value" -ForegroundColor White
    Write-Host "4) Scan for Gold/Money" -ForegroundColor White
    Write-Host "5) Exit" -ForegroundColor White
    Write-Host ""
}

function Auto-ScanHP {
    Write-Host "============================================" -ForegroundColor Cyan
    Write-Host "   Auto HP Scanner" -ForegroundColor Cyan
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

    # Step 2: Filter results
    if ($results.Count -gt 10) {
        Write-Host "[Step 2] Now change your HP (take damage or heal)..." -ForegroundColor Cyan
        $hp2 = Read-Host "Enter your NEW HP value"
        if (-not $hp2) { return }

        $results = Filter-Results -ProcessId $process.Id -Addresses $results -NewValue ([int]$hp2)
        Write-Host "Remaining: $($results.Count) addresses" -ForegroundColor Yellow
        Write-Host ""
    }

    # Step 3: Repeat if needed
    $maxIterations = 5
    $iteration = 3
    while ($results.Count -gt 10 -and $iteration -le $maxIterations) {
        Write-Host "[Step $iteration] Still $($results.Count) results..." -ForegroundColor Cyan
        Write-Host "Change your HP again..." -ForegroundColor Cyan
        $hp3 = Read-Host "Enter your NEW HP value"
        if (-not $hp3) { break }

        $results = Filter-Results -ProcessId $process.Id -Addresses $results -NewValue ([int]$hp3)
        Write-Host "Remaining: $($results.Count) addresses" -ForegroundColor Yellow
        Write-Host ""
        $iteration++
    }

    # Display results
    Write-Host "============================================" -ForegroundColor Green
    if ($results.Count -eq 0) {
        Write-Host "No addresses found!" -ForegroundColor Red
    }
    elseif ($results.Count -le 10) {
        Write-Host "SUCCESS! Found HP address(es):" -ForegroundColor Green
        Write-Host ""
        foreach ($addr in $results) {
            Write-Host ("  0x{0:X}" -f $addr) -ForegroundColor White
        }

        # Export results
        Export-Results -Addresses $results -Filename "hp_addresses.txt"
    }
    else {
        Write-Host "Too many results ($($results.Count)). Try more iterations." -ForegroundColor Yellow
        Export-Results -Addresses $results -Filename "hp_addresses_filtered.txt"
    }
    Write-Host "============================================" -ForegroundColor Green
    Write-Host ""
    Read-Host "Press Enter to continue"
}

function Auto-ScanCustom {
    Write-Host "============================================" -ForegroundColor Cyan
    Write-Host "   Custom Value Scanner" -ForegroundColor Cyan
    Write-Host "============================================" -ForegroundColor Cyan
    Write-Host ""

    # Find process
    $process = Find-Process -Name $ProcessName
    if (-not $process) {
        Write-Host "ERROR: Process '$ProcessName' not found!" -ForegroundColor Red
        return
    }

    Write-Host "Found process: $($process.ProcessName) (PID: $($process.Id))" -ForegroundColor Green
    Write-Host ""

    # Initial scan
    $value1 = Read-Host "Enter the value to scan for"
    if (-not $value1) { return }

    $results = Scan-Memory -ProcessId $process.Id -Value ([int]$value1)
    Write-Host "Found $($results.Count) addresses" -ForegroundColor Yellow
    Write-Host ""

    # Filter loop
    while ($results.Count -gt 10) {
        Write-Host "Still $($results.Count) results. Change the value and scan again." -ForegroundColor Cyan
        $value2 = Read-Host "Enter NEW value (or 'q' to quit)"
        if ($value2 -eq 'q') { break }

        $results = Filter-Results -ProcessId $process.Id -Addresses $results -NewValue ([int]$value2)
        Write-Host "Remaining: $($results.Count) addresses" -ForegroundColor Yellow
        Write-Host ""
    }

    # Display results
    if ($results.Count -gt 0 -and $results.Count -le 10) {
        Write-Host "Found address(es):" -ForegroundColor Green
        foreach ($addr in $results) {
            Write-Host ("  0x{0:X}" -f $addr) -ForegroundColor White
        }
        Export-Results -Addresses $results -Filename "custom_scan_results.txt"
    }

    Read-Host "Press Enter to continue"
}

# Main menu loop
do {
    Show-Menu
    $choice = Read-Host "Enter your choice"

    switch ($choice) {
        "1" { Auto-ScanHP }
        "2" {
            Write-Host "MP Scanner works the same as HP scanner" -ForegroundColor Yellow
            Auto-ScanHP
        }
        "3" { Auto-ScanCustom }
        "4" {
            Write-Host "Gold Scanner works the same as HP scanner" -ForegroundColor Yellow
            Auto-ScanHP
        }
        "5" {
            Write-Host "Goodbye!" -ForegroundColor Green
            exit
        }
        default {
            Write-Host "Invalid choice!" -ForegroundColor Red
            Start-Sleep -Seconds 1
        }
    }
} while ($true)
