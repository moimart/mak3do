#pragma once
#import <MetalKit/MetalKit.h>

@interface RenderView : MTKView
- (nonnull instancetype)initWithFrame:(CGRect)frameRect device:(nullable id<MTLDevice>)device;
@end
