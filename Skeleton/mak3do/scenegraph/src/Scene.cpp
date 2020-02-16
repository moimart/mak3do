#include <mak3do/scenegraph/Scene.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/apple/SceneImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/android/SceneImpl.h>
#endif

namespace mak3do {

ScenePtr Scene::load(const std::string& filename)
{
    return SceneImpl::load(filename);
}

Scene::Scene()
{
    m_pimpl = std::make_shared<SceneImpl>(this);
}

void Scene::add_node(NodePtr node)
{
    m_pimpl->add_node(node);
}

std::vector<NodePtr> Scene::nodes() const
{
    return m_pimpl->nodes();
}

void Scene::camera(const std::string& name)
{
    m_pimpl->main_camera(name);
}

}

