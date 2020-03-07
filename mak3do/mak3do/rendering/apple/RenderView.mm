#import "RenderView.h"
#include <mak3do/io/Touch.h>
#include <mak3do/io/apple/TouchManagerImpl.h>

@implementation RenderView {
    mak3do::io::TouchManagerImpl* _pimpl;
}

- (nonnull instancetype)initWithFrame:(CGRect)frameRect device:(nullable id<MTLDevice>)device {
    
    self = [super initWithFrame:frameRect device:device];
    
    self.multipleTouchEnabled = YES;
    
    _pimpl = mak3do::io::TouchManager::get()->pimpl().get();
    
    return self;
}
#if TARGET_OS_IOS
- (void) touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    std::vector<mak3do::io::TouchPtr> _touches;
    auto i = 0;
    for (UITouch* touch in touches) {
        auto __touch = std::make_shared<mak3do::io::Touch>();
        
        __touch->location.x = [touch locationInView:touch.view].x;
        __touch->location.y = [touch locationInView:touch.view].y;
        __touch->tid = i++;
        
        _touches.push_back(__touch);
    }
    
    _pimpl->inject_touches_begin(_touches);
}

- (void) touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    std::vector<mak3do::io::TouchPtr> _touches;
    auto i = 0;
    for (UITouch* touch in touches) {
        auto __touch = std::make_shared<mak3do::io::Touch>();
        
        __touch->location.x = [touch locationInView:touch.view].x;
        __touch->location.y = [touch locationInView:touch.view].y;
        __touch->tid = i++;
        
        _touches.push_back(__touch);
    }
    
    _pimpl->inject_touches_ended(_touches);
}

- (void) touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    std::vector<mak3do::io::TouchPtr> _touches;
    auto i = 0;
    for (UITouch* touch in touches) {
        auto __touch = std::make_shared<mak3do::io::Touch>();
        
        __touch->location.x = [touch locationInView:touch.view].x;
        __touch->location.y = [touch locationInView:touch.view].y;
        __touch->tid = i++;
        
        _touches.push_back(__touch);
    }
    
    _pimpl->inject_touches_moved(_touches);
}

- (void) touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    std::vector<mak3do::io::TouchPtr> _touches;
    auto i = 0;
    for (UITouch* touch in touches) {
        auto __touch = std::make_shared<mak3do::io::Touch>();
        
        __touch->location.x = [touch locationInView:touch.view].x;
        __touch->location.y = [touch locationInView:touch.view].y;
        __touch->tid = i++;
        
        _touches.push_back(__touch);
    }
    
    _pimpl->inject_touches_cancelled(_touches);
}
#endif

@end
