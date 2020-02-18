#include <mak3do/game/PhysicsElement2D.h>
#include <mak3do/game/box2d/PhysicsElement2DImpl.h>

namespace mak3do {

PhysicsElement2D::PhysicsElement2D()
{
    m_pimpl = std::make_shared<PhysicsElement2DImpl>(this);
}

}
