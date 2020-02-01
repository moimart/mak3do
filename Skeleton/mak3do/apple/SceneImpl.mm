#include "SceneImpl.h"
#include "../Node.h"
#include "NodeImpl.h"
#import <SceneKit/SceneKit.h>

namespace mak3do {
SceneImpl::SceneImpl(Scene* parent)
{
    SCNScene* scene = [[SCNScene alloc] init];
    
    SCNNode* cameraNode = [[SCNNode alloc] init];
    SCNCamera* camera = [[SCNCamera alloc] init];
    
    cameraNode.camera = camera;
    
    [cameraNode setPosition:SCNVector3Make(0, 1.2, 4)];
    [scene.rootNode addChildNode:cameraNode];
    
    m_native = (void*)CFBridgingRetain(scene);
}

void SceneImpl::addNode(NodePtr node)
{
    SCNScene* scene = (__bridge SCNScene*)m_native;
    SCNNode* native_node = (__bridge SCNNode*)node->m_pimpl->m_native;
    
    [scene.rootNode addChildNode:native_node];
}

}
