#pragma once

#include "types.h"
#include "../Geometry.h"

namespace mak3do {

class Surface : public Geometry {
public:
    Surface();
    ~Surface();
private:
    SurfaceImplPtr m_surface_pimpl;
};
}



