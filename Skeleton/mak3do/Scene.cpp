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

void Scene::add_node(NodePtr node)
{
    m_pimpl->addNode(node);
}

void Scene::camera(const std::string& name)
{
    m_pimpl->mainCamera(name);
}

}

