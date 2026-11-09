
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <chrono>
#include <vector>
#include <string>

using namespace geode::prelude;

static int g_assistLevel = 100;

class InputAdjusterPopup : public Popup {
protected:
    CCLabelBMFont* m_levelLabel = nullptr;
    CCLabelBMFont* m_progressLabel = nullptr;
    CCLabelBMFont* m_resultLabel = nullptr;
    CCLayerColor* m_progressFill = nullptr;

    bool m_started = false;
    int m_samples = 0;

    std::chrono::steady_clock::time_point m_lastTap;
    std::vector<double> m_intervals;

    bool init() {
        if (!Popup::init(320.f, 270.f)) {
            return false;
        }

        this->setTitle("Input Adjuster");

        auto size = m_mainLayer->getContentSize();

        auto description = CCLabelBMFont::create(
            "Touch calibration",
            "bigFont.fnt"
        );
        description->setScale(0.48f);
        description->setPosition({
            size.width / 2.f,
            size.height - 62.f
        });
        m_mainLayer->addChild(description);

        // Nivel de asistencia
        m_levelLabel = CCLabelBMFont::create(
            "Assistance: 100%",
            "bigFont.fnt"
        );
        m_levelLabel->setScale(0.48f);
        m_levelLabel->setPosition({
            size.width / 2.f,
            size.height - 88.f
        });
        m_mainLayer->addChild(m_levelLabel);

        // Botones de asistencia
        auto menu = CCMenu::create();

        auto b100 = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("100%"),
            this,
            menu_selector(InputAdjusterPopup::on100)
        );

        auto b50 = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("50%"),
            this,
            menu_selector(InputAdjusterPopup::on50)
        );

        auto b10 = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("10%"),
            this,
            menu_selector(InputAdjusterPopup::on10)
        );

        b100->setPosition({-85.f, 0.f});
        b50->setPosition({0.f, 0.f});
        b10->setPosition({85.f, 0.f});

        menu->addChild(b100);
        menu->addChild(b50);
        menu->addChild(b10);

        menu->setPosition({
            size.width / 2.f,
            size.height - 125.f
        });

        m_mainLayer->addChild(menu);

        // Texto de progreso
        m_progressLabel = CCLabelBMFont::create(
            "Samples: 0 / 10",
            "bigFont.fnt"
        );
        m_progressLabel->setScale(0.42f);
        m_progressLabel->setPosition({
            size.width / 2.f,
            size.height - 157.f
        });
        m_mainLayer->addChild(m_progressLabel);

        // Barra de progreso
        auto background = CCLayerColor::create(
            {55, 55, 55, 255},
            220.f,
            10.f
        );

        background->setPosition({
            size.width / 2.f - 110.f,
            size.height - 178.f
        });

        m_mainLayer->addChild(background);

        m_progressFill = CCLayerColor::create(
            {40, 210, 120, 255},
            1.f,
            10.f
        );

        m_progressFill->setAnchorPoint({0.f, 0.5f});
        m_progressFill->setPosition({
            size.width / 2.f - 110.f,
            size.height - 173.f
        });

        m_mainLayer->addChild(m_progressFill);

        // Resultado
        m_resultLabel = CCLabelBMFont::create(
            "Average interval: -- ms",
            "bigFont.fnt"
        );

        m_resultLabel->setScale(0.40f);
        m_resultLabel->setPosition({
            size.width / 2.f,
            size.height - 200.f
        });

        m_mainLayer->addChild(m_resultLabel);

        // Botón de medición
        auto sampleButton = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("TAP TO CALIBRATE"),
            this,
            menu_selector(InputAdjusterPopup::onSample)
        );

        auto sampleMenu = CCMenu::create();
        sampleButton->setPosition({0.f, 0.f});
        sampleMenu->addChild(sampleButton);

        sampleMenu->setPosition({
            size.width / 2.f,
            30.f
        });

        m_mainLayer->addChild(sampleMenu);

        updateLevel();

        return true;
    }

    void updateLevel() {
        if (!m_levelLabel) return;

        std::string text =
            "Assistance: " + std::to_string(g_assistLevel) + "%";

        m_levelLabel->setString(text.c_str());
    }

    void setLevel(int level) {
        g_assistLevel = level;
        updateLevel();

        log::info(
            "Input Adjuster level: {}%",
            g_assistLevel
        );
    }

    void on100(CCObject*) {
        setLevel(100);
    }

    void on50(CCObject*) {
        setLevel(50);
    }

    void on10(CCObject*) {
        setLevel(10);
    }

    void updateProgress() {
        float progress = m_samples / 10.f;

        m_progressFill->setContentSize({
            220.f * progress,
            10.f
        });

        std::string text =
            "Samples: " + std::to_string(m_samples) + " / 10";

        m_progressLabel->setString(text.c_str());
    }

    void onSample(CCObject*) {
        using Clock = std::chrono::steady_clock;

        auto now = Clock::now();

        if (!m_started || m_samples >= 10) {
            m_started = true;
            m_samples = 0;
            m_intervals.clear();
            m_lastTap = now;

            m_resultLabel->setString(
                "Keep tapping the button!"
            );

            updateProgress();
            return;
        }

        double interval =
            std::chrono::duration<double, std::milli>(
                now - m_lastTap
            ).count();

        m_lastTap = now;

        m_intervals.push_back(interval);
        m_samples++;

        updateProgress();

        if (m_samples == 10) {
            double total = 0.0;

            for (double value : m_intervals) {
                total += value;
            }

            double average = total / m_intervals.size();

            std::string result =
                "Average interval: " +
                std::to_string(static_cast<int>(average)) +
                " ms";

            m_resultLabel->setString(result.c_str());

            log::info(
                "Calibration completed. Average interval: {} ms",
                average
            );

            m_started = false;
        }
    }

public:
    static InputAdjusterPopup* create() {
        auto ret = new InputAdjusterPopup();

        if (ret->init()) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};

// Integración en el menú principal
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
