#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>

@interface MetalView : UIView

- (nonnull instancetype)initWithFrame:(CGRect)frameRect device:(id<MTLDevice>)device;
- (CAMetalLayer *)metalLayer;

@end


