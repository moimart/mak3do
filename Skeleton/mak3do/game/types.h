#pragma once

#include <mak3do/scenegraph/all>

namespace mak3do {
class Sound;
class SoundImpl;
class World;
class WorldImpl;
class GameObject;
class GameObjectImpl;
class PhysicsWorld;
class PhysicsWorldImpl;
class PhysicsElement;
class PhysicsWorld2DImpl;
class PhysicsElement2D;
class PhysicsElement2DImpl;
class PhysicsWorld3DImpl;
class PhysicsElement3D;
class PhysicsElement3DImpl;
class GameScript;

typedef std::shared_ptr<Sound> SoundPtr;
typedef std::shared_ptr<GameObject> GameObjectPtr;
typedef std::shared_ptr<PhysicsWorld> PhysicsWorldPtr;
typedef std::shared_ptr<PhysicsElement> PhysicsElementPtr;
typedef std::shared_ptr<World> WorldPtr;
typedef std::shared_ptr<GameScript> GameScriptPtr;
typedef std::shared_ptr<PhysicsElement2D> PhysicsElement2DPtr;
typedef std::shared_ptr<PhysicsElement3D> PhysicsElement3DPtr;

template<class R, class... Args>
struct Callback {
    std::function<R(Args...)> lambda;
    std::string cb_id;
};

struct ColCallback {
    std::function<void(PhysicsElementPtr, PhysicsElementPtr)> lambda;
    std::string cb_id;
};

struct UpdCallback {
    std::function<void(PhysicsElementPtr, PhysicsElementPtr)> lambda;
    std::string cb_id;
};



//typedef std::shared_ptr<Callback<void(PhysicsElementPtr, PhysicsElementPtr)> > CollisionCallbackPtr;
typedef std::shared_ptr<ColCallback> CollisionCallbackPtr;
//typedef std::shared_ptr<Callback<void(float)> > UpdateCallbackPtr;
typedef std::shared_ptr<UpdCallback> UpdateCallbackPtr;

template <class _Tp, class... Args>
std::shared_ptr<_Tp>
create(Args&&... args)
{
    return std::make_shared<_Tp>(std::forward<Args>(args)...);
}
}
