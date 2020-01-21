#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>

@class SceneRenderer;

@interface MetalView : UIView

- (nonnull instancetype)initWithFrame:(CGRect)frameRect;

@property (readonly) SceneRenderer* _Nonnull renderer;

@end


