#import "MetalView.h"

@implementation MetalView

- (nonnull instancetype)initWithFrame:(CGRect)frameRect device:(id<MTLDevice>)device {

    self = [super initWithFrame:frameRect];
    
    CAMetalLayer* layer = self.metalLayer;
    
    layer.opaque = YES;
    layer.framebufferOnly = YES;
    layer.device = device;
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;

    return self;
}

+ (Class) layerClass
{
    return NSClassFromString(@"CAMetalLayer");
}

-(CAMetalLayer *)metalLayer
{
    return (CAMetalLayer *)self.layer;
}

@end
