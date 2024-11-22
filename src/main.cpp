#include "main.hpp"
#include "AutoHooks.hpp"
#include "Logger.hpp"
#include "modInfo.hpp"

/// @brief Called at the early stages of game loading
/// @param info
/// @return
MOD_EXPORT_FUNC void setup(CModInfo& info) {
    info = modInfo.to_c();

    Logger.info("Completed setup!");
}

/// @brief Called later on in the game loading - a good time to install function hooks
/// @return
MOD_EXPORT_FUNC void late_load() {
    il2cpp_functions::Init();

    Logger.info("Installing hooks...");
    AutoHooks::InstallHooks();
    Logger.info("Installed all hooks!");
}
