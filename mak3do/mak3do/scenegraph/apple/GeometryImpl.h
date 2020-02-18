#pragma once

#include <mak3do/scenegraph/types.h>
#include <mak3do/scenegraph/Color.h>

namespace mak3do {
class GeometryImpl {
public:
    GeometryImpl(Geometry* parent);
    ~GeometryImpl();
    
    void color(const color::RGBA& color);
    
    void modify_shader_geometry(const std::string& modifier_code);
    
    void replace_geometry(void* geometry);
private:
    Geometry* m_abstract;
    void* m_native_geometry;
    
    friend class NodeImpl;
    friend class SceneImpl;
};
}
