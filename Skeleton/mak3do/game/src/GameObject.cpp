#include <mak3do/game/GameObject.h>
//#include "GameScript.h"
#include <mak3do/game/PhysicsElement.h>
#include <mak3do/game/World.h>

namespace mak3do {

class GameObjectImpl {
public:
    GameObjectImpl(const std::string& id, GameObject* parent);

    std::string name() const { return m_name; }

    //void addScript(const std::string& id, GameScriptPtr script);
    void add_node(const std::string& id, NodePtr node);
    void add_physics_element(const std::string& name, PhysicsElementPtr element);
    //void addSound(const std::string& id, SoundPtr sound);

    void remove_from_world();

    void position(const Vec3& position);
    Vec3 position() const;

    NodePtr node(const std::string& id);
    PhysicsElementPtr physics_element(const std::string& id);
    //SoundPtr getSound(const std::string& id);

    std::vector<NodePtr> nodes();
    std::vector<PhysicsElementPtr> elements();

    void parent_world(WorldPtr world);
    WorldPtr parent_world() const;

    void update(float dt);

    void rotation(bool enable);
    void disable_physics_coupling(bool disable = true);

private:
    std::map<std::string, NodePtr> m_nodes;
    std::map<std::string, PhysicsElementPtr> m_elements;
    //std::map<std::string, SoundPtr> m_sounds;

    //std::map<std::string, GameScriptPtr> m_scripts;

    std::string m_name;

    Vec3 m_position;
    WorldPtr m_parent_world;

    bool m_rotation_enabled{ true };
    bool m_disable_phys{ false };

    GameObject* m_parent;
};


GameObject::GameObject(const std::string& name)
: m_pimpl(std::make_shared<GameObjectImpl>(name, this))
{
}

void GameObject::add_node(const std::string& id, NodePtr node)
{
    m_pimpl->add_node(id, node);
}

void GameObject::add_physics_element(const std::string& id, PhysicsElementPtr element)
{
    m_pimpl->add_physics_element(id, element);
}

void GameObject::remove_from_world()
{
    m_pimpl->remove_from_world();
}

NodePtr GameObject::node(const std::string& id) const
{
    return m_pimpl->node(id);
}

PhysicsElementPtr GameObject::physics_element(const std::string& id) const
{
    return m_pimpl->physics_element(id);
}

void GameObject::update(float dt)
{
    m_pimpl->update(dt);
}

void GameObject::rotation(bool enable)
{
    m_pimpl->rotation(enable);
}

void GameObject::disable_physics_coupling(bool disable)
{
    m_pimpl->disable_physics_coupling(disable);
}

std::vector<NodePtr> GameObject::nodes()
{
    return m_pimpl->nodes();
}

std::vector<PhysicsElementPtr> GameObject::elements()
{
    return m_pimpl->elements();
}

void GameObject::position(const Vec3& position)
{
    m_pimpl->position(position);
}

Vec3 GameObject::position() const
{
    return m_pimpl->position();
}

void GameObject::parent_world(WorldPtr world)
{
    m_pimpl->parent_world(world);
}

WorldPtr GameObject::world() const
{
    return m_pimpl->parent_world();
}

///////

GameObjectImpl::GameObjectImpl(const std::string& name, GameObject* parent)
: m_name(name)
, m_parent_world(WorldPtr())
, m_parent(parent)
{
}

void GameObjectImpl::add_node(const std::string& id, NodePtr node)
{
    m_nodes[id] = node;
}

void GameObjectImpl::add_physics_element(const std::string& id, PhysicsElementPtr element)
{
    m_elements[id] = element;
}

void GameObjectImpl::remove_from_world()
{
    m_parent_world->remove_object(std::static_pointer_cast<GameObject>(m_parent->shared_from_this()));
}

NodePtr GameObjectImpl::node(const std::string& id)
{
    auto found = m_nodes.find(id);

    if (found == m_nodes.end()) {
        return nullptr;
    }

    return m_nodes[id];
}

PhysicsElementPtr GameObjectImpl::physics_element(const std::string& id)
{
    auto found = m_elements.find(id);

    if (found == m_elements.end()) {
        return nullptr;
    }

    return m_elements[id];
}

void GameObjectImpl::update(float dt)
{

    if (m_elements.size() == 0 || m_disable_phys) {
        for (auto& kvp : m_nodes) {
            kvp.second->position(m_position);
        }
    } else if (m_elements.size() == 1) {
        auto element = m_elements.begin()->second;
        auto position = element->position();

        for (auto& kvp : m_nodes) {
            kvp.second->position(Vec3(position.x * World::meterToPixelRatio,
                position.y * World::meterToPixelRatio,
                position.z * World::meterToPixelRatio));

            if (m_rotation_enabled) {
                kvp.second->rotation(element->rotation());
            }
        }
    } else {
        auto first_element = m_elements.begin()->second;
        auto first_position = first_element->position();

        for (auto& kvp : m_nodes) {
            if (m_elements.find(kvp.first) == m_elements.end()) {
                kvp.second->position(Vec3(first_position.x * World::meterToPixelRatio,
                    first_position.y * World::meterToPixelRatio,
                    first_position.z * World::meterToPixelRatio));

                if (m_rotation_enabled) {
                    kvp.second->rotation(first_element->rotation());
                }
            } else {
                auto position = m_elements[kvp.first]->position();

                kvp.second->position(Vec3(position.x * World::meterToPixelRatio,
                    position.y * World::meterToPixelRatio,
                    position.z * World::meterToPixelRatio));

                kvp.second->rotation(m_elements[kvp.first]->rotation());
            }
        }
    }
}

std::vector<NodePtr> GameObjectImpl::nodes()
{
    std::vector<NodePtr> ret;

    for (auto& kvp : m_nodes) {
        ret.push_back(kvp.second);
    }

    return ret;
}

std::vector<PhysicsElementPtr> GameObjectImpl::elements()
{
    std::vector<PhysicsElementPtr> ret;

    for (auto& kvp : m_elements) {
        ret.push_back(kvp.second);
    }

    return ret;
}

void GameObjectImpl::position(const Vec3& position)
{
    m_position = position;

    for (auto& element : m_elements) {
        element.second->position(position);
    }
}

Vec3 GameObjectImpl::position() const
{
    if (m_elements.size() > 0) {
        return m_elements.begin()->second->position();
    }

    return m_position;
}

void GameObjectImpl::parent_world(WorldPtr world)
{
    if (world != WorldPtr()) {
        //for (auto& callback : m_enter_callbacks.getCallbacks())
        //    callback();
    } else {
        //for (auto& callback : m_exit_callbacks.getCallbacks())
        //    callback();
    }

    m_parent_world = world;
}

WorldPtr GameObjectImpl::parent_world() const
{
    return m_parent_world;
}

void GameObjectImpl::rotation(bool enable)
{
    m_rotation_enabled = enable;
}

void GameObjectImpl::disable_physics_coupling(bool disable)
{
    m_disable_phys = disable;
}

std::string GameObject::name() const
{
    return m_pimpl->name();
}

}
