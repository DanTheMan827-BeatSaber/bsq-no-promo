#include "Hooking.hpp"
#include "Logger.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "main.hpp"
#include "modInfo.hpp"
#include "scotland2/shared/modloader.h"

/// @brief Called at the early stages of game loading
/// @param info The mod info.  Update this with your mod's info.
/// @return
MOD_EXPORT_FUNC void setup(CModInfo& info) {
    // Convert the mod info to a C struct and set that as the modloader info.
    info = modInfo.to_c();

    Logger.info("Completed setup!");
}

/// @brief Called early on in the game loading
/// @return
MOD_EXPORT_FUNC void load() {
    // Initialize il2cpp functions
    il2cpp_functions::Init();

    // install early hooks
    Logger.info("Installing early hooks");
    INSTALL_EARLY_HOOKS();
    Logger.info("Finished installing early hooks");
}

/// @brief Called later on in the game loading - a good time to install function hooks
/// @return
MOD_EXPORT_FUNC void late_load() {
    // Install late hooks
    Logger.info("Installing late hooks");
    INSTALL_LATE_HOOKS();
    Logger.info("Finished installing late hooks");
}
