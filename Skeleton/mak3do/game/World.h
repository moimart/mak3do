#pragma once

#include "types.h"
#include "PhysicsWorld.h"
#include <mak3do/scenegraph/all>

namespace mak3do {

class World : public std::enable_shared_from_this<World> {
public:
    static float meterToPixelRatio;

    World(ScenePtr scene);
    World(PhysicsWorld::Type type, ScenePtr scene);
    World(PhysicsWorldPtr world, ScenePtr scene);

    //void addScript(const std::string& id, GameScriptPtr script);

    void start();
    void pause();
    void resume();

    void add_object(GameObjectPtr object, const std::string& id = "");
    void remove_object(GameObjectPtr object);

    virtual CameraPtr camera() const;
    virtual void camera(CameraPtr camera) {}

    std::vector<GameObjectPtr> objects(const std::string& name) const;
    GameObjectPtr objects_with_id(const std::string& id) const;

    PhysicsWorldPtr physics_world() const;

    void updated(UpdateCallbackPtr callback);
    void stop_updated(const std::string& id);
    void stop_updated(UpdateCallbackPtr callback);
    void cleanup_updated();

    void update(float dt);

protected:
    std::shared_ptr<WorldImpl> m_pimpl;
};
}
