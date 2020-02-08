#include "Light.h"
#ifdef M3D_PLAT_APPLE
#include "apple/LightImpl.h"
#elif M3D_PLAT_ANDROID
#include "android/LightImpl.h"
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

}


