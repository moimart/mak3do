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

class TouchManager;
class TouchManagerImpl;

typedef std::shared_ptr<TouchManager> TouchManagerPtr;
typedef std::shared_ptr<TouchManagerImpl> TouchManagerImplPtr;

template <class R>
struct _TouchCallback {
    std::function<R> lambda;
    std::string cb_id;
};

struct Touch {
    Vec2 location { Vec2(-1,-1) };
    unsigned int id { 0 };
};

typedef std::shared_ptr<Touch> TouchPtr;

typedef _TouchCallback<bool(const std::vector<TouchPtr>&)> TouchCallback;

typedef std::shared_ptr<TouchCallback> TouchCallbackPtr;

}
}
