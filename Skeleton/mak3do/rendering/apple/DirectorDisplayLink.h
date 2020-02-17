
#import <Foundation/Foundation.h>

@interface DirectorDisplayLink : NSObject {
    id displayLink;
    int interval;
}
@property (readwrite) int interval;
- (void)startMainLoop;
- (void)doCaller:(id)sender;
- (void)setAnimationInterval:(double)interval;
+ (id)shared;
+ (void)destroy;
@end
