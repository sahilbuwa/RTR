//
//  main.m
//  Window
//
//  Created by user224299 on 12/25/22.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char* argv[])
{
	// Code
	// Create autorelease pool for counting
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
	
	NSString *delegateClassName = NSStringFromClass([AppDelegate class]);
	
	int result = UIApplicationMain(argc, argv, nil, delegateClassName);
	
	// Tell Autorelease pool to release all the objects created by this application
	[pool release];
	
	return result;
}
