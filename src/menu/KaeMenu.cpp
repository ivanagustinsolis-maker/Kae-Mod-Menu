
#include "KaeMenu.hpp"

#include "../modules/InputAdjuster.hpp"
#include "../modules/AutoPlay.hpp"
#include "../core/Settings.hpp"

bool KaeMenu::init() {
    if (!Popup::init(380.f, 270.f))
        return false;

    this->setTitle("KAE MOD-MENU");

    auto size = m_mainLayer->getContentSize();

    auto background = CCLayerColor::create(
        {15, 12, 25, 245},
        size.width,
        size.height
    );

    m_mainLayer->addChild(background, -2);

    auto title = CCLabelBMFont::create(
        "KAE MOD-MENU",
        "bigFont.fnt"
    );

    title->setColor({190, 135, 255});
    title->setScale(0.65f);
    title->setPosition({
        size.width / 2.f,
        size.height - 27.f
    });

    m_mainLayer->addChild(title);

    auto subtitle = CCLabelBMFont::create(
        "UNIFIED GEODE EXPERIENCE",
        "goldFont.fnt"
    );

    subtitle->setScale(0.30f);
    subtitle->setPosition({
        size.width / 2.f,
        size.height - 47.f
    });

    m_mainLayer->addChild(subtitle);

    auto sidebar = CCLayerColor::create(
        {27, 21, 42, 255},
        100.f,
        190.f
    );

    sidebar->setPosition({7.f, 8.f});
    m_mainLayer->addChild(sidebar);

    createPages();

    auto nav = CCMenu::create();
    nav->setPosition({57.f, 166.f});

    addNavButton(nav, "HOME", 0.f,
        menu_selector(KaeMenu::onHome));

    addNavButton(nav, "INPUT", -39.f,
        menu_selector(KaeMenu::onInput));

    addNavButton(nav, "AUTOPLAY", -78.f,
        menu_selector(KaeMenu::onAutoPlay));

    addNavButton(nav, "VISUALS", -117.f,
        menu_selector(KaeMenu::onVisuals));

    addNavButton(nav, "SETTINGS", -156.f,
        menu_selector(KaeMenu::onSettings));

    m_mainLayer->addChild(nav);

    showPage(0);

    return true;
}

void KaeMenu::addNavButton(
    CCMenu* menu,
    const char* title,
    float y,
    SEL_MenuHandler callback
) {
    auto button = CCMenuItemSpriteExtra::create(
        ButtonSprite::create(
            title,
            85,
            true,
            "goldFont.fnt",
            "GJ_button_04.png",
            25.f,
            0.45f
        ),
        this,
        callback
    );

    button->setPosition({0.f, y});
    menu->addChild(button);
}

void KaeMenu::addLabel(
    CCNode* parent,
    const char* text,
    float x,
    float y,
    float scale
) {
    auto label = CCLabelBMFont::create(
        text,
        "bigFont.fnt"
    );

    label->setAnchorPoint({0.f, 0.5f});
    label->setScale(scale);
    label->setPosition({x, y});
    parent->addChild(label);
}

void KaeMenu::addActionButton(
    CCNode* parent,
    const char* title,
    float x,
    float y,
    SEL_MenuHandler callback
) {
    auto menu = CCMenu::create();
    menu->setPosition({x, y});

    auto button = CCMenuItemSpriteExtra::create(
        ButtonSprite::create(title),
        this,
        callback
    );

    menu->addChild(button);
    parent->addChild(menu);
}

void KaeMenu::createPages() {
    for (int i = 0; i < 5; i++) {
        m_pages[i] = CCNode::create();
        m_mainLayer->addChild(m_pages[i]);
    }

    // HOME
    auto home = m_pages[0];

    addLabel(home, "WELCOME TO KAE", 125.f, 205.f, 0.48f);
    addLabel(home, "Your Geometry Dash toolkit.", 125.f, 175.f, 0.34f);
    addLabel(home, "Modules", 125.f, 130.f, 0.42f);
    addLabel(home, "Input Adjuster", 125.f, 103.f, 0.36f);
    addLabel(home, "Kae AutoPlay", 125.f, 78.f, 0.36f);
    addLabel(home, "Version 0.1.0 | Android ARM64", 125.f, 35.f, 0.29f);

    // INPUT ADJUSTER
    auto input = m_pages[1];

    addLabel(input, "INPUT ADJUSTER", 125.f, 205.f, 0.48f);
    addLabel(input, "Input calibration module", 125.f, 174.f, 0.34f);
    addLabel(input, "Module status:", 125.f, 125.f, 0.36f);

    addActionButton(
        input,
        "TOGGLE MODULE",
        245.f,
        85.f,
        menu_selector(KaeMenu::onToggleInput)
    );

    addLabel(input, "Original input features pending integration.", 125.f, 45.f, 0.25f);

    // AUTOPLAY
    auto autoplay = m_pages[2];

    addLabel(autoplay, "KAE AUTOPLAY", 125.f, 205.f, 0.48f);
    addLabel(autoplay, "Gameplay analysis module", 125.f, 174.f, 0.34f);
    addLabel(autoplay, "Cube / Ship", 125.f, 135.f, 0.38f);

    addActionButton(
        autoplay,
        "TOGGLE MODULE",
        245.f,
        85.f,
        menu_selector(KaeMenu::onToggleAutoPlay)
    );

    addLabel(autoplay, "Local demo integration pending.", 125.f, 45.f, 0.29f);

    // VISUALS
    auto visuals = m_pages[3];

    addLabel(visuals, "VISUAL SETTINGS", 125.f, 205.f, 0.48f);
    addLabel(visuals, "Menu theme: Purple Neon", 125.f, 160.f, 0.36f);
    addLabel(visuals, "Interface: Premium", 125.f, 130.f, 0.36f);
    addLabel(visuals, "More visual options coming soon.", 125.f, 75.f, 0.29f);

    // SETTINGS
    auto settings = m_pages[4];

    addLabel(settings, "KAE CORE", 125.f, 205.f, 0.48f);
    addLabel(settings, "Global configuration", 125.f, 170.f, 0.36f);
    addLabel(settings, "Build: 0.1.0", 125.f, 125.f, 0.36f);
    addLabel(settings, "Geode SDK: 5.11.2", 125.f, 95.f, 0.36f);
    addLabel(settings, "Android ARM64", 125.f, 65.f, 0.36f);
}

void KaeMenu::showPage(int page) {
    if (page < 0 || page >= 5)
        return;

    for (int i = 0; i < 5; i++)
        m_pages[i]->setVisible(i == page);
}

void KaeMenu::onHome(CCObject*) {
    showPage(0);
}

void KaeMenu::onInput(CCObject*) {
    showPage(1);
}

void KaeMenu::onAutoPlay(CCObject*) {
    showPage(2);
}

void KaeMenu::onVisuals(CCObject*) {
    showPage(3);
}

void KaeMenu::onSettings(CCObject*) {
    showPage(4);
}

void KaeMenu::onToggleInput(CCObject*) {
    InputAdjuster::toggle();
}

void KaeMenu::onToggleAutoPlay(CCObject*) {
    AutoPlay::toggle();
}

KaeMenu* KaeMenu::create() {
    auto ret = new KaeMenu();

    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

