#import "RenderView.h"
#include <mak3do/io/Pointer.h>
#include <mak3do/io/apple/PointerManagerImpl.h>

@implementation RenderView {
    mak3do::io::PointerManagerImpl* _pimpl;
}

- (nonnull instancetype)initWithFrame:(CGRect)frameRect device:(nullable id<MTLDevice>)device {
    
    self = [super initWithFrame:frameRect device:device];
#if TARGET_OS_IOS
    self.multipleTouchEnabled = YES;
#endif
    _pimpl = mak3do::io::PointerManager::get()->pimpl().get();
    
    return self;
}

#if TARGET_OS_MAC
- (void) mouseDown:(NSEvent *)event {
    auto __touch = std::make_shared<mak3do::io::PointerEvent>();
    __touch->location.x = [event locationInWindow].x * [[NSScreen mainScreen] backingScaleFactor];
    __touch->location.y = [event locationInWindow].y * [[NSScreen mainScreen] backingScaleFactor];
    
    _pimpl->inject_pointers_begin({__touch});
}

- (void) mouseDragged:(NSEvent *)event {
    auto __touch = std::make_shared<mak3do::io::PointerEvent>();
    __touch->location.x = [event locationInWindow].x * [[NSScreen mainScreen] backingScaleFactor];
    __touch->location.y = [event locationInWindow].y * [[NSScreen mainScreen] backingScaleFactor];
    
    _pimpl->inject_pointers_moved({__touch});
}

- (void) mouseUp:(NSEvent *)event {
    auto __touch = std::make_shared<mak3do::io::PointerEvent>();
    __touch->location.x = [event locationInWindow].x * [[NSScreen mainScreen] backingScaleFactor];
    __touch->location.y = [event locationInWindow].y * [[NSScreen mainScreen] backingScaleFactor];
    
    _pimpl->inject_pointers_ended({__touch});
}

- (void) scrollWheel:(NSEvent *)event {
    auto dx = [event scrollingDeltaX];
    auto dy = [event scrollingDeltaY];
    
    _pimpl->inject_scrolled(dx,dy);
}

#endif

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
    
    _pimpl->inject_pointers_begin(_touches);
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
    
    _pimpl->inject_pointers_ended(_touches);
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
    
    _pimpl->inject_pointers_moved(_touches);
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
    
    _pimpl->inject_pointers_cancelled(_touches);
}
#endif

@end
