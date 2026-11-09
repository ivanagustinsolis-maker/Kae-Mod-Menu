
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "menu/KaeMenu.hpp"

using namespace geode::prelude;

class $modify(KaeMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        auto menu = this->getChildByID("bottom-menu");

        if (menu) {
            auto button = CCMenuItemSpriteExtra::create(
                ButtonSprite::create(
                    "KAE",
                    50,
                    true,
                    "goldFont.fnt",
                    "GJ_button_04.png",
                    30.f,
                    0.8f
                ),
                this,
                menu_selector(KaeMenuLayer::onOpenKae)
            );

            button->setID("kae-mod-menu-button"_spr);
            menu->addChild(button);
            menu->updateLayout();
        }

        log::info("Kae Mod-Menu loaded.");
        return true;
    }

    void onOpenKae(CCObject*) {
        auto popup = KaeMenu::create();

        if (popup)
            popup->show();
    }
};
n("Kae AutoPlay: bottom-menu not found.");
        }

        return true;
    }

    void onOpenKae(CCObject*) {
        auto popup = KaeAutoPlayPopup::create();

        if (popup)
            popup->show();
    }
};
