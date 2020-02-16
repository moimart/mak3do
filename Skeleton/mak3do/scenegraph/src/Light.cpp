#include <mak3do/scenegraph/Light.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/apple/LightImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/android/LightImpl.h>
#endif

namespace mak3do {

Light::Light()
: m_light_pimpl(std::make_shared<LightImpl>(this))
{
    
}

Light::Light(LightType type)
: m_light_pimpl(std::make_shared<LightImpl>(this))
{
    
}
void Light::type(const Light::LightType& type)
{
    m_light_pimpl->type(type);
}

Light::LightType Light::type() const
{
    return m_light_pimpl->type();
}

void Light::color(const color::RGB& color)
{
    m_light_pimpl->color(color);
}

color::RGB Light::color() const
{
    return m_light_pimpl->color();
}

void Light::shadows(bool onoff)
{
    m_light_pimpl->shadows(onoff);
}

bool Light::shadows() const
{
    return m_light_pimpl->shadows();
}

}


