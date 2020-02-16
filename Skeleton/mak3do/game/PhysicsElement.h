#pragma once

#include "types.h"

namespace mak3do {
//subclass to PhysicsElement2D and PhysicsElement3D

class PhysicsElementImpl {
public:
    virtual void world(PhysicsWorldPtr world) = 0;
    virtual PhysicsWorldPtr world() const = 0;
    virtual void position(const Vec3& position) = 0;
    virtual Vec3 position() const = 0;

    virtual void rotation(const Quaternion& rotation) = 0;
    virtual Quaternion rotation() const = 0;

    virtual void velocity(const Vec3& velocity) = 0;

    virtual void impulse(const Vec3& impulse) = 0;
    virtual void impulse(const Vec3& impulse, const Vec3& point) = 0;

    virtual void force(const Vec3& force) = 0;
    virtual void force(const Vec3& force, const Vec3& point) = 0;

    virtual void collided(CollisionCallbackPtr callback) = 0;
    virtual void stop_collided(CollisionCallbackPtr callback) = 0;
    virtual void cleanup_collided() = 0;
};

class PhysicsElement : public std::enable_shared_from_this<PhysicsElement> {
public:
    virtual ~PhysicsElement() {}

    virtual void world(PhysicsWorldPtr world);
    virtual PhysicsWorldPtr world() const;
    virtual void position(const Vec3& position);
    virtual Vec3 position() const;

    virtual void rotation(const Quaternion& rotation);
    virtual Quaternion rotation() const;

    virtual void velocity(const Vec3& velocity);

    virtual void impulse(const Vec3& impulse);
    virtual void impulse(const Vec3& impulse, const Vec3& point);

    virtual void force(const Vec3& force);
    virtual void force(const Vec3& force, const Vec3& point);

    virtual void collided(CollisionCallbackPtr callback);
    virtual void stop_collided(CollisionCallbackPtr callback);
    virtual void cleanup_collided();

protected:
    std::shared_ptr<PhysicsElementImpl> m_pimpl;
};

namespace ElementFactory {
    struct ElementDef {
        float mass;

        unsigned int shape;

        float radius;

        enum class physics_type {
            UNKNOWN = 0,
            _2D = 0,
            _3D
        };

        physics_type type;

        ElementDef()
            : radius(0)
            , type(physics_type::UNKNOWN)
            , mass(0)
            , shape(0)
        {
        }
    };

    struct ElementDef2D : public ElementDef {
        enum shape_type {
            DEFAULT = 0,
            POLYGON,
            CHAIN,
            BOX
        };

        struct
        {
            Vec2 origin, end;
        } edge;

        ElementDef2D()
        {
            type = physics_type::_2D;
            radius = 1;
            mass = 1;
            shape = (unsigned int)ElementDef2D::shape_type::BOX;
        }
    };

    struct ElementDef3D : public ElementDef {
        enum shape_type {
            DEFAULT = 0,
            SPHERE,
            CAPSULE,
            BOX,
            CONE,
            CYLINDER,
            PLANE
        };

        float height;
        Vec3 half_extents;

        ElementDef3D()
            : height(0)
            , half_extents(Vec3::ZERO)
        {
            type = physics_type::_3D;
            radius = 4;
            mass = 1;
            shape = (unsigned int)ElementDef3D::shape_type::SPHERE;
        }

        ElementDef3D(float _radius)
            : height(0)
            , half_extents(Vec3::ZERO)
        {
            type = physics_type::_3D;
            mass = 1;
            shape = (unsigned int)ElementDef3D::shape_type::SPHERE;
            radius = _radius;
        }
    };

    PhysicsElementPtr make_basic_element(const ElementDef& element);
};
}
