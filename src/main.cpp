
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace {
    enum class Page {
        Control,
        Settings,
        Stats,
        About
    };

    static bool g_demoArmed = false;
    static bool g_cubeSelected = true;
    static bool g_smartDetection = true;
    static bool g_autoRestart = false;
}

class KaeAutoPlayPopup : public Popup {
protected:
    CCNode* m_pages[4] = {};
    CCLabelBMFont* m_status = nullptr;
    CCLabelBMFont* m_mode = nullptr;
    CCLabelBMFont* m_detection = nullptr;

    Page m_currentPage = Page::Control;

    bool init() {
        if (!Popup::init(360.f, 270.f))
            return false;

        this->setTitle("KAE AUTOPLAY");

        auto size = m_mainLayer->getContentSize();

        // Main background
        auto background = CCLayerColor::create(
            {15, 13, 25, 245},
            size.width,
            size.height
        );
        m_mainLayer->addChild(background, -2);

        // Header
        auto title = CCLabelBMFont::create(
            "KAE AUTOPLAY",
            "bigFont.fnt"
        );
        title->setScale(0.65f);
        title->setColor({190, 130, 255});
        title->setPosition({
            size.width / 2.f,
            size.height - 27.f
        });
        m_mainLayer->addChild(title);

        auto version = CCLabelBMFont::create(
            "SMART GAMEPLAY ANALYSIS  |  v0.1.0",
            "goldFont.fnt"
        );
        version->setScale(0.27f);
        version->setPosition({
            size.width / 2.f,
            size.height - 45.f
        });
        m_mainLayer->addChild(version);

        // Sidebar
        auto sidebar = CCLayerColor::create(
            {24, 20, 39, 255},
            86.f,
            190.f
        );
        sidebar->setPosition({8.f, 8.f});
        m_mainLayer->addChild(sidebar);

        createPages();

        // Navigation
        auto nav = CCMenu::create();
        nav->setPosition({51.f, 160.f});

        addNavButton(nav, "CONTROL", -0.f,
            menu_selector(KaeAutoPlayPopup::onControl));

        addNavButton(nav, "SETTINGS", -42.f,
            menu_selector(KaeAutoPlayPopup::onSettings));

        addNavButton(nav, "STATS", -84.f,
            menu_selector(KaeAutoPlayPopup::onStats));

        addNavButton(nav, "ABOUT", -126.f,
            menu_selector(KaeAutoPlayPopup::onAbout));

        m_mainLayer->addChild(nav);

        showPage(Page::Control);
        updateStatus();

        return true;
    }

    void addNavButton(
        CCMenu* menu,
        const char* text,
        float y,
        SEL_MenuHandler callback
    ) {
        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(text, 75, true, "goldFont.fnt", "GJ_button_04.png", 25.f, 0.45f),
            this,
            callback
        );

        button->setPosition({0.f, y});
        menu->addChild(button);
    }

    CCLabelBMFont* addLabel(
        CCNode* parent,
        const char* text,
        float x,
        float y,
        float scale = 0.42f
    ) {
        auto label = CCLabelBMFont::create(
            text,
            "bigFont.fnt"
        );

        label->setScale(scale);
        label->setAnchorPoint({0.f, 0.5f});
        label->setPosition({x, y});
        parent->addChild(label);

        return label;
    }

    CCMenuItemSpriteExtra* addButton(
        CCNode* parent,
        const char* text,
        float x,
        float y,
        SEL_MenuHandler callback
    ) {
        auto menu = CCMenu::create();
        menu->setPosition({x, y});

        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(text),
            this,
            callback
        );

        menu->addChild(button);
        parent->addChild(menu);

        return button;
    }

    void createPages() {
        auto size = m_mainLayer->getContentSize();

        for (int i = 0; i < 4; i++) {
            m_pages[i] = CCNode::create();
            m_pages[i]->setPosition({0.f, 0.f});
            m_mainLayer->addChild(m_pages[i]);
        }

        // CONTROL PAGE
        auto control = m_pages[0];

        addLabel(control, "AUTOPLAY STATUS", 108.f, 218.f, 0.42f);

        m_status = addLabel(
            control,
            "DISABLED",
            108.f,
            194.f,
            0.55f
        );
        m_status->setColor({180, 180, 190});

        addLabel(control, "MODE SELECTION", 108.f, 157.f, 0.42f);

        addButton(
            control, "CUBE", 155.f, 132.f,
            menu_selector(KaeAutoPlayPopup::onCube)
        );

        addButton(
            control, "SHIP", 255.f, 132.f,
            menu_selector(KaeAutoPlayPopup::onShip)
        );

        m_mode = addLabel(
            control,
            "Selected: Cube",
            108.f,
            105.f,
            0.38f
        );

        addLabel(
            control,
            "SMART DETECTION",
            108.f,
            73.f,
            0.38f
        );

        m_detection = addLabel(
            control,
            "Detector: NOT CONNECTED",
            108.f,
            52.f,
            0.32f
        );
        m_detection->setColor({255, 190, 100});

        addButton(
            control, "START", 165.f, 20.f,
            menu_selector(KaeAutoPlayPopup::onStart)
        );

        addButton(
            control, "STOP", 270.f, 20.f,
            menu_selector(KaeAutoPlayPopup::onStop)
        );

        // SETTINGS PAGE
        auto settings = m_pages[1];

        addLabel(settings, "CONFIGURATION", 108.f, 218.f, 0.48f);

        addLabel(settings, "Smart Detection", 108.f, 174.f, 0.42f);

        addButton(
            settings, "TOGGLE", 265.f, 174.f,
            menu_selector(KaeAutoPlayPopup::onToggleDetection)
        );

        addLabel(settings, "Auto Restart", 108.f, 128.f, 0.42f);

        addButton(
            settings, "TOGGLE", 265.f, 128.f,
            menu_selector(KaeAutoPlayPopup::onToggleRestart)
        );

        addLabel(
            settings,
            "Detection settings are experimental.",
            108.f,
            70.f,
            0.30f
        );

        // STATS PAGE
        auto stats = m_pages[2];

        addLabel(stats, "SESSION STATISTICS", 108.f, 218.f, 0.48f);
        addLabel(stats, "Progress: --", 108.f, 175.f, 0.42f);
        addLabel(stats, "Obstacles detected: --", 108.f, 145.f, 0.38f);
        addLabel(stats, "Actions calculated: --", 108.f, 115.f, 0.38f);
        addLabel(stats, "No gameplay session yet.", 108.f, 65.f, 0.32f);

        // ABOUT PAGE
        auto about = m_pages[3];

        addLabel(about, "KAE AUTOPLAY", 108.f, 218.f, 0.50f);
        addLabel(about, "Version: 0.1.0", 108.f, 177.f, 0.40f);
        addLabel(about, "Developer: Kae", 108.f, 147.f, 0.40f);
        addLabel(about, "Android ARM64", 108.f, 117.f, 0.40f);
        addLabel(about, "Geometry Dash / Geode", 108.f, 87.f, 0.34f);
        addLabel(about, "Experimental development build", 108.f, 48.f, 0.29f);
    }

    void showPage(Page page) {
        m_currentPage = page;

        for (int i = 0; i < 4; i++) {
            m_pages[i]->setVisible(i == static_cast<int>(page));
        }
    }

    void updateStatus() {
        if (!m_status || !m_mode)
            return;

        m_status->setString(
            g_demoArmed ? "DEMO ARMED" : "DISABLED"
        );

        m_status->setColor(
            g_demoArmed
                ? ccColor3B{190, 130, 255}
                : ccColor3B{180, 180, 190}
        );

        m_mode->setString(
            g_cubeSelected ? "Selected: Cube" : "Selected: Ship"
        );

        m_detection->setString(
            g_smartDetection
                ? "Detector: NOT CONNECTED"
                : "Smart Detection: OFF"
        );
    }

    void onControl(CCObject*) {
        showPage(Page::Control);
    }

    void onSettings(CCObject*) {
        showPage(Page::Settings);
    }

    void onStats(CCObject*) {
        showPage(Page::Stats);
    }

    void onAbout(CCObject*) {
        showPage(Page::About);
    }

    void onCube(CCObject*) {
        g_cubeSelected = true;
        updateStatus();
    }

    void onShip(CCObject*) {
        g_cubeSelected = false;
        updateStatus();
    }

    void onToggleDetection(CCObject*) {
        g_smartDetection = !g_smartDetection;
        updateStatus();
    }

    void onToggleRestart(CCObject*) {
        g_autoRestart = !g_autoRestart;

        FLAlertLayer::create(
            "Kae AutoPlay",
            g_autoRestart
                ? "Auto Restart preference enabled."
                : "Auto Restart preference disabled.",
            "OK"
        )->show();
    }

    void onStart(CCObject*) {
        g_demoArmed = true;
        updateStatus();

        FLAlertLayer::create(
            "Kae AutoPlay",
            "Interface preview enabled.\n"
            "Gameplay detector is not implemented yet.",
            "OK"
        )->show();
    }

    void onStop(CCObject*) {
        g_demoArmed = false;
        updateStatus();
    }

public:
    static KaeAutoPlayPopup* create() {
        auto ret = new KaeAutoPlayPopup();

        if (ret->init()) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};

class $modify(KaeMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        auto menu = this->getChildByID("bottom-menu");

        if (menu) {
            auto button = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("KAE", 50, true, "goldFont.fnt", "GJ_button_04.png", 30.f, 0.8f),
                this,
                menu_selector(KaeMenuLayer::onOpenKae)
            );

            button->setID("kae-autoplay-button"_spr);
            menu->addChild(button);
            menu->updateLayout();

            log::info("Kae AutoPlay button added.");
        }
        else {
            log::warn("Kae AutoPlay: bottom-menu not found.");
        }

        return true;
    }

    void onOpenKae(CCObject*) {
        auto popup = KaeAutoPlayPopup::create();

        if (popup)
            popup->show();
    }
};
