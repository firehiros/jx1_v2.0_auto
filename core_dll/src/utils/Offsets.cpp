/**
 * @file Offsets.cpp
 * @brief Implementation of Offsets static members
 */

#include "Offsets.h"

namespace JX1 {
namespace Offsets {

    // Define static members for Functions struct
    // These will be populated at runtime during reverse engineering phase
    uintptr_t Functions::Attack = 0x0;
    uintptr_t Functions::UseSkill = 0x0;
    uintptr_t Functions::StopAttack = 0x0;
    uintptr_t Functions::MoveTo = 0x0;
    uintptr_t Functions::Teleport = 0x0;
    uintptr_t Functions::UseItem = 0x0;
    uintptr_t Functions::PickupItem = 0x0;
    uintptr_t Functions::DropItem = 0x0;
    uintptr_t Functions::SendChat = 0x0;

} // namespace Offsets
} // namespace JX1
