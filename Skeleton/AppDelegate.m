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

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    CGRect r = [[UIScreen mainScreen] bounds];//CGRectMake(0, 0, 640, 1136);
    UIWindow* w = [[UIWindow alloc] initWithFrame: r];
    self.window = w;

    //MetalView *metalView = [[MetalView alloc] initWithFrame:r];

    // Use RootViewController manage EAGLView
    self.viewController = [[ViewController alloc] initWithNibName:nil bundle:nil];

    //self.viewController.view = metalView;
    
    [self.window addSubview: self.viewController.view];
    
    [[DirectorDisplayLink shared] startMainLoop];
    
    return YES;
}


@end
