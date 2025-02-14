#include "autohooks/shared/hooks.hpp"
#include "logger.hpp"

// GlobalNamespace
#include "GlobalNamespace/MainMenuViewController.hpp"

// UnityEngine
#include "UnityEngine/GameObject.hpp"

namespace Hooks {
    // Usings
    using namespace GlobalNamespace;

    // Hooks the MainMenuViewController to hide the musicPackPromoBanner object.
    MAKE_LATE_HOOK_MATCH(
        MainMenuViewController_DidActivate,
        &MainMenuViewController::DidActivate,
        void,
        MainMenuViewController* self,
        bool firstActivation,
        bool addedToHierarchy,
        bool screenSystemEnabling
    ) {
        Logger.debug("MainMenuViewController_DidActivate");
        self->____musicPackPromoBanner->get_gameObject()->SetActive(false);
        MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    }
}  // namespace Hooks
