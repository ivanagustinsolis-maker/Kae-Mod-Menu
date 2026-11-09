
#include "InputAdjuster.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace {
    bool enabled = false;
}

void InputAdjuster::toggle() {
    enabled = !enabled;

    log::info(
        "Input Adjuster module: {}",
        enabled ? "enabled" : "disabled"
    );

    FLAlertLayer::create(
        "Input Adjuster",
        enabled
            ? "Module enabled. Original features are not connected yet."
            : "Module disabled.",
        "OK"
    )->show();
}

bool InputAdjuster::isEnabled() {
    return enabled;
}

