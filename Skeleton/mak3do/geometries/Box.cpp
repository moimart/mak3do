#include "Box.h"
#ifdef M3D_PLAT_APPLE
#include "../apple/BoxImpl.h"
#elif M3D_PLAT_ANDROID
#include "../android/BoxImpl.h"
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



