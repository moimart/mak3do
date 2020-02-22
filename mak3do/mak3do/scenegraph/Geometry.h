#pragma once

#include "types.h"
#include "Color.h"
#include "Node.h"

namespace mak3do {

class Geometry;

class Geometry : public Base {
public:
    Geometry();
    ~Geometry();
    
    void color(const color::RGBA& color);
    
    void modify_shader_geometry(const std::string& modifier_code);
    
    void material(MaterialPtr material);
protected:
    GeometryImplPtr m_geometry_pimpl;
    
    friend class GeometryImpl;
    friend class NodeImpl;
    friend class SceneImpl;
};
}

