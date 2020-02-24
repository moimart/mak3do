#include <mak3do/scenegraph/Geometry.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/GeometryImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/GeometryImpl.h>
#endif

namespace mak3do {

Geometry::Geometry()
{
    m_geometry_pimpl = std::make_shared<GeometryImpl>(this);
    m_base_pimpl = std::dynamic_pointer_cast<Pimpl>(m_geometry_pimpl);
}

void Geometry::color(const color::RGBA& color)
{
    m_geometry_pimpl->color(color);
}

void Geometry::modify_shader_geometry(const std::string& modifier_code)
{
    m_geometry_pimpl->modify_shader_geometry(modifier_code);
}

void Geometry::material(MaterialPtr material)
{
    m_geometry_pimpl->material(material);
}

void Geometry::value(const std::string& name, float value)
{
    m_geometry_pimpl->value(name,value);
}

void Geometry::value(const std::string& name, const Vec3& value)
{
    m_geometry_pimpl->value(name,value);
}

void Geometry::value(const std::string& name, const Vec4& value)
{
    m_geometry_pimpl->value(name,value);
}

Geometry::~Geometry()
{
    std::string s = "";
}

}


