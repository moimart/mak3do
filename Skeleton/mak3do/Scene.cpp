#include "Scene.h"
#ifdef M3D_PLAT_APPLE
#include "apple/SceneImpl.h"
#elif M3D_PLAT_ANDROID
#include "android/SceneImpl.h"
#endif

namespace mak3do {

Scene::Scene()
{
#ifdef M3D_PLAT_APPLE
    m_pimpl = std::make_shared<SceneImpl>(this);
#endif
}

}

