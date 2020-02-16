#include "GameObject.h"
#include "../common/CallbackManager.h"
#include "../common/CocosHelper.h"
#include "GameScript.h"
#include "PhysicsElement.h"
#include "World.h"

#define CREATE_CALLBACK_IMPL(callback, type)                           \
    int GameObject::on##callback(const std::string& id, type callback) \
    {                                                                  \
        return m_pImpl->on##callback(id, callback);                    \
    }                                                                  \
    void GameObject::remove##callback(const std::string& id)           \
    {                                                                  \
        m_pImpl->remove##callback(id);                                 \
    }                                                                  \
    void GameObject::remove##callback(int handler)                     \
    {                                                                  \
        m_pImpl->remove##callback(handler);                            \
    }                                                                  \
    void GameObject::removeAll##callback##Callbacks()                  \
    {                                                                  \
        m_pImpl->removeAll##callback##Callbacks();                     \
    }

#define CREATE_CALLBACK_PIMPL(callback, type, object)                      \
    int GameObjectImpl::on##callback(const std::string& id, type callback) \
    {                                                                      \
        return object.addCallback(id, callback);                           \
    }                                                                      \
    void GameObjectImpl::remove##callback(const std::string& id)           \
    {                                                                      \
        object.removeCallback(id);                                         \
    }                                                                      \
    void GameObjectImpl::remove##callback(int handler)                     \
    {                                                                      \
        m_update_callbacks.removeCallback(handler);                        \
    }                                                                      \
    void GameObjectImpl::removeAll##callback##Callbacks()                  \
    {                                                                      \
        m_update_callbacks.removeAllCallbacks();                           \
    }

using namespace mak3do;

namespace mak3do {
class GameObjectImpl {
public:
    GameObjectImpl(const std::string& id, GameObject* parent);

    std::string getId() const { return m_id; }

    void addScript(const std::string& id, GameScriptPtr script);
    void addModel(const std::string& id, ModelPtr model);
    void addPhysicsElement(const std::string& id, PhysicsElementPtr element);
    void addSound(const std::string& id, SoundPtr sound);

    CREATE_CALLBACK(Update, UpdateCallback);
    CREATE_CALLBACK(Enter, GameObject::EnterCallback);
    CREATE_CALLBACK(Exit, GameObject::ExitCallback);

    void removeFromWorld();

    void setPosition(const Vec3& position);
    Vec3 getPosition() const;

    ModelPtr getModel(const std::string& id);
    PhysicsElementPtr getPhysicsElement(const std::string& id);
    SoundPtr getSound(const std::string& id);

    std::vector<ModelPtr> getAllModels();
    std::vector<PhysicsElementPtr> getAllElements();

    void setParentWorld(WorldPtr world);
    WorldPtr getParentWorld() const;

    void update(float dt);

    void enableRotation(bool enable);
    void disablePhysicsCoupling(bool disable = true);

private:
    CallbackManager<UpdateCallback> m_update_callbacks;
    CallbackManager<GameObject::EnterCallback> m_enter_callbacks;
    CallbackManager<GameObject::ExitCallback> m_exit_callbacks;

    std::map<std::string, ModelPtr> m_models;
    std::map<std::string, PhysicsElementPtr> m_elements;
    std::map<std::string, SoundPtr> m_sounds;

    std::map<std::string, GameScriptPtr> m_scripts;

    std::string m_id;

    Vec3 m_position;
    WorldPtr m_parent_world;

    bool m_rotation_enabled{ true };
    bool m_disable_phys{ false };

    GameObject* m_parent;
};
}

GameObject::GameObject(const std::string& id)
    : m_pImpl(std::make_shared<GameObjectImpl>(id, this))
{
}

GameObject::~GameObject()
{
}

void GameObject::addModel(const std::string& id, ModelPtr model)
{
    m_pImpl->addModel(id, model);
}

void GameObject::addPhysicsElement(const std::string& id, PhysicsElementPtr element)
{
    m_pImpl->addPhysicsElement(id, element);
}

CREATE_CALLBACK_IMPL(Update, UpdateCallback);
CREATE_CALLBACK_IMPL(Enter, EnterCallback);
CREATE_CALLBACK_IMPL(Exit, ExitCallback);

void GameObject::removeFromWorld()
{
    m_pImpl->removeFromWorld();
}

ModelPtr GameObject::getModel(const std::string& id) const
{
    return m_pImpl->getModel(id);
}

PhysicsElementPtr GameObject::getPhysicsElement(const std::string& id) const
{
    return m_pImpl->getPhysicsElement(id);
}

SoundPtr GameObject::getSound(const std::string& id) const
{
    return m_pImpl->getSound(id);
}

void GameObject::update(float dt)
{
    m_pImpl->update(dt);
}

void GameObject::enableRotation(bool enable)
{
    m_pImpl->enableRotation(enable);
}

void GameObject::disablePhysicsCoupling(bool disable)
{
    m_pImpl->disablePhysicsCoupling(disable);
}

std::vector<ModelPtr> GameObject::getAllModels()
{
    return m_pImpl->getAllModels();
}

std::vector<PhysicsElementPtr> GameObject::getAllElements()
{
    return m_pImpl->getAllElements();
}

void GameObject::setPosition(const Vec3& position)
{
    m_pImpl->setPosition(position);
}

Vec3 GameObject::getPosition() const
{
    return m_pImpl->getPosition();
}

void GameObject::setParentWorld(WorldPtr world)
{
    m_pImpl->setParentWorld(world);
}

WorldPtr GameObject::getWorld() const
{
    return m_pImpl->getParentWorld();
}

void GameObject::addScript(const std::string& id, GameScriptPtr script)
{
    m_pImpl->addScript(id, script);
}

void GameObject::addSound(const std::string& id, SoundPtr sound)
{
    m_pImpl->addSound(id, sound);
}

///////

GameObjectImpl::GameObjectImpl(const std::string& id, GameObject* parent)
    : m_id(id)
    , m_parent_world(WorldPtr())
    , m_parent(parent)
{
}

void GameObjectImpl::addModel(const std::string& id, ModelPtr model)
{
    m_models[id] = model;
}

void GameObjectImpl::addPhysicsElement(const std::string& id, PhysicsElementPtr element)
{
    m_elements[id] = element;
}

CREATE_CALLBACK_PIMPL(Update, UpdateCallback, m_update_callbacks);
CREATE_CALLBACK_PIMPL(Enter, GameObject::EnterCallback, m_enter_callbacks);
CREATE_CALLBACK_PIMPL(Exit, GameObject::ExitCallback, m_exit_callbacks);

void GameObjectImpl::removeFromWorld()
{
    m_parent_world->removeObject(static_pointer_cast<GameObject>(m_parent->shared_from_this()));
}

ModelPtr GameObjectImpl::getModel(const std::string& id)
{
    auto found = m_models.find(id);

    if (found == m_models.end())
        return ModelPtr();

    return m_models[id];
}

PhysicsElementPtr GameObjectImpl::getPhysicsElement(const std::string& id)
{
    auto found = m_elements.find(id);

    if (found == m_elements.end())
        return PhysicsElementPtr();

    return m_elements[id];
}

SoundPtr GameObjectImpl::getSound(const std::string& id)
{
    auto found = m_sounds.find(id);

    if (found == m_sounds.end())
        return SoundPtr();

    return m_sounds[id];
}

void GameObjectImpl::update(float dt)
{
    for (auto& callback : m_update_callbacks.getCallbacks())
        callback(dt);

    for (auto& script : m_scripts)
        script.second->update(dt);

    if (m_elements.size() == 0 || m_disable_phys) {
        for (auto& kvp : m_models) {
            kvp.second->setPosition(m_position);
        }
    } else if (m_elements.size() == 1) {
        auto element = m_elements.begin()->second;
        auto position = element->getPosition();

        for (auto& kvp : m_models) {
            kvp.second->setPosition(Vec3(position.x * World::meterToPixelRatio,
                position.y * World::meterToPixelRatio,
                position.z * World::meterToPixelRatio));

            if (m_rotation_enabled)
                kvp.second->setRotation(element->getRotation());
        }
    } else {
        auto first_element = m_elements.begin()->second;
        auto first_position = first_element->getPosition();

        for (auto& kvp : m_models) {
            if (m_elements.find(kvp.first) == m_elements.end()) {
                kvp.second->setPosition(Vec3(first_position.x * World::meterToPixelRatio,
                    first_position.y * World::meterToPixelRatio,
                    first_position.z * World::meterToPixelRatio));

                if (m_rotation_enabled)
                    kvp.second->setRotation(first_element->getRotation());
            } else {
                auto position = m_elements[kvp.first]->getPosition();

                kvp.second->setPosition(Vec3(position.x * World::meterToPixelRatio,
                    position.y * World::meterToPixelRatio,
                    position.z * World::meterToPixelRatio));

                kvp.second->setRotation(m_elements[kvp.first]->getRotation());
            }
        }
    }
}

std::vector<ModelPtr> GameObjectImpl::getAllModels()
{
    std::vector<ModelPtr> ret;

    for (auto& kvp : m_models)
        ret.push_back(kvp.second);

    return ret;
}

std::vector<PhysicsElementPtr> GameObjectImpl::getAllElements()
{
    std::vector<PhysicsElementPtr> ret;

    for (auto& kvp : m_elements)
        ret.push_back(kvp.second);

    return ret;
}

void GameObjectImpl::setPosition(const Vec3& position)
{
    m_position = position;

    for (auto& element : m_elements)
        element.second->setPosition(position);
}

Vec3 GameObjectImpl::getPosition() const
{
    if (m_elements.size() > 0)
        return m_elements.begin()->second->getPosition();

    return m_position;
}

void GameObjectImpl::setParentWorld(WorldPtr world)
{
    if (world != WorldPtr()) {
        for (auto& callback : m_enter_callbacks.getCallbacks())
            callback();
    } else {
        for (auto& callback : m_exit_callbacks.getCallbacks())
            callback();
    }

    m_parent_world = world;
}

void GameObjectImpl::addScript(const std::string& id, GameScriptPtr script)
{
    m_scripts[id] = script;
}

void GameObjectImpl::addSound(const std::string& id, SoundPtr sound)
{
    m_sounds[id] = sound;
}

WorldPtr GameObjectImpl::getParentWorld() const
{
    return m_parent_world;
}

void GameObjectImpl::enableRotation(bool enable)
{
    m_rotation_enabled = enable;
}

void GameObjectImpl::disablePhysicsCoupling(bool disable)
{
    m_disable_phys = disable;
}

std::string GameObject::getId() const
{
    return m_pImpl->getId();
}
