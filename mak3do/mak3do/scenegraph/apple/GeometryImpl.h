#pragma once

#include <mak3do/scenegraph/types.h>
#include <mak3do/scenegraph/Color.h>

namespace mak3do {
class GeometryImpl : public Pimpl {
public:
    GeometryImpl(Geometry* parent);
    virtual ~GeometryImpl();
    
    void color(const color::RGBA& color);
    
    void material(MaterialPtr material);
    
    void modify_shader_geometry(const std::string& modifier_code);
    
    void replace_geometry(void* geometry);
    
    void value(const std::string& name, float value);
    void value(const std::string& name, const Vec3& value);
    void value(const std::string& name, const Vec4& value);
private:
    Geometry* m_abstract;
    void* m_native_geometry;
    
    friend class NodeImpl;
    friend class SceneImpl;
    friend class MaterialImpl;
};
}
