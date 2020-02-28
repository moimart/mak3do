//
//  GameViewController.m
//  mak3do-sandbox-mac
//
//  Created by Moises Martinez on 23.02.20.
//  Copyright © 2020 Moises Martinez. All rights reserved.
//

#import "GameViewController.h"
#import "SceneRenderer.h"
#import "DirectorDisplayLink.h"
#import <WebKit/WebKit.h>

#include <mak3do/Game.h>

@implementation GameViewController
{
    MTKView *_view;
    WKWebView *_wv;
}

- (void)wk {
    
    WKWebViewConfiguration* conf = [[WKWebViewConfiguration alloc] init];
    WKWebView* wv = [[WKWebView alloc] initWithFrame:self.view.frame configuration:conf];
    
    NSURL* url = [NSURL URLWithString:@"https://lh3.googleusercontent.com/proxy/H2IMmpB-EokvDz-2RpCOLaiIxJBLnF73zISJDHz4VExU5h6HMtiiSpQiH5dM0TDFUueBujbeBUNlXh6DfULaeMjAUvTQyLZyFU0UsKEecSIKZbu2o28ceNz-1e5Qpw"];//@"https://upload.wikimedia.org/wikipedia/commons/6/67/Firefox_Logo%2C_2017.svg"];
    [wv setValue:[NSNumber numberWithBool:NO] forKey:@"drawsBackground"];

    [wv loadRequest:[NSURLRequest requestWithURL:url]];
    wv.frame = self.view.bounds;
    
    [self.view addSubview:wv];
    _wv = wv;
}

- (NSSize)window:(NSWindow *)window willUseFullScreenContentSize:(NSSize)proposedSize
{
    _view.frame = self.view.bounds;
    _wv.frame = self.view.bounds;
    
    return proposedSize;
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
    _view.frame = self.view.bounds;
    _wv.frame = self.view.bounds;
    
    return frameSize;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    SceneRenderer* renderer = [SceneRenderer renderer:[[NSScreen mainScreen] frame]];

    _view = renderer.view;
    //self.view = _view;
    
    [self.view addSubview:_view];
    auto window = [[[NSApplication sharedApplication] windows] firstObject];
    window.delegate = self;
    
    if(!_view.device)
    {
        NSLog(@"Metal is not supported on this device");
        self.view = [[NSView alloc] initWithFrame:self.view.frame];
        return;
    }
    
    [self wk];

    auto game = std::make_shared<mak3do::Game>();
    game->start();

    [[DirectorDisplayLink shared] startMainLoop];
    [renderer mtkView:_view drawableSizeWillChange:self.view.bounds.size];
    [self windowWillResize:window toSize:self.view.bounds.size];
}

@end
