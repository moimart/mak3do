#include <mak3do/scenegraph/geometries/Box.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/apple/BoxImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/android/BoxImpl.h>
#endif

namespace mak3do {

Box::Box()
: m_box_pimpl(std::make_shared<BoxImpl>(this))
{
    
}

Box::~Box()
{
    std::string s = "";
}

}



