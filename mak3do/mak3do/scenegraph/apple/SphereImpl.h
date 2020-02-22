#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class SphereImpl : public Pimpl {
public:
    SphereImpl(Sphere* parent);
private:
    Sphere* m_abstract;
    void* m_native_sphere;
};
}

