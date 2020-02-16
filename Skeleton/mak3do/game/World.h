#pragma once

#include "types.h"
#include "PhysicsWorld.h"
#include <mak3do/scenegraph/all>

namespace mak3do {
class WorldImpl;
class World : public Object {
public:
    static float meterToPixelRatio;

    World(ScenePtr scene);
    World(PhysicsWorld::Type type, SceneRef scene);
    World(PhysicsWorldPtr world, SceneRef scene);

    //void addScript(const std::string& id, GameScriptPtr script);

    void start();
    void pause();
    void resume();

    void add_object(GameObjectPtr object, const std::string& id = "");
    void remove_object(GameObjectPtr object);

    virtual CameraPtr camera() const;
    virtual void camera(CameraPtr camera) {}

    std::vector<GameObjectPtr> getObjectsByObjectId(const std::string& id) const;
    GameObjectPtr getObjectById(const std::string& id) const;

    PhysicsWorldPtr physics_world() const;

    void updated(UpdateCallbackPtr callback);
    void stop_updated(const std::string& id);
    void stop_updated(UpdateCallbackPtr callback);
    void cleanup_updated();

    void update(float dt);

protected:
    std::shared_ptr<WorldImpl> m_pImpl;
};
}
