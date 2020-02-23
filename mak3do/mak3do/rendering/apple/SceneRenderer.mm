#import "SceneRenderer.h"
#import <Metal/Metal.h>

static SceneRenderer* _renderer = nil;

@implementation SceneRenderer {
    id<MTLDevice> _device;
    id<MTLCommandQueue> _commandQueue;
    SCNRenderer* _scnRenderer;
    CAMetalLayer* _layer;
    double _at;
    
    id<MTLRenderPipelineState> _pipelineState;
    
    vector_uint2 _viewportSize;
    
    NSString* _cameraName;
    SCNNode* _mainCamera;
};

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
#if TARGET_OS_MAC
#else
    _view.contentScaleFactor = [UIScreen mainScreen].scale;
#endif
    _view.delegate = self;
    
    _viewportSize.x = _view.drawableSize.width;
    _viewportSize.y = _view.drawableSize.height;
    
    _mainCamera = nil;
    
    _at = 0;
    
    //_scene = [self setupScene];
    
    return self;
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    [self render:0];
}

- (void) update:(float)dt {
    _at += dt;
}

- (void) render: (float)dt {
    
    auto commandBuffer = [_commandQueue commandBuffer];

    MTLRenderPassDescriptor *renderPassDescriptor = _view.currentRenderPassDescriptor;
    
    if (renderPassDescriptor != nil) {
    
        if (_mainCamera == nil) {
            _mainCamera = _scene.rootNode.childNodes[0];
        }
        
        if (_scene != nil) {
            [_scnRenderer setScene: _scene];
            [_scnRenderer setPointOfView:_mainCamera];
            auto frame = CGRectMake(0, 0, _viewportSize.x, _viewportSize.y);
            [_scnRenderer renderAtTime:_at viewport:frame commandBuffer:commandBuffer passDescriptor:renderPassDescriptor];
        }
        
        [commandBuffer presentDrawable:_view.currentDrawable];
    }
    
    [commandBuffer commit];
    [_view releaseDrawables];
}

- (void) setCameraName:(NSString *)cameraName {
    
    if (_scene != nil) {
        for (SCNNode* node in _scene.rootNode.childNodes) {
            if (node.name != nil && [node.name isEqualToString:cameraName]) {
                _mainCamera = node;
                return;
            }
        }
        
        _mainCamera = _scene.rootNode.childNodes[0];
    }
}

@end
