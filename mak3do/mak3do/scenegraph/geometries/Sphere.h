#pragma once

#include "types.h"
#include "../Geometry.h"

namespace mak3do {

class Sphere : public Geometry {
public:
    Sphere();
    ~Sphere();
private:
    SphereImplPtr m_sphere_pimpl;
};
}


