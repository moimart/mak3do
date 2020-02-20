#include "GameController.h"
#include <mak3do/io/apple/GameControllerImpl.h>

namespace mak3do {
namespace io {

GameControllerManager* GameControllerManager::get()
{
    static GameControllerManager* gcm = nullptr;

    if (gcm == nullptr)
        gcm = new GameControllerManager();

    return gcm;
}

GameController::GameController(unsigned int number)
: m_number(number)
{
}

unsigned int GameController::number() const
{
    return m_number;
}

bool GameController::active() const
{
    return true;
}

std::string GameController::name() const
{
    return m_name;
}

void GameController::name(const std::string& name)
{
    m_name = name;
}

std::vector<controller::Button> GameController::supported_buttons() const
{
    return m_buttons;
}

bool GameController::left_stick_supported() const
{
    return m_left_stick_supported;
}

bool GameController::right_stick_supported() const
{
    return m_right_stick_supported;
}

bool GameController::dpad_supported() const
{
    return m_dpad_support_supported;
}

bool GameController::motion_supported() const
{
    return m_motion_supported;
}

bool GameController::touch_supported() const
{
    return m_touch_supported;
}

void GameController::button_pressed(ButtonCallbackPtr callback)
{
    m_press_callbacks.push_back(callback);
}

void GameController::button_released(ButtonCallbackPtr callback)
{
    m_release_callbacks.push_back(callback);
}

void GameController::dpad_pressed(DpadCallbackPtr callback)
{
    m_dpad_press_callbacks.push_back(callback);
}

void GameController::dpad_released(DpadCallbackPtr callback)
{
    m_dpad_release_callbacks.push_back(callback);
}

void GameController::left_stick_changed(AnalogCallbackPtr callback)
{
    m_left_stick_callbacks.push_back(callback);
}

void GameController::right_stick_changed(AnalogCallbackPtr callback)
{
    m_left_stick_callbacks.push_back(callback);
}

void GameController::motion_changed(MotionCallbackPtr callback)
{
    m_motion_callbacks.push_back(callback);
}

void GameController::stop(ButtonCallbackPtr callback)
{
    m_press_callbacks.erase(std::remove(m_press_callbacks.begin(),
                                        m_press_callbacks.end(),
                                        callback),m_press_callbacks.end());
    
    m_release_callbacks.erase(std::remove(m_release_callbacks.begin(),
                                          m_release_callbacks.end(),
                                          callback),m_release_callbacks.end());
}

void GameController::stop(DpadCallbackPtr callback)
{
    m_dpad_press_callbacks.erase(std::remove(m_dpad_press_callbacks.begin(),
                                             m_dpad_press_callbacks.end(),
                                             callback),m_dpad_press_callbacks.end());
    
    m_dpad_release_callbacks.erase(std::remove(m_dpad_release_callbacks.begin(),
                                               m_dpad_release_callbacks.end(),
                                               callback),m_dpad_release_callbacks.end());
}

void GameController::stop(AnalogCallbackPtr callback)
{
    m_left_stick_callbacks.erase(std::remove(m_left_stick_callbacks.begin(),
                                             m_left_stick_callbacks.end(),
                                             callback),m_left_stick_callbacks.end());
    
    m_right_stick_callbacks.erase(std::remove(m_right_stick_callbacks.begin(),
                                              m_right_stick_callbacks.end(),
                                              callback),m_right_stick_callbacks.end());
}

void GameController::stop(MotionCallbackPtr callback)
{
    m_motion_callbacks.erase(std::remove(m_motion_callbacks.begin(),
                                         m_motion_callbacks.end(),
                                         callback),m_motion_callbacks.end());
}

GameControllerManager::GameControllerManager()
: m_pImpl(std::make_shared<GameControllerManagerImpl>(this))
{
}

std::vector<GameControllerPtr> GameControllerManager::controllers() const
{
    return m_pImpl->controllers();
}

std::map<int, GameControllerPtr> GameControllerManager::mapped_controllers() const
{
    return m_pImpl->mapped_controllers();
}

void GameControllerManager::controller_added(ControllerFoundCallbackPtr callback)
{
    m_pImpl->controller_added(callback);
}

void GameControllerManager::stop(ControllerFoundCallbackPtr callback)
{
    m_pImpl->stop(callback);
}

void GameControllerManager::stop_all()
{
    m_pImpl->stop_all();
}

}
}
