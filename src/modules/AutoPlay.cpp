
#include "AutoPlay.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace {
    bool enabled = false;
}

void AutoPlay::toggle() {
    enabled = !enabled;

    log::info(
        "Kae AutoPlay module: {}",
        enabled ? "enabled" : "disabled"
    );

    FLAlertLayer::create(
        "Kae AutoPlay",
        enabled
            ? "Module enabled. Gameplay analysis is not connected yet."
            : "Module disabled.",
        "OK"
    )->show();
}

bool AutoPlay::isEnabled() {
    return enabled;
}

