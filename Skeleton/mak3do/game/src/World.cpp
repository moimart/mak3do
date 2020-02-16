#include <mak3do/game/World.h>
#include <mak3do/game/GameObject.h>
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
    
    std::vector<std::set<GameObjectPtr>::iterator> m_to_delete;
    std::vector<std::map<std::string, GameObjectPtr>::iterator> m_to_delete_with_id;

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
: m_pimpl(std::make_shared<WorldImpl>(world, scene, this))
{
}

void World::add_object(GameObjectPtr object, const std::string& id)
{
    m_pimpl->add_object(object, id);
}

void World::remove_object(GameObjectPtr object)
{
    m_pimpl->remove_object(object);
}

std::vector<GameObjectPtr> World::objects(const std::string& id) const
{
    return m_pimpl->getObjectsByObjectId(id);
}

GameObjectPtr World::objects_with_id(const std::string& id) const
{
    return m_pimpl->getObjectById(id);
}

CameraPtr World::camera() const
{
    return m_pimpl->default_camera();
}

void World::update(float dt)
{
    m_pimpl->update(dt);
}

void World::start()
{
    m_pimpl->start();
}

void World::pause()
{
    m_pimpl->pause();
}

void World::resume()
{
    m_pimpl->resume();
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
    m_pimpl->updated(callback);
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

    for (auto& element : elements) {
        m_world->add_element(element);
    }

    for (auto node : object->nodes()) {
        m_scene->add_node(node);
    }

    object->parent_world(std::static_pointer_cast<World>(m_parent->shared_from_this()));

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

void WorldImpl::remove_object(GameObjectPtr object)
{
    auto iter = m_objects.find(object);

    GameObjectPtr o;

    if (iter == m_objects.end()) {
        auto iter_map = _find_in_map(object, m_objects_with_ids);
        if (iter_map != m_objects_with_ids.end()) {
            o = iter_map->second;

            if (m_doing_update) {
                m_to_delete_with_id.push_back(iter_map);
            } else {
                m_objects_with_ids.erase(iter_map);
            }
        } else {
            return;
        }
    } else {
        o = *iter;

        if (m_doing_update) {
            m_to_delete.push_back(iter);
        } else {
            m_objects.erase(iter);
        }
    }

    for (auto node : o->nodes()) {
        if (node->parent() != nullptr)
            node->removeFromParent();
    }

    for (auto element : o->elements()) {
        m_world->remove_element(element);
    }

    object->parent_world(nullptr);
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

void WorldImpl::start()
{
    if (!m_world_started) {
        /* TODO: Scheduler!!!
        auto sched = Director::get()->scheduler();
         
        auto callback = std::make_shared<SchedulerCallback>();
         
        callback->lambda = [this](float dt) {
            update(dt);
        };
        callback->cb_id = "world_update";

        sched->schedule(callback);
         
        */

        m_world_started = true;
        m_world_paused = false;
    }
}

void WorldImpl::pause()
{
    m_world_paused = true;
}

void WorldImpl::resume()
{
    m_world_paused = false;
}

/*
 void WorldImpl::addScript(const std::string& id, GameScriptPtr script)
{
    m_scripts[id] = script;
}
 */

std::vector<GameObjectPtr> WorldImpl::getObjectsByObjectId(const std::string& id) const
{
    std::vector<GameObjectPtr> objects_to_return;

    for (auto& object : m_objects) {
        if (object->name() == id) {
            objects_to_return.push_back(object);
        }
    }

    for (auto& kvp : m_objects_with_ids) {
        if (kvp.second->name() == id) {
            objects_to_return.push_back(kvp.second);
        }
    }

    return objects_to_return;
}

GameObjectPtr WorldImpl::getObjectById(const std::string& id) const
{
    auto iter = m_objects_with_ids.find(id);

    if (iter != m_objects_with_ids.end()) {
        return iter->second;
    }

    return nullptr;
}

PhysicsWorldPtr WorldImpl::physics_world() const
{
    return m_world;
}

void WorldImpl::updated(UpdateCallbackPtr callback)
{
    m_callbacks.push_back(callback);
}

void WorldImpl::stop_updated(const std::string& id)
{
    std::remove_if(m_callbacks.begin(),m_callbacks.end(),[&id](UpdateCallbackPtr callback) {
        return callback->cb_id == id;
    });
}

void WorldImpl::stop_updated(UpdateCallbackPtr callback)
{
    std::remove(m_callbacks.begin(),m_callbacks.end(),callback);
}

void WorldImpl::cleanup_updated()
{
    m_callbacks.clear();
}

}
