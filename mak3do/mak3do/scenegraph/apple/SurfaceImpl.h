#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class SurfaceImpl {
public:
    SurfaceImpl(Surface* parent);
private:
    Surface* m_abstract;
    void* m_native_surface;
};
}

