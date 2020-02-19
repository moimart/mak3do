#include <mak3do/scenegraph/geometries/Surface.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/SurfaceImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/SurfaceImpl.h>
#endif

namespace mak3do {

Surface::Surface()
: m_surface_pimpl(std::make_shared<SurfaceImpl>(this))
{
    
}

Surface::~Surface()
{
    std::string s = "";
}

}



