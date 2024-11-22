#pragma once

#include "scotland2/shared/loader.hpp"

/// @brief Stores the ID and version of our mod, and is sent to the modloader upon startup
inline const modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
