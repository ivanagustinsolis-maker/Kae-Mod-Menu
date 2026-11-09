
#include "Settings.hpp"

namespace {
    bool purpleTheme = true;
    bool animations = true;
}

bool KaeSettings::getPurpleTheme() {
    return purpleTheme;
}

void KaeSettings::setPurpleTheme(bool value) {
    purpleTheme = value;
}

bool KaeSettings::getAnimations() {
    return animations;
}

void KaeSettings::setAnimations(bool value) {
    animations = value;
}

