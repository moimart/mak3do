#import "SceneRenderer.h"
#import <Metal/Metal.h>

static SceneRenderer* _renderer = nil;

@implementation SceneRenderer {
    id<MTLDevice> _device;
    id<MTLCommandQueue> _commandQueue;
    SCNRenderer* _scnRenderer;
    CAMetalLayer* _layer;
    
    id<MTLRenderPipelineState> _pipelineState;
    
    vector_uint2 _viewportSize;
};

- (SCNScene*) setupScene {
    SCNScene* scene = [[SCNScene alloc] init];
    
    SCNNode* cameraNode = [[SCNNode alloc] init];
    SCNCamera* camera = [[SCNCamera alloc] init];
    
    cameraNode.camera = camera;
    
    [cameraNode setPosition:SCNVector3Make(0, 1.2, 4)];
    [scene.rootNode addChildNode:cameraNode];
    
    SCNBox* box = [[SCNBox alloc] init];
    [box setWidth:1];
    [box setHeight:1];
    [box setLength:1];
    [box setChamferRadius: 0];
    box.materials.firstObject.diffuse.contents = [UIColor blueColor];
    SCNNode* node = [[SCNNode alloc] init];
    [node setGeometry:box];
    [node setPosition:SCNVector3Make(0, 0, 0)];
    [scene.rootNode addChildNode:node];
    
    SCNSphere* sphere = [[SCNSphere alloc] init];
    [sphere setRadius:2];
    sphere.materials.firstObject.diffuse.contents = [UIColor redColor];
    SCNNode* sphereNode = [[SCNNode alloc] init];
    [sphereNode setPosition:SCNVector3Make(0.0, 0.1, -10)];
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

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

- (id  _Nonnull) initWithFrame:(CGRect)frameRect {
    
    _device = MTLCreateSystemDefaultDevice();
    _commandQueue = [_device newCommandQueue];
    _scnRenderer = [SCNRenderer rendererWithDevice:_device options:nil];
    
    _view = [[MTKView alloc] initWithFrame:frameRect device:_device];
    _view.contentScaleFactor = [UIScreen mainScreen].scale;
    
    _viewportSize.x = _view.drawableSize.width;
    _viewportSize.y = _view.drawableSize.height;
    
    //_scene = [self setupScene];
    
    return self;
}

- (void) renderer:(id<SCNSceneRenderer>)renderer willRenderScene:(SCNScene *)scene atTime:(NSTimeInterval)time {
    [self render];
}

- (void) initRenderTest {
    
    NSError *error = NULL;
    // Load all the shader files with a .metal file extension in the project.
    id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];

    id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexTestShader"];
    id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentTestShader"];

    // Configure a pipeline descriptor that is used to create a pipeline state.
    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateDescriptor.label = @"Simple Pipeline";
    pipelineStateDescriptor.vertexFunction = vertexFunction;
    pipelineStateDescriptor.fragmentFunction = fragmentFunction;
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = _view.colorPixelFormat;

    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                             error:&error];
}

- (void) render {
    
    auto commandBuffer = [_commandQueue commandBuffer];

    MTLRenderPassDescriptor *renderPassDescriptor = _view.currentRenderPassDescriptor;
    
    if (_scene != nil) {
        [_scnRenderer setScene: _scene];
        [_scnRenderer setPointOfView:_scene.rootNode.childNodes[0]];
        auto frame = CGRectMake(0, 0, _viewportSize.x, _viewportSize.y);
        [_scnRenderer renderWithViewport:frame commandBuffer:commandBuffer passDescriptor:renderPassDescriptor];
    }
    
    [commandBuffer presentDrawable:_view.currentDrawable];
    
    [commandBuffer commit];
}

@end
