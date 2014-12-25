//
//  AppDelegate.h
//  OpenglTest
//
//  Created by Steven Stewart on 12/24/14.
//  Copyright (c) 2014 Maaz Kamani. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "core.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate, GLKViewDelegate>
{
    float screenWidthInPixels;
    float screenHeightInPixels;
}

@property (strong, nonatomic) UIWindow *window;


@end

