#import <SceneKit/SceneKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

@interface SceneRenderer : NSObject<MTKViewDelegate>

@property (readonly) MTKView* _Nonnull view;
@property SCNScene* _Nullable scene;

+ (instancetype _Nonnull) shared;
+ (instancetype _Nonnull) renderer:(CGRect)frameRect;
- (id _Nonnull) initWithFrame:(CGRect)frameRect;
- (void) render;
- (void) setCameraName:(NSString*)cameraName;
@end

