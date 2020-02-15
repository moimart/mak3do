#pragma once

#include "../types.h"
#include "../Color.h"

namespace mak3do {
class GeometryImpl {
public:
    GeometryImpl(Geometry* parent);
    ~GeometryImpl();
    
    void color(const color::RGBA& color);
    
    void replace_geometry(void* geometry);
private:
    Geometry* m_abstract;
    void* m_native_geometry;
    
    friend class NodeImpl;
};
}
