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

#include <mak3do/Game.h>

@implementation GameViewController
{
    MTKView *_view;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    SceneRenderer* renderer = [SceneRenderer renderer:[[NSScreen mainScreen] frame]];

    _view = renderer.view;
    self.view = _view;

    if(!_view.device)
    {
        NSLog(@"Metal is not supported on this device");
        self.view = [[NSView alloc] initWithFrame:self.view.frame];
        return;
    }

    auto game = std::make_shared<mak3do::Game>();
    game->start();

    [[DirectorDisplayLink shared] startMainLoop];
    [renderer mtkView:_view drawableSizeWillChange:_view.bounds.size];
}

@end
