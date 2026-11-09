
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// Nivel seleccionado durante la sesión
static int g_assistLevel = 100;

// Ventana de Input Adjuster
class InputAdjusterPopup : public Popup {
protected:
    CCLabelBMFont* m_statusLabel = nullptr;

    bool init() {
        if (!Popup::init(320.f, 230.f)) {
            return false;
        }

        this->setTitle("Input Adjuster");

        auto size = m_mainLayer->getContentSize();

        // Descripción
        auto description = CCLabelBMFont::create(
            "Select assistance level",
            "bigFont.fnt"
        );

        description->setScale(0.45f);
        description->setPosition(
            {size.width / 2.f, size.height - 65.f}
        );

        m_mainLayer->addChild(description);

        // Indicador de estado
        m_statusLabel = CCLabelBMFont::create(
            "Assistance: 100%",
            "bigFont.fnt"
        );

        m_statusLabel->setScale(0.48f);
        m_statusLabel->setPosition(
            {size.width / 2.f, size.height - 95.f}
        );

        m_mainLayer->addChild(m_statusLabel);

        // Menú de niveles
        auto menu = CCMenu::create();
        menu->setPosition(
            {size.width / 2.f, size.height / 2.f - 10.f}
        );

        auto button100 = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("100%"),
            this,
            menu_selector(InputAdjusterPopup::on100)
        );

        auto button50 = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("50%"),
            this,
            menu_selector(InputAdjusterPopup::on50)
        );

        auto button10 = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("10%"),
            this,
            menu_selector(InputAdjusterPopup::on10)
        );

        button100->setPosition({-85.f, 0.f});
        button50->setPosition({0.f, 0.f});
        button10->setPosition({85.f, 0.f});

        menu->addChild(button100);
        menu->addChild(button50);
        menu->addChild(button10);

        // Botón de calibración
        auto calibrateButton = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("AUTO CALIBRATE"),
            this,
            menu_selector(InputAdjusterPopup::onCalibrate)
        );

        calibrateButton->setPosition({0.f, -65.f});
        menu->addChild(calibrateButton);

        m_mainLayer->addChild(menu);

        updateStatus();

        return true;
    }

    void updateStatus() {
        if (!m_statusLabel) return;

        if (g_assistLevel == 100) {
            m_statusLabel->setString("Assistance: 100%");
        }
        else if (g_assistLevel == 50) {
            m_statusLabel->setString("Assistance: 50%");
        }
        else {
            m_statusLabel->setString("Assistance: 10%");
        }
    }

    void on100(CCObject*) {
        g_assistLevel = 100;
        updateStatus();
        log::info("Input Adjuster: 100% selected");
    }

    void on50(CCObject*) {
        g_assistLevel = 50;
        updateStatus();
        log::info("Input Adjuster: 50% selected");
    }

    void on10(CCObject*) {
        g_assistLevel = 10;
        updateStatus();
        log::info("Input Adjuster: 10% selected");
    }

    void onCalibrate(CCObject*) {
        FLAlertLayer::create(
            "Input Adjuster",
            "Calibration interface is ready. "
            "Touch input measurement still needs to be implemented.",
            "OK"
        )->show();
    }

public:
    static InputAdjusterPopup* create() {
        auto popup = new InputAdjusterPopup();

        if (popup->init()) {
            popup->autorelease();
            return popup;
        }

        delete popup;
        return nullptr;
    }
};

// Botón en el menú principal de Geometry Dash
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("bottom-menu");

        if (menu) {
            auto button = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("INPUT"),
                this,
                menu_selector(MyMenuLayer::onInputAdjuster)
            );

            button->setID("input-adjuster-button"_spr);

            menu->addChild(button);
            menu->updateLayout();

            log::info("Input Adjuster button added");
        }
        else {
            log::warn("Could not find bottom-menu");
        }

        return true;
    }

    void onInputAdjuster(CCObject*) {
        auto popup = InputAdjusterPopup::create();

        if (popup) {
            popup->show();
        }
    }
};
