#pragma once

#include "../GameController.h"

namespace mak3do {
namespace io {

    class GameControllerManagerImpl {
    public:
        GameControllerManagerImpl(GameControllerManager* m_parent);
        ~GameControllerManagerImpl();

        std::vector<GameControllerPtr> controllers() const;
        std::map<int, GameControllerPtr> mapped_controllers() const;

        void controller_added(ControllerFoundCallbackPtr callback);
        void stop(ControllerFoundCallbackPtr callback);
        void stop_all();

        void setup_controllers(bool reset = false);
        GameControllerPtr setup_controller(int c, void* nc);
        void button(unsigned int controller, controller::Button button, void* nbutton, std::vector< ButtonCallbackPtr>& callbacks);

        void buttons_callback(unsigned int i, void* _gamepad, void* _element, bool extended = false);
        void dpad_direction(unsigned int i, controller::Dpad pad, bool pressed, float value);

        GameControllerManager* m_parent;
        void* m_native;
        std::map<int, GameControllerPtr> m_controllers;
        std::map<int, void*> m_native_controllers;
        std::vector<ControllerFoundCallbackPtr> m_found_callbacks;
    };
}
}
