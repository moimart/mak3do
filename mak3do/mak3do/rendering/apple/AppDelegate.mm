//
//  AppDelegate.m
//  Skeleton
//
//  Created by Moises Martinez on 20.01.20.
//  Copyright © 2020 Moises Martinez. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"
#import "DirectorDisplayLink.h"
#import "SceneRenderer.h"
#import <MapKit/MapKit.h>

#include <mak3do/Game.hpp>

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    CGRect r = [[UIScreen mainScreen] bounds];//CGRectMake(0, 0, 640, 1136);
    UIWindow* w = [[UIWindow alloc] initWithFrame: r];
    self.window = w;

    SceneRenderer* renderer = [SceneRenderer renderer:r];

    self.viewController = [[ViewController alloc] initWithNibName:nil bundle:nil];

    self.viewController.view = renderer.view;//[[MKMapView alloc] initWithFrame:r];
    [self.window setRootViewController:self.viewController];
    [self.window makeKeyAndVisible];
    
    auto game = std::make_shared<mak3do::Game>();
    
    game->start();

    [[DirectorDisplayLink shared] startMainLoop];
    
    return YES;
}


@end
