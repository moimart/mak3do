#include "Geometry.h"
#ifdef M3D_PLAT_APPLE
#include "apple/GeometryImpl.h"
#elif M3D_PLAT_ANDROID
#include "android/GeometryImpl.h"
#endif

namespace mak3do {

Geometry::Geometry()
: m_geometry_pimpl(std::make_shared<GeometryImpl>(this))
{
    
}

void Geometry::color(const color::RGBA& color)
{
    m_geometry_pimpl->color(color);
}

void Geometry::modify_shader_geometry(const std::string& modifier_code)
{
    m_geometry_pimpl->modify_shader_geometry(modifier_code);
}

Geometry::~Geometry()
{
    std::string s = "";
}

}


