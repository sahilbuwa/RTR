//
//  AppDelegate.m
//  Window
//
//  Created by user224299 on 12/25/22.
//
#import "AppDelegate.h"
#import "ViewController.h"
#import "MyView.h"

@implementation AppDelegate
{
	@private
	UIWindow* window;
	ViewController *viewController;
	MyView *view;
	
}
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	// Code
	window = [UIWindow new]; // CreateWindow
	[window setBackgroundColor:[UIColor blackColor]]; // SetBackGroudColor
	viewController = [[ViewController alloc]init]; // Window->ViewController->view
	[window setRootViewController:viewController];	// ViewController set kela.
	CGRect rect = window.screen.bounds;	// CGRect rect = [[window screen]bounds];	// View sathi bounds ghetle
	view = [[MyView alloc]initWithFrame:rect];
	[viewController setView:view];
	[view release];
	
	[window makeKeyAndVisible];
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	// Code
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
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	// Code
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
