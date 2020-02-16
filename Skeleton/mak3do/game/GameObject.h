#pragma once

#include "types.h"

namespace mak3do {

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
    GameObject(const std::string& name);
    virtual ~GameObject() = default;

    std::string name() const;

    void add_node(const std::string& nid, NodePtr node);
    NodePtr node(const std::string& nid) const;
    
    void add_physics_element(const std::string& eid, PhysicsElementPtr element);
    PhysicsElementPtr physics_element(const std::string& id) const;
    
    void remove_from_world();

    void position(const Vec3& position);
    Vec3 position() const;

    void rotation(bool enabled = true);
    void disable_physics_coupling(bool disable = true);

    WorldPtr world() const;

    std::vector<NodePtr> nodes();
protected:
    void update(float dt);
    
    friend class World;
private:
    std::shared_ptr<GameObjectImpl> m_pimpl;

    std::vector<PhysicsElementPtr> elements();
    void parent_world(WorldPtr world);

    friend class WorldImpl;
};
}
