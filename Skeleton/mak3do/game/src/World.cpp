#include <mak3do/game/World.h>
//#include "GameScript.h"
#include <mak3do/game/PhysicsWorld.h>

float mak3do::World::meterToPixelRatio = 1.f; //11.0f;

namespace mak3do {
class WorldImpl {
public:
    WorldImpl(ScenePtr scene, World* parent);
    WorldImpl(PhysicsWorld::Type type, ScenePtr scene, World* parent);
    WorldImpl(PhysicsWorldPtr world, ScenePtr scene, World* parent);
    ~WorldImpl();


    void add_object(GameObjectPtr object, const std::string& id = "");
    void remove_object(GameObjectPtr object);

    //void addScript(const std::string& id, GameScriptPtr script);

    std::vector<GameObjectPtr> getObjectsByObjectId(const std::string& id) const;
    GameObjectPtr getObjectById(const std::string& id) const;

    CameraPtr default_camera() const;

    PhysicsWorldPtr physics_world() const;

    void update(float dt);

    void updated(UpdateCallbackPtr callback);
    void stop_updated(const std::string& id);
    void stop_updated(UpdateCallbackPtr);
    void cleanup_updated();

    void start();
    void pause();
    void resume();

private:
    std::set<GameObjectPtr> m_objects;
    std::map<std::string, GameObjectPtr> m_objects_with_ids;

    //std::map<std::string, GameScriptPtr> m_scripts;

    World* m_parent;
    PhysicsWorldPtr m_world;
    ScenePtr m_scene;

    bool m_world_started { false };
    bool m_world_paused { false };
    bool m_doing_update { false };

    std::vector<UpdateCallbackPtr> m_callbacks;
};

World::World(ScenePtr scene)
: m_pimpl(std::make_shared<WorldImpl>(scene, this))
{
}

World::World(PhysicsWorld::Type type, ScenePtr scene)
: m_pimpl(std::make_shared<WorldImpl>(type, scene, this))
{
}

World::World(PhysicsWorldPtr world, ScenePtr scene)
: m_pImpl(std::make_shared<WorldImpl>(world, scene, this))
{
}

void World::addObject(GameObjectPtr object, const std::string& id)
{
    m_pimpl->addObject(object, id);
}

void World::removeObject(GameObjectPtr object)
{
    m_pimpl->removeObject(object);
}

std::vector<GameObjectPtr> World::getObjectsByObjectId(const std::string& id) const
{
    return m_pimpl->getObjectsByObjectId(id);
}

GameObjectPtr World::getObjectById(const std::string& id) const
{
    return m_pimpl->getObjectById(id);
}

CameraRef World::getDefaultCamera() const
{
    return m_pimpl->default_camera();
}

void World::update(float dt)
{
    m_pImpl->update(dt);
}

void World::start()
{
    m_pImpl->start();
}

void World::pause()
{
    m_pImpl->pause();
}

void World::resume()
{
    m_pImpl->resume();
}

/*
void World::addScript(const std::string& id, GameScriptPtr script)
{
    m_pImpl->addScript(id, script);
}
*/

PhysicsWorldPtr World::physics_world() const
{
    return m_pimpl->physics_world();
}

void World::updated(UpdateCallbackPtr callback)
{
    m_pImpl->updated(callback);
}

void World::stop_updated(const std::string& id)
{
    m_pimpl->stop_updated(id);
}

void World::stop_updated(UpdateCallbackPtr callback)
{
    m_pimpl->stop_updated(callback);
}

void World::cleanup_updated()
{
    m_pimpl->cleanup_updated();
}


WorldImpl::WorldImpl(PhysicsWorldPtr world, ScenePtr scene, World* parent)
: m_world(world)
, m_scene(scene)
, m_parent(parent)
{
}

WorldImpl::WorldImpl(ScenePtr scene, World* parent)
: m_world(mak3do::create<PhysicsWorld>())
, m_scene(scene)
, m_parent(parent)
{
}

WorldImpl::WorldImpl(PhysicsWorld::Type type, ScenePtr scene, World* parent)
: m_world(mak3do::create<PhysicsWorld>(type))
, m_scene(scene)
, m_parent(parent)
{
}

WorldImpl::~WorldImpl()
{
    
}

void WorldImpl::add_object(GameObjectPtr object, const std::string& id)
{
    auto elements = object->elements();

    for (auto& element : elements)
        m_world->addPhysicsElement(element);

    for (auto node : object->nodes()) {
        scene->add_node(node);
    }

    object->parent_world(static_pointer_cast<World>(m_parent->shared_from_this()));
    object->parent(static_pointer_cast<Object>(m_parent->shared_from_this()));

    if (id != "") {
        m_objects_with_ids[id] = object;
    } else {
        m_objects.insert(object);
    }
}

static inline std::map<std::string, GameObjectPtr>::iterator _find_in_map(GameObjectPtr object, std::map<std::string, GameObjectPtr>& _map)
{
    auto iter = _map.begin();
    for (; iter != _map.end(); iter++) {
        if (object == iter->second)
            return iter;
    }

    return _map.end();
}

void WorldImpl::removeObject(GameObjectPtr object)
{
    auto iter = m_objects.find(object);

    GameObjectPtr o;

    if (iter == m_objects.end()) {
        auto iter_map = _find_in_map(object, m_objects_with_ids);
        if (iter_map != m_objects_with_ids.end()) {
            o = iter_map->second;

            if (m_doing_update)
                m_to_delete_with_id.push_back(iter_map);
            else
                m_objects_with_ids.erase(iter_map);
        } else {
            return;
        }
    } else {
        o = *iter;

        if (m_doing_update)
            m_to_delete.push_back(iter);
        else
            m_objects.erase(iter);
    }

    for (auto node : o->nodes()) {
        if (node->parent() != nullptr)
            node->removeFromParent();
    }

    for (auto element : o->elements()) {
        m_world->remove_element(element);
    }

    object->parent_world(nullptr);
    object->parent(nullptr);
}

void WorldImpl::update(float dt)
{
    if (m_world_paused)
        return;

    m_world->update(dt);

    for (auto& cb : m_callbacks) {
        cb->lambda(dt);
    }

    //for (auto& script : m_scripts)
    //    script.second->update(dt);

    m_doing_update = true;
    for (auto& object : m_objects) {
        object->update(dt);
    }
    m_doing_update = false;

    m_doing_update = true;
    for (auto& kvp : m_objects_with_ids) {
        kvp.second->update(dt);
    }
    m_doing_update = false;

    for (auto iter : m_to_delete)  {
        m_objects.erase(iter);
    }
    m_to_delete.clear();

    for (auto iter : m_to_delete_with_id) {
        m_objects_with_ids.erase(iter);
    }
    m_to_delete_with_id.clear();
}

CameraPtr WorldImpl::default_camera() const
{
    return m_scene->camera();
}

void WorldImpl::startWorld()
{
    if (!m_world_started) {
        auto sched = Director::getInstance()->getScheduler();

        m_sched_id = sched->scheduleCallback(nullptr, 0, true, 0, false, [this](float dt) {
            update(dt);
        });

        m_world_started = true;
        m_world_paused = false;
    }
}

void WorldImpl::pauseWorld()
{
    m_world_paused = true;
}

void WorldImpl::resumeWorld()
{
    m_world_paused = false;
}

void WorldImpl::addScript(const std::string& id, GameScriptPtr script)
{
    m_scripts[id] = script;
}

std::vector<GameObjectPtr> WorldImpl::getObjectsByObjectId(const std::string& id) const
{
    std::vector<GameObjectPtr> objects_to_return;

    for (auto& object : m_objects) {
        if (object->getId() == id)
            objects_to_return.push_back(object);
    }

    for (auto& kvp : m_objects_with_ids) {
        if (kvp.second->getId() == id)
            objects_to_return.push_back(kvp.second);
    }

    return objects_to_return;
}

GameObjectPtr WorldImpl::getObjectById(const std::string& id) const
{
    auto iter = m_objects_with_ids.find(id);

    if (iter != m_objects_with_ids.end())
        return iter->second;

    return GameObjectPtr();
}

PhysicsWorldPtr WorldImpl::getPhysicsWorld() const
{
    return m_world;
}

int WorldImpl::onUpdate(const std::string& id, UpdateCallback callback)
{
    return m_update_callbacks.addCallback(id, callback);
}

void WorldImpl::removeUpdateCallback(const std::string& id)
{
    m_update_callbacks.removeCallback(id);
}

void WorldImpl::removeUpdateCallback(int handler)
{
    m_update_callbacks.removeCallback(handler);
}

void WorldImpl::removeAllUpdateCallbacks()
{
    m_update_callbacks.removeAllCallbacks();
}

}
