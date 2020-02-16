#pragma once

#include "PhysicsElement.h"

namespace mak3do {
class PhysicsElement2D : public PhysicsElement {
public:
    PhysicsElement2D();

private:
    friend class PhysicsWorld2DImpl;
};
}
