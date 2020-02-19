#include "SceneImpl.h"
#include <mak3do/scenegraph/Node.h>
#include <mak3do/scenegraph/Camera.h>
#include <mak3do/scenegraph/Light.h>
#include <mak3do/scenegraph/Scene.h>
#include <mak3do/scenegraph/Geometry.h>
#include <mak3do/scenegraph/geometries/Box.h>
#include <mak3do/scenegraph/geometries/Sphere.h>
#include "NodeImpl.h"
#include "CameraImpl.h"
#include "LightImpl.h"
#include "GeometryImpl.h"
#include "BoxImpl.h"
#include "SphereImpl.h"
#import <SceneKit/SceneKit.h>
#import <mak3do/rendering/apple/SceneRenderer.h>

#import <GLTFSceneKit/GLTFSceneKit-Swift.h>

namespace mak3do {

void SceneImpl::create_geometry(void* node, NodePtr real_node)
{
    SCNNode* __node = (__bridge SCNNode*)node;
    
    if (__node.geometry != nil) {
        GeometryPtr geometry;
        if ([__node.geometry isKindOfClass:[SCNBox class]]) {
            geometry = std::make_shared<Box>();
        } else if ([__node.geometry isKindOfClass:[SCNSphere class]]) {
            geometry = std::make_shared<Sphere>();
        /*
        } else if ([__node.geometry isKindOfClass:[SCNPlane class]]) {
            geometry = std::make_shared<Plane>();
        } else if ([__node.geometry isKindOfClass:[SCNPyramid class]]) {
            geometry = std::make_shared<Pyramid>();
        } else if ([__node.geometry isKindOfClass:[SCNCone class]]) {
         geometry = std::make_shared<Cone>();
        } else if ([__node.geometry isKindOfClass:[SCNCylinder class]]) {
            geometry = std::make_shared<Cylinder>();
        } else if ([__node.geometry isKindOfClass:[SCNCapsule class]]) {
            geometry = std::make_shared<Capsule>();
        } else if ([__node.geometry isKindOfClass:[SCNTube class]]) {
            geometry = std::make_shared<Tibe>();
        } else if ([__node.geometry isKindOfClass:[SCNTorus class]]) {
            geometry = std::make_shared<Torus>();
        */
        } else {
            geometry = std::make_shared<Geometry>();
        }
        
        geometry->m_geometry_pimpl->m_native_geometry = (void*)CFBridgingRetain(__node.geometry);
        real_node->m_pimpl->m_geometry = geometry;
    }
}

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
            create_geometry((__bridge void*)__node, node);
            
            //recursion
            add_children((__bridge void*)__node.childNodes, node);
        }
    }
}

ScenePtr SceneImpl::load(const std::string& filename)
{
    NSString* __filename = [NSString stringWithUTF8String:filename.c_str()];
    SCNScene* __scene = [SCNScene sceneNamed:__filename];
    
    if (__scene == nil) {
        NSURL* url = [[NSBundle mainBundle] URLForResource:__filename withExtension:@""];
        GLTFSceneSource* source = [[GLTFSceneSource alloc] initWithURL:url options:nil];
        
        NSError* error;
        __scene = [source sceneWithOptions:nil error:&error];
        
        if (error != nil) {
          NSLog(@"%@", error);
          return nullptr;
        }
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
            create_geometry((__bridge void*)__node, node);
            
            //recursion
            add_children((__bridge void*)__node.childNodes, node);
        }
    }
    
    return scene;
}


SceneImpl::SceneImpl(Scene* parent)
{
    SCNScene* scene = [SCNScene scene];
    
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

CameraPtr SceneImpl::camera() const
{
    return CameraPtr();
}

std::vector<NodePtr> SceneImpl::nodes() const
{
    return m_nodes;
}

}
