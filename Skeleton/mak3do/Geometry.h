#pragma once

#include "types.h"
#include "Color.h"
#include "Node.h"

namespace mak3do {

class Geometry;

class Geometry : std::enable_shared_from_this<Geometry> {
public:
    Geometry();
    ~Geometry();
    
    GeometryImplPtr get_pimpl() const { return m_geometry_pimpl; };
    
    void color(const color::RGBA& color);
protected:
    GeometryImplPtr m_geometry_pimpl;
    
    friend class GeometryImpl;
    friend class NodeImpl;
};
}
