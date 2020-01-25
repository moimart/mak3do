#import <SceneKit/SceneKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

@interface SceneRenderer : NSObject<SCNSceneRendererDelegate>

@property (readonly) MTKView* _Nonnull view;

+ (instancetype _Nonnull) shared;
+ (instancetype _Nonnull) renderer:(CGRect)frameRect;
- (id _Nonnull) initWithFrame:(CGRect)frameRect;
- (void) render;

@end

