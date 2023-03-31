//
//  AppDelegate.m
//  Window
//
//  Created by user224299 on 12/25/22.
//
#import "AppDelegate.h"
#import "ViewController.h"
#import "GLESView.h"

@implementation AppDelegate
{
    @private
    UIWindow* window;
    ViewController *viewController;
    GLESView *view;
    
}
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Code
    window = [UIWindow new]; // CreateWindow
    [window setBackgroundColor:[UIColor blackColor]]; // SetBackGroudColor
    viewController = [[ViewController alloc]init]; // Window->ViewController->view
    [window setRootViewController:viewController];    // ViewController set kela.
    CGRect rect = window.screen.bounds;    // CGRect rect = [[window screen]bounds];    // View sathi bounds ghetle
    view = [[GLESView alloc]initWithFrame:rect];
    [viewController setView:view];
    [view release];
    
    [window makeKeyAndVisible];
	[view startAnimation];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Code
	[view stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Code
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Code
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Code
	[view startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Code
	[view stopAnimation];
}

- (void)dealloc
{
    // Code
    if(view)
    {
        [view release];
        view = nil;
    }
    if(viewController)
    {
        [viewController release];
        viewController = nil;
    }
    if(window)
    {
        [window release];
        window = nil;
    }
    [super dealloc];
}
@end
