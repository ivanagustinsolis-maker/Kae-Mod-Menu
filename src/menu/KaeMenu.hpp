
#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class KaeMenu : public Popup {
protected:
    CCNode* m_pages[5] = {};

    bool init();

    void createPages();
    void showPage(int page);

    void addNavButton(
        CCMenu* menu,
        const char* title,
        float y,
        SEL_MenuHandler callback
    );

    void addLabel(
        CCNode* parent,
        const char* text,
        float x,
        float y,
        float scale = 0.42f
    );

    void addActionButton(
        CCNode* parent,
        const char* title,
        float x,
        float y,
        SEL_MenuHandler callback
    );

    void onHome(CCObject*);
    void onInput(CCObject*);
    void onAutoPlay(CCObject*);
    void onVisuals(CCObject*);
    void onSettings(CCObject*);

    void onToggleInput(CCObject*);
    void onToggleAutoPlay(CCObject*);

public:
    static KaeMenu* create();
};

