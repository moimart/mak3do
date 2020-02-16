#pragma once

#include "types.h"

#define CREATE_CALLBACK(callback, type)                     \
    int on##callback(const std::string& id, type callback); \
    void remove##callback(const std::string& id);           \
    void remove##callback(int handler);                     \
    void removeAll##callback##Callbacks();

namespace mak3do {

class GameObect;

class GameObject : public std::enable_shared_from_this<GameObect> {
public:
    typedef Callback<void(void)> EnterCallback;
    typedef Callback<void(void)> ExitCallback;

    GameObject(const std::string& id);
    virtual ~GameObject();

    std::string getId() const;

    void addScript(const std::string& id, GameScriptPtr script);
    void addModel(const std::string& id, ModelPtr model);
    void addPhysicsElement(const std::string& id, PhysicsElementPtr element);
    void addSound(const std::string& id, SoundPtr sound);

    int onUpdate(const std::string& id, UpdateCallback callback);
    void removeUpdate(const std::string& id);
    void removeUpdate(int handler);
    void removeAllUpdateCallbacks();

    CREATE_CALLBACK(Enter, EnterCallback);
    CREATE_CALLBACK(Exit, ExitCallback);

    void removeFromWorld();

    void setPosition(const Vec3& position);
    Vec3 getPosition() const;

    void enableRotation(bool enabled = true);
    void disablePhysicsCoupling(bool disable = true);

    WorldPtr getWorld() const;

    //to be called by World
    void update(float dt);

    ModelPtr getModel(const std::string& id) const;
    PhysicsElementPtr getPhysicsElement(const std::string& id) const;
    SoundPtr getSound(const std::string& id) const;

    std::vector<ModelPtr> getAllModels();

private:
    std::shared_ptr<GameObjectImpl> m_pImpl;

    std::vector<PhysicsElementPtr> getAllElements();
    void setParentWorld(WorldPtr world);

    friend class WorldImpl;
};
}
