#include "Camera.h"
#ifdef M3D_PLAT_APPLE
#include "apple/CameraImpl.h"
#elif M3D_PLAT_ANDROID
#include "android/CameraImpl.h"
#endif

namespace mak3do {

Camera::Camera()
: m_camera_pimpl(std::make_shared<CameraImpl>(this))
{
    
}

Camera::~Camera()
{
    std::string s = "";
}

}

