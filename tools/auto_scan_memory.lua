-- Cheat Engine Auto Memory Scanner Script
-- Usage: Load this script in Cheat Engine -> Table -> Show Cheat Table Lua Script

-- Configuration
local PROCESS_NAME = "JX1.exe"
local SCAN_TYPE = vtDword -- vtByte, vtWord, vtDword, vtQword, vtSingle, vtDouble, vtString
local SCAN_OPTION = soExactValue -- soUnknownValue, soValueBetween, soBiggerThan, soSmallerThan

-- Function to attach to process
function attachToProcess(processName)
    local pid = getProcessIDFromProcessName(processName)
    if pid == nil then
        print("Process not found: " .. processName)
        return false
    end

    openProcess(pid)
    print("Attached to process: " .. processName .. " (PID: " .. pid .. ")")
    return true
end

-- Function to perform initial scan
function performScan(valueToScan, scanType, scanOption)
    print("Starting scan for value: " .. tostring(valueToScan))

    local scan = createMemScan()
    scan.firstScan(scanOption, scanType, rtRounded, tostring(valueToScan), "", 0, 0xFFFFFFFFFFFFFFFF, "*W", fsmNotAligned, "", false, false, false, false)
    scan.waitTillDone()

    local foundCount = foundlist_getCount()
    print("Found " .. foundCount .. " results")

    return scan
end

-- Function to perform next scan (filter results)
function nextScan(valueToScan, scanOption)
    print("Filtering results for value: " .. tostring(valueToScan))

    local scan = createMemScan()
    scan.nextScan(scanOption, rtRounded, tostring(valueToScan), "", false, false, false, false, fsmNotAligned)
    scan.waitTillDone()

    local foundCount = foundlist_getCount()
    print("Remaining results: " .. foundCount)

    return foundCount
end

-- Function to add results to address list
function addResultsToList(maxResults)
    maxResults = maxResults or 100
    local count = foundlist_getCount()

    if count > maxResults then
        print("Too many results (" .. count .. "), limiting to " .. maxResults)
        count = maxResults
    end

    print("Adding " .. count .. " results to address list")

    for i = 0, count - 1 do
        local address = foundlist_getAddress(i)
        local memrec = addresslist_createMemoryRecord()
        memrec.Address = string.format("%X", address)
        memrec.Type = SCAN_TYPE
    end

    print("Added results to address list")
end

-- Function to export results to file
function exportResults(filename)
    local count = foundlist_getCount()
    local file = io.open(filename, "w")

    if file == nil then
        print("Failed to open file: " .. filename)
        return
    end

    file:write("Address,Value\n")

    for i = 0, count - 1 do
        local address = foundlist_getAddress(i)
        local value = readInteger(address)
        file:write(string.format("0x%X,%d\n", address, value or 0))
    end

    file:close()
    print("Exported " .. count .. " results to " .. filename)
end

-- Example: Auto-scan workflow
function autoScanHP()
    print("=== Auto HP Scanner ===")

    -- Step 1: Attach to process
    if not attachToProcess(PROCESS_NAME) then
        return
    end

    -- Step 2: Initial scan
    print("\n[Step 1] Enter your current HP value and press Enter")
    local hp1 = tonumber(io.read())
    performScan(hp1, vtDword, soExactValue)

    -- Step 3: Wait and rescan
    print("\n[Step 2] Change your HP (take damage/heal), then enter new HP value:")
    local hp2 = tonumber(io.read())
    nextScan(hp2, soExactValue)

    -- Step 4: Repeat if needed
    local foundCount = foundlist_getCount()
    while foundCount > 10 do
        print("\n[Step 3] Still " .. foundCount .. " results. Change HP again and enter new value:")
        local hp3 = tonumber(io.read())
        foundCount = nextScan(hp3, soExactValue)
    end

    -- Step 5: Add to list
    if foundCount > 0 and foundCount <= 10 then
        print("\nFound HP address(es)!")
        addResultsToList(10)
        exportResults("hp_addresses.csv")
    else
        print("\nNo results found or too many results")
    end
end

-- Example: Scan unknown value with changed/unchanged
function autoScanUnknown()
    print("=== Auto Unknown Value Scanner ===")

    if not attachToProcess(PROCESS_NAME) then
        return
    end

    -- Initial unknown scan
    print("Starting unknown value scan...")
    performScan("", vtDword, soUnknownValue)

    -- Changed value scan
    print("\nMake the value change, then press Enter")
    io.read()
    nextScan("", soChanged)

    -- Unchanged scan
    print("\nKeep the value unchanged, then press Enter")
    io.read()
    nextScan("", soUnchanged)

    -- Repeat
    local foundCount = foundlist_getCount()
    while foundCount > 100 do
        print("\nStill " .. foundCount .. " results")
        print("1) Value changed")
        print("2) Value unchanged")
        local choice = tonumber(io.read())

        if choice == 1 then
            foundCount = nextScan("", soChanged)
        else
            foundCount = nextScan("", soUnchanged)
        end
    end

    if foundCount > 0 and foundCount <= 100 then
        addResultsToList(100)
    end
end

-- Print menu
print("=================================")
print("Cheat Engine Auto Scanner")
print("=================================")
print("Available functions:")
print("  autoScanHP() - Scan for HP value")
print("  autoScanUnknown() - Scan for unknown values")
print("=================================")
