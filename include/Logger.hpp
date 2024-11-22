#pragma once

#include "beatsaber-hook/shared/utils/logging.hpp"

/// @brief A logger, useful for printing debug messages
/// @return
inline constexpr auto Logger = Paper::ConstLoggerContext(MOD_ID "_" VERSION);
