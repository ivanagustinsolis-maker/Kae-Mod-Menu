
#include "Settings.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace Kae {

    bool Settings::inputAdjusterEnabled = false;
    bool Settings::autoPlayEnabled = false;

    bool Settings::showNotifications = true;
    bool Settings::darkTheme = true;

    void Settings::load() {
        auto& mod = Mod::get();

        inputAdjusterEnabled =
            mod->getSavedValue<bool>("input-adjuster-enabled");

        autoPlayEnabled =
            mod->getSavedValue<bool>("autoplay-enabled");

        showNotifications =
            mod->getSavedValue<bool>("show-notifications", true);

        darkTheme =
            mod->getSavedValue<bool>("dark-theme", true);
    }

    void Settings::save() {
        auto& mod = Mod::get();

        mod->setSavedValue(
            "input-adjuster-enabled",
            inputAdjusterEnabled
        );

        mod->setSavedValue(
            "autoplay-enabled",
            autoPlayEnabled
        );

        mod->setSavedValue(
            "show-notifications",
            showNotifications
        );

        mod->setSavedValue(
            "dark-theme",
            darkTheme
        );
    }

    void Settings::reset() {
        inputAdjusterEnabled = false;
        autoPlayEnabled = false;
        showNotifications = true;
        darkTheme = true;

        save();
    }

}
