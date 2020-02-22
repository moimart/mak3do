#include <mak3do/scenegraph/Material.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/MaterialImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/MaterialImpl.h>
#endif

namespace mak3do {

Material::Material()
: m_pimpl(std::make_shared<MaterialImpl>(this))
{
    
}
}



