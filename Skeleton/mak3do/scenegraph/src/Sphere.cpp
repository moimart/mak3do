#include <mak3do/scenegraph/geometries/Sphere.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/SphereImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/SphereImpl.h>
#endif

namespace mak3do {

Sphere::Sphere()
: m_sphere_pimpl(std::make_shared<SphereImpl>(this))
{
    
}

Sphere::~Sphere()
{
    std::string s = "";
}

}


