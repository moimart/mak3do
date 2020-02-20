#pragma once

#include "types.h"

namespace mak3do {
namespace io {
    class GameControllerManagerImpl;

    class GameController {
    public:

        unsigned int number() const;
        bool active() const;
        std::string name() const;
        void name(const std::string& name);
        std::vector<controller::Button> supported_buttons() const;
        bool left_stick_supported() const;
        bool right_stick_supported() const;
        bool dpad_supported() const;
        bool motion_supported() const;
        bool touch_supported() const;
        
        void button_pressed(ButtonCallbackPtr callback);
        void button_released(ButtonCallbackPtr callback);
        
        void dpad_pressed(DpadCallbackPtr callback);
        void dpad_released(DpadCallbackPtr callback);

        void left_stick_changed(AnalogCallbackPtr callback);
        void right_stick_changed(AnalogCallbackPtr callback);
        
        void motion_changed(MotionCallbackPtr callback);

        void stop(ButtonCallbackPtr callback);
        void stop(DpadCallbackPtr callback);
        void stop(AnalogCallbackPtr callback);
        void stop(MotionCallbackPtr callback);
        void stop_all();

    private:
        GameController(unsigned int number);

        friend class GameControllerManager;
        friend class GameControllerManagerImpl;

        bool m_active { true };

        std::vector<ButtonCallbackPtr> m_press_callbacks;
        std::vector<ButtonCallbackPtr> m_release_callbacks;
        std::vector<DpadCallbackPtr> m_dpad_press_callbacks;
        std::vector<DpadCallbackPtr> m_dpad_release_callbacks;
        std::vector<AnalogCallbackPtr> m_left_stick_callbacks;
        std::vector<AnalogCallbackPtr> m_right_stick_callbacks;
        std::vector<MotionCallbackPtr> m_motion_callbacks;

        unsigned m_number;
        std::string m_name;
        std::vector<controller::Button> m_buttons;
        bool m_left_stick_supported { false };
        bool m_right_stick_supported { false };
        bool m_dpad_support_supported { false };
        bool m_motion_supported { false };
        bool m_touch_supported { false };
    };

    class GameControllerManager {
    public:
        static GameControllerManager* get();

        std::vector<GameControllerPtr> controllers() const;
        std::map<int, GameControllerPtr> mapped_controllers() const;

        void controller_added(ControllerFoundCallbackPtr callback);
        void stop(ControllerFoundCallbackPtr callback);
        void stop_all();

    private:
        std::shared_ptr<GameControllerManagerImpl> m_pImpl;

        friend class GameControllerManagerImpl;
        
    protected:
        GameControllerManager();
    };
}
}
