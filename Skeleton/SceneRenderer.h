#import <SceneKit/SceneKit.h>
#import "MetalView.h"

@interface SceneRenderer : NSObject<SCNSceneRendererDelegate>

@property (readonly) MetalView* _Nonnull view;

+ (instancetype _Nonnull) shared;
+ (instancetype _Nonnull) renderer:(CGRect)frameRect;
- (id _Nonnull) initWithFrame:(CGRect)frameRect;
- (void) render;

@end

