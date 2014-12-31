//
//  AppDelegate.m
//  OpenglTest
//
//  Created by Steven Stewart on 12/24/14.
//  Copyright (c) 2014 Maaz Kamani. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@end

@implementation AppDelegate




- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    lastMillies = CACurrentMediaTime() * 1000;
    
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    CGSize screenSize = CGSizeMake(screenBounds.size.width * screenScale, screenBounds.size.height * screenScale);
    screenWidthInPixels = (float)screenSize.width;
    screenHeightInPixels = (float)screenSize.height;
    
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:context];
    
    initView(screenWidthInPixels, screenHeightInPixels);
    
    GLKView *view = [[GLKView alloc] initWithFrame:[[UIScreen mainScreen] bounds] context:context];
    view.delegate = self;
    
    GLKViewController *controller = [[GLKViewController alloc] init];
    controller.view = view;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [controller setPreferredFramesPerSecond:60];
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.rootViewController = controller;
    [self.window makeKeyAndVisible];
    
    
    
    return YES;
}
- (void)applicationWillResignActive:(UIApplication *)application
{
    
}
- (void)applicationDidEnterBackground:(UIApplication *)application
{

}
- (void)applicationWillEnterForeground:(UIApplication *)application
{

}
- (void)applicationDidBecomeActive:(UIApplication *)application
{

}
- (void)applicationWillTerminate:(UIApplication *)application
{

}

# pragma mark - GlkView Delegate
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    long currentMillies = CACurrentMediaTime() * 1000;
    long diff = currentMillies - lastMillies;
    lastMillies = currentMillies;
    if(diff > 100) diff = 100;
    
    renderScene((int)diff);
}


@end
