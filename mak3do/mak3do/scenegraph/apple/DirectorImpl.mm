#include "DirectorImpl.h"
#include "SceneImpl.h"
#include <mak3do/scenegraph/Director.h>
#include <mak3do/scenegraph/Scene.h>
#import "SceneRenderer.h"

namespace mak3do {

DirectorImpl::DirectorImpl(mak3do::Director* parent)
: m_parent(parent)
{
}

void DirectorImpl::loop(float dt)
{
    [[SceneRenderer shared] update: dt];
}

void DirectorImpl::setScene(ScenePtr scene)
{
    SCNScene* native_scene = (__bridge SCNScene*) scene->m_pimpl->m_native;
    [[SceneRenderer shared] setScene: native_scene];
    
    m_current_scene = scene;
}

}
