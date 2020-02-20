#include "GameControllerImpl.h"
#include <mutex>
#include <iostream>
#import <GameController/GameController.h>

static std::mutex s_mutex;

typedef std::map<mak3do::io::controller::Button,GCControllerButtonInput*> ButtonInputTable;

@interface ControllerManager : NSObject
{
    mak3do::io::GameControllerManagerImpl* native;
}

- (id) init:(mak3do::io::GameControllerManagerImpl*)_native;
- (void) controllerConnected:(NSNotification *)notification;
- (void) controllerDisconnected:(NSNotification *)notification;

@property (nonatomic, strong) NSArray<GCController *> *controllers;
@property (nonatomic, assign) std::map<GCController*,ButtonInputTable>* tables;

@end

@implementation ControllerManager

- (void) dealloc
{
    delete _tables;

    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:GCControllerDidConnectNotification object:nil];

    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:GCControllerDidDisconnectNotification object:nil];
}

- (void)controllerConnected:(NSNotification *)notification
{
    NSLog(@"Found Controller");
    GCController* controller = (GCController*)[notification object];

    int slot = 0;

    s_mutex.lock();

    if (native->m_controllers.size() > 0)
        while (native->m_controllers.find(slot++) != native->m_controllers.end());

    auto gc = native->setup_controller(slot, (void*)CFBridgingRetain(controller));

    for (auto& callback : native->m_found_callbacks) {
        if (callback->lambda != nullptr) {
            callback->lambda(gc);
        }
    }

    s_mutex.unlock();
}

- (void) controllerDisconnected:(NSNotification *)notification
{
    GCController* controller = (GCController*)[notification object];

    int slot = -1;
    for (auto& kvp : native->m_native_controllers)
    {
        if (kvp.second == (__bridge void *)(controller))
        {
            slot = kvp.first;
            break;
        }
    }

    s_mutex.lock();

    auto iter = native->m_native_controllers.find(slot);
    native->m_native_controllers.erase(iter);

    auto iter2 = native->m_controllers.find(slot);
    native->m_controllers.erase(iter2);

    auto iter3 = _tables->find(controller);
    if (iter3 != _tables->end())
        _tables->erase(iter3);

    s_mutex.unlock();
}

-(id) init:(mak3do::io::GameControllerManagerImpl*)_native
{
    self.controllers = [GCController controllers];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(controllerConnected:)
                                                 name:GCControllerDidConnectNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(controllerDisconnected:)
                                                 name:GCControllerDidDisconnectNotification
                                               object:nil];

    native = _native;

    _tables = new std::map<GCController*,ButtonInputTable>();

    return self;
}

@end

using namespace mak3do;
using namespace mak3do::io;

GameControllerManagerImpl::GameControllerManagerImpl(GameControllerManager* parent)
: m_parent(parent)
{
    auto native = [[ControllerManager alloc] init: this];
    m_native = (void*)CFBridgingRetain(native);

    setup_controllers();
}

GameControllerPtr GameControllerManagerImpl::setup_controller(int c, void* nc)
{
    auto gc = GameControllerPtr(new GameController(c));
    m_controllers[c] = gc;
    m_native_controllers[c] = nc;
    auto ncontroller = (__bridge GCController*)nc;

    [ncontroller setControllerPausedHandler:^(GCController* controller)
     {
        for (auto callback : m_controllers[c]->m_press_callbacks) {
             callback->lambda(controller::Button::PAUSE,0);
        }

        for (auto callback : m_controllers[c]->m_release_callbacks) {
             callback->lambda(controller::Button::PAUSE,0);
        }
     }];

    auto native = (__bridge ControllerManager*)m_native;
    std::map<GCController*,ButtonInputTable>& all_tables = *(native.tables);

    m_controllers[c]->m_buttons.push_back(controller::Button::PAUSE);

    if (ncontroller.extendedGamepad)
    {
        ButtonInputTable table =
        {
            std::make_pair(controller::Button::A, ncontroller.extendedGamepad.buttonA),
            std::make_pair(controller::Button::B, ncontroller.extendedGamepad.buttonB),
            std::make_pair(controller::Button::X, ncontroller.extendedGamepad.buttonX),
            std::make_pair(controller::Button::Y, ncontroller.extendedGamepad.buttonY),
            std::make_pair(controller::Button::L1, ncontroller.extendedGamepad.leftShoulder),
            std::make_pair(controller::Button::R1, ncontroller.extendedGamepad.rightShoulder),
            std::make_pair(controller::Button::L2, ncontroller.extendedGamepad.leftTrigger),
            std::make_pair(controller::Button::R2, ncontroller.extendedGamepad.rightTrigger),
        };

        all_tables[ncontroller] = table;

        m_controllers[c]->m_dpad_support_supported =
        m_controllers[c]->m_left_stick_supported =
        m_controllers[c]->m_right_stick_supported = true;
    }
    else
    if (ncontroller.microGamepad)
    {
        ButtonInputTable table =
        {
            std::make_pair(controller::Button::A, ncontroller.microGamepad.buttonA),
            std::make_pair(controller::Button::X, ncontroller.microGamepad.buttonX)
        };

        m_controllers[c]->m_touch_supported = true;

        all_tables[ncontroller] = table;
    }

    m_controllers[c]->m_motion_supported = (ncontroller.motion);

    if (ncontroller.extendedGamepad != nullptr
        || ncontroller.microGamepad != nullptr)
    {
        for (auto& kvp : all_tables[ncontroller])
            m_controllers[c]->m_buttons.push_back(kvp.first);
    }

    if (ncontroller.extendedGamepad)
    {
        [ncontroller.extendedGamepad setValueChangedHandler:^(GCExtendedGamepad *gamepad, GCControllerElement *element)
         {
            buttons_callback(c,(void*)nc,(__bridge void*)element,true);
         }];

        std::vector<GCControllerDirectionPad*> pads =
        {
            ncontroller.extendedGamepad.leftThumbstick,
            ncontroller.extendedGamepad.dpad
        };

        for (auto pad : pads)
        {
            [pad.left setValueChangedHandler:^(GCControllerButtonInput * _Nonnull button, float value, BOOL pressed)
            {
                dpad_direction(c, controller::Dpad::LEFT, pressed, value);
            }];

            [pad.right setValueChangedHandler:^(GCControllerButtonInput * _Nonnull button, float value, BOOL pressed)
             {
                 dpad_direction(c, controller::Dpad::RIGHT, pressed, value);
             }];

            [pad.up setValueChangedHandler:^(GCControllerButtonInput * _Nonnull button, float value, BOOL pressed)
             {
                 dpad_direction(c, controller::Dpad::UP, pressed, value);
             }];

            [pad.down setValueChangedHandler:^(GCControllerButtonInput * _Nonnull button, float value, BOOL pressed)
             {
                 dpad_direction(c, controller::Dpad::DOWN, pressed, value);
             }];
        }
    }
    else
    if (ncontroller.microGamepad)
    {
        [ncontroller.microGamepad setValueChangedHandler:^(GCMicroGamepad *gamepad, GCControllerElement *element)
         {
            buttons_callback(c,(void*)nc,(__bridge void*)element);
         }];
    }

    /*
    if (false && ncontroller.motion)
    {
        [ncontroller.motion setValueChangedHandler:^(GCMotion* motion)
         {
             auto acc = Vec3(motion.userAcceleration.x,motion.userAcceleration.y,motion.userAcceleration.z);
             auto grv = Vec3(motion.gravity.x,motion.gravity.y,motion.gravity.z);

             for (auto callback : m_controllers[c]->m_motion_callbacks)
                 callback.second(acc,grv);
         }];
    }*/

    return gc;
}

void GameControllerManagerImpl::setup_controllers(bool reset)
{
    auto native = (__bridge ControllerManager*)m_native;

    if (!reset)
        m_controllers.clear();

    for (int c = 0; c < [native.controllers count];c++)
    {
        auto ncontroller = [native.controllers objectAtIndex:c];
        setup_controller(c, (__bridge void*)ncontroller);
    }
}

inline void GameControllerManagerImpl::dpad_direction(unsigned int i, controller::Dpad pad, bool pressed, float value)
{
    if (pressed)
    {
        for (auto& callback : m_controllers[i]->m_dpad_press_callbacks) {
            callback->lambda(pad,value);
        }
    }
    else
    {
        for (auto& callback : m_controllers[i]->m_dpad_release_callbacks) {
            callback->lambda(pad,value);
        }
    }
}

inline void GameControllerManagerImpl::buttons_callback(unsigned int i, void* _controller, void* _element, bool extended)
{
    auto native = (__bridge ControllerManager*)m_native;
    auto nc = (__bridge GCController*)_controller;
    auto element = (__bridge GCControllerElement*)_element;

    if (extended)
    {
        if (element == nc.extendedGamepad.leftThumbstick)
        {
            for (auto& callback : m_controllers[i]->m_left_stick_callbacks) {
                callback->lambda(nc.extendedGamepad.leftThumbstick.xAxis.value,
                                nc.extendedGamepad.leftThumbstick.yAxis.value);
            }
            return;
        }
        else
        if (element == nc.extendedGamepad.rightThumbstick)
        {
            for (auto& callback : m_controllers[i]->m_right_stick_callbacks) {
                callback->lambda(nc.extendedGamepad.leftThumbstick.xAxis.value,
                                nc.extendedGamepad.leftThumbstick.yAxis.value);
            }
            return;
        }
        else
        if (element == nc.extendedGamepad.dpad)
        {
            for (auto& callback : m_controllers[i]->m_left_stick_callbacks) {
                callback->lambda(nc.extendedGamepad.dpad.xAxis.value,
                                 nc.extendedGamepad.dpad.yAxis.value);
            }
        }
    }

    if (m_controllers.size() == 0)
        return;

    auto callbacks = m_controllers[i]->m_press_callbacks;
    auto _button = controller::Button::NONE;
    float value = 0;

    ButtonInputTable table;

    auto t = native.tables->find(nc);
    if (t != native.tables->end())
        table = t->second;

    for (auto& kvp : table)
    {
        if (element == kvp.second)
        {
            _button = kvp.first;
            value = kvp.second.value;
            button(i, _button, (__bridge void*)element, callbacks);
            break;
        }
    }

    for (auto callback : callbacks) {
        callback->lambda(_button,value);
    }
}

GameControllerManagerImpl::~GameControllerManagerImpl()
{
    auto native = (ControllerManager*)CFBridgingRelease(m_native);
    m_native = nullptr;
    native = nullptr;
}

inline void GameControllerManagerImpl::button(unsigned int controller, controller::Button button, void *nbutton, std::vector<ButtonCallbackPtr>& callbacks )
{
    auto actual_button = (__bridge GCControllerButtonInput*)nbutton;
    if (!actual_button.isPressed) {
        callbacks = m_controllers[controller]->m_release_callbacks;
    }
}

std::vector<GameControllerPtr> GameControllerManagerImpl::controllers() const
{
    std::vector<GameControllerPtr> controllers;

    for (auto& kvp : m_controllers)
        controllers.push_back(kvp.second);

    return controllers;
}

std::map<int,GameControllerPtr> GameControllerManagerImpl::mapped_controllers() const
{
    return m_controllers;
}


void GameControllerManagerImpl::controller_added(ControllerFoundCallbackPtr callback)
{
    m_found_callbacks.push_back(callback);
}

void GameControllerManagerImpl::stop(ControllerFoundCallbackPtr callback)
{
   m_found_callbacks.erase(std::remove(m_found_callbacks.begin(),
                                       m_found_callbacks.end(),
                                       callback));
}

void GameControllerManagerImpl::stop_all()
{
    m_found_callbacks.clear();
}
