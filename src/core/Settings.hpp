
#pragma once

namespace Kae {

    class Settings {
    public:
        static bool inputAdjusterEnabled;
        static bool autoPlayEnabled;

        static bool showNotifications;
        static bool darkTheme;

        static void load();
        static void save();
        static void reset();
    };

}
