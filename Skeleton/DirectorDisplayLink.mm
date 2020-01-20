
#import <Foundation/Foundation.h>
#import "DirectorDisplayLink.h"
#import "Director.h"

static id s_sharedDirectorDisplayLink;

@interface NSObject(CADisplayLink)
+(id) displayLinkWithTarget: (id)arg1 selector:(SEL)arg2;
-(void) addToRunLoop: (id)arg1 forMode: (id)arg2;
-(void) setFrameInterval: (int)interval;
-(void) invalidate;
@end

@implementation DirectorDisplayLink

@synthesize interval;

+(id) shared
{
    if (s_sharedDirectorDisplayLink == nil) {
        s_sharedDirectorDisplayLink = [DirectorDisplayLink new];
    }
    
    return s_sharedDirectorDisplayLink;
}

+(void) destroy
{
    //[s_sharedDirectorDisplayLink release];
    s_sharedDirectorDisplayLink = nil;
}

-(void) alloc
{
        interval = 1;
}

-(void) dealloc
{
    //[displayLink release];
    //[super dealloc];
}

-(void) startMainLoop
{
        [displayLink invalidate];
        displayLink = nil;
        
        displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doCaller:)];
        [displayLink setFrameInterval: self.interval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) setAnimationInterval:(double)intervalNew
{
        // Director::setAnimationInterval() is called, we should invalidate it first
    [displayLink invalidate];
    displayLink = nil;

    if (intervalNew <= 0)
        intervalNew = 60.0f;

    self.interval = (60.0f / intervalNew);

    if (self.interval <= 0)
        self.interval = 1;

    displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doCaller:)];

    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [displayLink setFrameInterval: self.interval];
}
                      
-(void) doCaller: (id) sender
{
    mak3do::Director::get()->loop();
}

@end
