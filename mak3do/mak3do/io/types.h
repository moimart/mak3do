#pragma once

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <mak3do/math/all>

namespace mak3do {
namespace io {

namespace controller {

enum class Button {
    NONE,
    A,
    X,
    Y,
    B,
    L1,
    R1,
    L2,
    R2,
    PAUSE,
    START
};

enum class Dpad {
    UP,
    DOWN,
    LEFT,
    RIGHT
};
}

class GameController;
class GameControllerImpl;
class GameControllerManager;
class GameControllerManagerImpl;

typedef std::shared_ptr<GameController> GameControllerPtr;
typedef std::shared_ptr<GameControllerManager> GameControllerManagerPtr;
typedef std::shared_ptr<GameControllerImpl> GameControllerImplPtr;
typedef std::shared_ptr<GameControllerManagerImpl> GameControllerManagerImplPtr;

template <class R>
struct _ControllerCallback {
    std::function<R> lambda;
    std::string cb_id;
};
    
typedef _ControllerCallback<void(GameControllerPtr)> ControllerFoundCallback;
typedef std::shared_ptr<ControllerFoundCallback> ControllerFoundCallbackPtr;

typedef _ControllerCallback<void(controller::Button, float)> ButtonCallback;
typedef std::shared_ptr<ButtonCallback> ButtonCallbackPtr;

typedef _ControllerCallback<void(controller::Dpad, float)> DpadCallback;
typedef std::shared_ptr<DpadCallback> DpadCallbackPtr;

typedef _ControllerCallback<void(float, float)> AnalogCallback;
typedef std::shared_ptr<AnalogCallback> AnalogCallbackPtr;

typedef _ControllerCallback<void(const Vec3&, const Vec3&)> MotionCallback;
typedef std::shared_ptr<MotionCallback> MotionCallbackPtr;

class PointerManager;
class PointerManagerImpl;

typedef std::shared_ptr<PointerManager> PointerManagerPtr;
typedef std::shared_ptr<PointerManagerImpl> PointerManagerImplPtr;

template <class R>
struct _PointerEventCallback {
    std::function<R> lambda;
    std::string cb_id;
};

struct PointerEvent {
    Vec2 location { Vec2(-1,-1) };
    unsigned int tid { 0 };
};

struct ScrollEvent {
    float dx { 0 };
    float dy { 0 };
};

typedef std::shared_ptr<PointerEvent> PointerEventPtr;
typedef std::shared_ptr<ScrollEvent> ScrollEventPtr;

typedef _PointerEventCallback<bool(const std::vector<PointerEventPtr>&)> PointerEventCallback;
typedef _PointerEventCallback<void(float,float)> ScrollEventCallback;

typedef std::shared_ptr<PointerEventCallback> PointerEventCallbackPtr;
typedef std::shared_ptr<ScrollEventCallback> ScrollEventCallbackPtr;

}
}
