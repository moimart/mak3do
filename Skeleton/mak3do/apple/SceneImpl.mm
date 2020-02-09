#include "SceneImpl.h"
#include "../Node.h"
#include "../Camera.h"
#include "../Light.h"
#include "../Scene.h"
#include "NodeImpl.h"
#include "CameraImpl.h"
#include "LightImpl.h"
#import <SceneKit/SceneKit.h>
#import "../../apple/SceneRenderer.h"

namespace mak3do {

void SceneImpl::add_children(void* children, NodePtr parent)
{
    NSArray<SCNNode*>* __children = (__bridge NSArray<SCNNode*>*)children;
    
    for (SCNNode* __node in __children) {
        if (__node.camera != nil) {
            //create a camera
            auto camera = std::make_shared<Camera>();
            camera->m_pimpl->m_native = (void*)CFBridgingRetain(__node);
            camera->m_camera_pimpl->m_native_camera = (void*)CFBridgingRetain(__node.camera);
            
            parent->add_node(camera);
            
            //child nodes?
        } else if (__node.light != nil) {
            //create a light
            auto light = std::make_shared<Light>();
            light->m_pimpl->m_native = (void*)CFBridgingRetain(__node);
            light->m_light_pimpl->m_native = (void*)CFBridgingRetain(__node.light);
            
            parent->add_node(light);
        } else {
            //create a normal node
            auto node = std::make_shared<Node>();
            node->m_pimpl->m_native = (void*)CFBridgingRetain(__node);
            
            parent->add_node(node);
            
            //recursion
            add_children((__bridge void*)__node.childNodes, node);
        }
    }
}

ScenePtr SceneImpl::load(const std::string& filename)
{
    SCNScene* __scene = [SCNScene sceneNamed:@"test_scene.scn"];
    
    if (__scene == nil) {
        return nullptr;
    }
    
    auto scene = std::make_shared<Scene>();
    
    scene->m_pimpl->m_native = (void*)CFBridgingRetain(__scene);
    
    //add default camera for consistency
    if (__scene.rootNode.camera == nil) {
        SCNNode* cameraNode = [SCNNode node];
        SCNCamera* camera = [SCNCamera camera];
        
        cameraNode.camera = camera;
        
        [cameraNode setPosition:SCNVector3Make(0, 1.2, 4)];
        [__scene.rootNode addChildNode:cameraNode];
    }
    
    //create nodes
    for (SCNNode* __node in __scene.rootNode.childNodes) {
        if (__node.camera != nil) {
            //create a camera
            auto camera = std::make_shared<Camera>();
            camera->m_pimpl->m_native = (void*)CFBridgingRetain(__node);
            camera->m_camera_pimpl->m_native_camera = (void*)CFBridgingRetain(__node.camera);
            
            scene->add_node(camera);
            
            //child nodes?
        } else if (__node.light != nil) {
            //create a light
            auto light = std::make_shared<Light>();
            light->m_pimpl->m_native = (void*)CFBridgingRetain(__node);
            light->m_light_pimpl->m_native = (void*)CFBridgingRetain(__node.light);
            
            scene->add_node(light);
        } else {
            //create a normal node
            auto node = std::make_shared<Node>();
            node->m_pimpl->m_native = (void*)CFBridgingRetain(__node);
            
            scene->add_node(node);
            
            //recursion
            add_children((__bridge void*)__node.childNodes, node);
        }
    }
    
    return scene;
}


SceneImpl::SceneImpl(Scene* parent)
{
    SCNScene* scene = [[SCNScene alloc] init];
    
    SCNNode* cameraNode = [SCNNode node];
    SCNCamera* camera = [SCNCamera camera];
    
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
