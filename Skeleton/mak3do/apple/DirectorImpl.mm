#include "DirectorImpl.h"
#include "../Director.h"
#import "SceneRenderer.h"

namespace mak3do {

DirectorImpl::DirectorImpl(mak3do::Director* parent)
: m_parent(parent)
{
}

void DirectorImpl::loop()
{
    [[SceneRenderer shared] render];
}
}
