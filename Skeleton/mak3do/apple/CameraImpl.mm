#include "CameraImpl.h"
#include "NodeImpl.h"
#include "../Camera.h"
#import <SceneKit/SceneKit.h>

namespace mak3do {

CameraImpl::CameraImpl(Camera* parent)
: m_abstract(parent)
{
    SCNNode* cameraNode = (__bridge SCNNode*)parent->m_pimpl->m_native;
    SCNCamera* camera = [[SCNCamera alloc] init];
    
    cameraNode.camera = camera;
    
    parent->position(Vec3::ZERO);
}

CameraImpl::~CameraImpl()
{
}

}

