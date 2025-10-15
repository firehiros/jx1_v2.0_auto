/**
 * @file MacroSystem.h
 * @brief Macro recording and playback system
 *
 * Phase 7 - Advanced Features
 *
 * Allows users to:
 * - Record action sequences
 * - Play back macros
 * - Create custom automation scripts
 * - Conditional logic
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <functional>

namespace JX1 {

/**
 * Macro action types
 */
enum class MacroActionType {
    Attack,           // Attack entity
    UseSkill,         // Use skill
    MoveTo,           // Move to position
    UseItem,          // Use item
    SendChat,         // Send chat
    Wait,             // Wait for duration
    Condition,        // Conditional check
    Loop,             // Loop start
    LoopEnd,          // Loop end
    Custom            // Custom action
};

/**
 * Macro action
 */
struct MacroAction {
    MacroActionType type = MacroActionType::Wait;

    // Parameters (depends on type)
    union {
        struct {
            uint32_t entityID;
        } attack;

        struct {
            uint32_t skillID;
            uint32_t targetID;
        } useSkill;

        struct {
            float x, y, z;
        } moveTo;

        struct {
            int slot;
        } useItem;

        struct {
            char message[256];
        } sendChat;

        struct {
            DWORD duration;  // milliseconds
        } wait;

        struct {
            int loopCount;
        } loop;
    } params;

    std::string description;  // Human-readable description

    MacroAction() { memset(&params, 0, sizeof(params)); }
};

/**
 * Macro - Sequence of actions
 */
struct Macro {
    std::string name;
    std::vector<MacroAction> actions;
    bool enabled = true;
    bool loopInfinite = false;
    int loopCount = 1;
    int currentLoop = 0;
    int currentActionIndex = 0;

    void Reset() {
        currentLoop = 0;
        currentActionIndex = 0;
    }

    bool IsComplete() const {
        if (loopInfinite) return false;
        return currentLoop >= loopCount;
    }
};

/**
 * MacroSystem - Macro recording and playback
 *
 * Usage:
 *   MacroSystem* sys = MacroSystem::Get();
 *
 *   // Record macro
 *   sys->StartRecording("MyMacro");
 *   // ... perform actions ...
 *   sys->StopRecording();
 *
 *   // Play macro
 *   sys->PlayMacro("MyMacro");
 */
class MacroSystem {
public:
    /**
     * Get singleton instance
     */
    static MacroSystem* Get();

    // ========================================
    // Recording
    // ========================================

    /**
     * Start recording macro
     */
    void StartRecording(const char* macroName);

    /**
     * Stop recording
     */
    void StopRecording();

    /**
     * Check if recording
     */
    bool IsRecording() const { return m_recording; }

    /**
     * Add action to current recording
     */
    void RecordAction(const MacroAction& action);

    // ========================================
    // Playback
    // ========================================

    /**
     * Play macro by name
     */
    bool PlayMacro(const char* macroName);

    /**
     * Stop current playback
     */
    void StopPlayback();

    /**
     * Check if playing
     */
    bool IsPlaying() const { return m_playing; }

    /**
     * Update playback (call every frame)
     */
    void Update();

    // ========================================
    // Macro Management
    // ========================================

    /**
     * Save macro to file
     */
    bool SaveMacro(const char* macroName, const char* filename);

    /**
     * Load macro from file
     */
    bool LoadMacro(const char* filename);

    /**
     * Delete macro
     */
    bool DeleteMacro(const char* macroName);

    /**
     * Get all macro names
     */
    std::vector<std::string> GetMacroNames() const;

    /**
     * Get macro by name
     */
    Macro* GetMacro(const char* macroName);

    // ========================================
    // Macro Builder (Code API)
    // ========================================

    /**
     * Create macro programmatically
     */
    Macro* CreateMacro(const char* name);

    /**
     * Add actions to macro
     */
    void AddAttack(Macro* macro, uint32_t entityID);
    void AddUseSkill(Macro* macro, uint32_t skillID, uint32_t targetID = 0);
    void AddMoveTo(Macro* macro, float x, float y, float z);
    void AddUseItem(Macro* macro, int slot);
    void AddSendChat(Macro* macro, const char* message);
    void AddWait(Macro* macro, DWORD duration);
    void AddLoop(Macro* macro, int count);

private:
    MacroSystem() = default;
    ~MacroSystem() = default;
    MacroSystem(const MacroSystem&) = delete;
    MacroSystem& operator=(const MacroSystem&) = delete;

    /**
     * Execute macro action
     */
    bool ExecuteAction(const MacroAction& action);

    /**
     * Find macro by name
     */
    int FindMacroIndex(const char* name) const;

    // ========================================
    // State
    // ========================================

    // Recording
    bool m_recording = false;
    Macro m_recordingMacro;

    // Playback
    bool m_playing = false;
    Macro* m_currentMacro = nullptr;
    DWORD m_actionStartTime = 0;

    // Storage
    std::vector<Macro> m_macros;
};

} // namespace JX1
