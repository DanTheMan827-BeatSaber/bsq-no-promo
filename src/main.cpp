#pragma region Includes
#include "main.hpp"

// GlobalNamespace
#include "GlobalNamespace/MainMenuViewController.hpp"

// UnitEngine
#include "UnityEngine/GameObject.hpp"
#pragma endregion

#pragma region Usings
using namespace GlobalNamespace;
#pragma endregion

#pragma region Hook definitions
// Hooks the MainMenuViewController to hide the musicPackPromoBanner object.
MAKE_HOOK_MATCH(MainMenuViewController_DidActivate, &MainMenuViewController::DidActivate, void, MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    Logger.debug("MainMenuViewController_DidActivate");
    self->____musicPackPromoBanner->get_gameObject()->SetActive(false);
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}
#pragma endregion

#pragma region Mod setup
/// @brief Called at the early stages of game loading
/// @param info
/// @return
MOD_EXPORT_FUNC void setup(CModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;

    Logger.info("Completed setup!");
}

/// @brief Called later on in the game loading - a good time to install function hooks
/// @return
MOD_EXPORT_FUNC void late_load() {
    il2cpp_functions::Init();

    Logger.info("Installing hooks...");

    INSTALL_HOOK(Logger, MainMenuViewController_DidActivate);

    Logger.info("Installed all hooks!");
}
#pragma endregion
