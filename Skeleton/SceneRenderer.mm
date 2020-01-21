#import "SceneRenderer.h"
#import <Metal/Metal.h>

static SceneRenderer* _renderer = nil;

@implementation SceneRenderer {
    id<MTLDevice> _device;
    id<MTLCommandQueue> _commandQueue;
    SCNRenderer* _scnRenderer;
    CAMetalLayer* _layer;
};

- (SCNScene*) setupScene {
    SCNScene* scene = [[SCNScene alloc] init];
    
    SCNBox* box = [[SCNBox alloc] init];
    [box setWidth:1];
    [box setHeight:1];
    [box setLength:1];
    [box setChamferRadius: 0];
    box.materials.firstObject.diffuse.contents = [UIColor blueColor];
    SCNNode* node = [[SCNNode alloc] init];
    [node setGeometry:box];
    [node setPosition:SCNVector3Make(0, 0, 300)];
    [scene.rootNode addChildNode:node];
    
    SCNSphere* sphere = [[SCNSphere alloc] init];
    [sphere setRadius:1];
    sphere.materials.firstObject.diffuse.contents = [UIColor yellowColor];
    SCNNode* sphereNode = [[SCNNode alloc] init];
    [sphereNode setPosition:SCNVector3Make(2, 0, 0)];
    [scene.rootNode addChildNode:sphereNode];
    
    return scene;
}

+ (instancetype _Nonnull) shared {
    return _renderer;
}

+ (instancetype _Nonnull) renderer:(CGRect)frameRect {
    
    if (_renderer == nil) {
        _renderer = [[SceneRenderer alloc] initWithFrame:frameRect];
    }
    
    return _renderer;
}

- (id  _Nonnull) initWithFrame:(CGRect)frameRect {
    _view = [[SCNView alloc] initWithFrame:frameRect];
    
    [_view setScene:[self setupScene]];
    
    _device = MTLCreateSystemDefaultDevice();
    _commandQueue = [_device newCommandQueue];
    _scnRenderer = [SCNRenderer rendererWithDevice:_device options:nil];
    
    //_view.delegate = self;
    
    return self;
}

- (void) renderer:(id<SCNSceneRenderer>)renderer willRenderScene:(SCNScene *)scene atTime:(NSTimeInterval)time {
    [self render];
}

- (void) render {
    MTLRenderPassColorAttachmentDescriptor *colorAttachment = [MTLRenderPassColorAttachmentDescriptor new];
    CAMetalLayer* metalLayer = (CAMetalLayer*)_view.layer;
    
    colorAttachment.texture = metalLayer.nextDrawable.texture;
    colorAttachment.loadAction = MTLLoadActionClear;
    colorAttachment.clearColor = MTLClearColorMake(0,0,0,1);
    colorAttachment.storeAction = MTLStoreActionStore;
    
    MTLRenderPassDescriptor* renderPassDescriptor = [MTLRenderPassDescriptor
                                                     renderPassDescriptor];
    
    renderPassDescriptor.colorAttachments[0] = colorAttachment;
    
    auto commandBuffer = [_commandQueue commandBuffer];
    
    auto scene = [self setupScene];
    [_scnRenderer setScene: scene];
    [_scnRenderer setPointOfView:scene.rootNode];
    [_scnRenderer renderWithViewport:_view.frame commandBuffer:commandBuffer passDescriptor:renderPassDescriptor];
    
    
    [commandBuffer commit];
}

@end
