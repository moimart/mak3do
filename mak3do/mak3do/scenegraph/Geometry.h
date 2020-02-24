#pragma once

#include "types.h"
#include "Color.h"
#include "Node.h"

namespace mak3do {

class Geometry;

class Geometry : public Base {
public:
    Geometry();
    virtual ~Geometry();
    
    void color(const color::RGBA& color);
    
    void modify_shader_geometry(const std::string& modifier_code);
    
    void material(MaterialPtr material);
    
    void value(const std::string& name, float value);
    void value(const std::string& name, const Vec3& value);
    void value(const std::string& name, const Vec4& value);
protected:
    GeometryImplPtr m_geometry_pimpl;
    
    friend class GeometryImpl;
    friend class NodeImpl;
    friend class SceneImpl;
};
}

