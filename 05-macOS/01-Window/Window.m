// Header files
#import <foundation/foundation.h> // Stdio.h ithli
#import <cocoa/cocoa.h> // Windows.h ithli

// Global variable Declarations

// Interface / Class Declarations (Should be in header file)
@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

// Entry Point Function
int main(int argc, char* argv[])
{
	// Code
	// Create autorelease pool for counting
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

	NSApp = [NSApplication sharedApplication];

	// Create AppDelegate object
	AppDelegate *appDelegate = [[AppDelegate alloc]init];

	// Give our AppDelegate(Son) to NSApp(father)
	[NSApp setDelegate:appDelegate];

	// Start Game Loop / Run loop
	[NSApp run];

	// Tell Autorelease pool to release all the objects created by this application
	[pool release];

	return 0; 
}

// Implementation of AppDelegate
@implementation AppDelegate
{
	@private
	// Class Variables (Initialization not allowed)
	NSWindow *window;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification // WM_CREATE
{
	// Code
	NSRect rect = NSMakeRect(0.0, 0.0, 800.0, 600.0); // Carbon API CoreGraphics which are C-Style APIs.
	window = [[NSWindow alloc]initWithContentRect:rect
										styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskClosable|NSWindowStyleMaskMiniaturizable|NSWindowStyleMaskResizable 
										  backing:NSBackingStoreBuffered
											defer:NO];
	[window setTitle:@"MacOS Window:SAB"];
	NSColor *backgroundColor = [NSColor blackColor];
	[window setBackgroundColor:backgroundColor];
	[window center];
	// Set windows delegate to this object
	[window setDelegate:self];
	[window makeKeyAndOrderFront:self];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
	// Code
}

- (void)windowWillClose:(NSNotification *)notification
{
	// Code
	[NSApp terminate:self];
}

- (void)dealloc
{
	// Code
	if(window)
	{
		[window release];
		window = nil;
	}
	[super dealloc];
}
@end
