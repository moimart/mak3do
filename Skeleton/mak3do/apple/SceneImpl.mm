#include "SceneImpl.h"
#include "../Node.h"
#include "NodeImpl.h"
#import <SceneKit/SceneKit.h>
#import "../../apple/SceneRenderer.h"

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

SceneImpl::~SceneImpl()
{
    SCNScene* scene = (SCNScene*)CFBridgingRelease(m_native);
    
    scene = nil;
}

void SceneImpl::add_node(NodePtr node)
{
    SCNScene* scene = (__bridge SCNScene*)m_native;
    SCNNode* native_node = (__bridge SCNNode*)node->m_pimpl->m_native;
    
    [scene.rootNode addChildNode:native_node];
    
    m_nodes.push_back(node);
}

void SceneImpl::main_camera(const std::string& name)
{
    SceneRenderer* renderer = [SceneRenderer shared];
    
    [renderer setCameraName:[NSString stringWithUTF8String:name.c_str()]];
}

}
