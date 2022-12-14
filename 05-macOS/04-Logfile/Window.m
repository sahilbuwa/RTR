//
//  Window.m
//  Created by user224299 on 12/12/22.
// Header files
#import <foundation/foundation.h> // Stdio.h ithli
#import <cocoa/cocoa.h> // Windows.h ithli

// Global variable Declarations
FILE *gpFile = NULL;
// Interface / Class Declarations (Should be in header file)
@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@interface MyView : NSView
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
    MyView *view;
}
// Starting with - is instance method, and with + is static method.
- (void)applicationDidFinishLaunching:(NSNotification *)notification // WM_CREATE
{
    // Code
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appDirPath = [appBundle bundlePath];
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/log.txt",parentDirPath];
    const char* pszLogFileNameWithPath = [logFileNameWithPath UTF8String]; //cStringUsingEncoding:NSASCIIStringEncoding pn chalel UTF8String chya jaagi.
    gpFile = fopen(pszLogFileNameWithPath, "w");
    if(gpFile == NULL)
    {
        [NSApp terminate:self];
    }
    fprintf(gpFile, "Program is started successfully.\n");
    NSRect rect = NSMakeRect(0.0, 0.0, 800.0, 600.0); // (Carbon API CoreGraphics-NSMakeRect) which are C-Style APIs.
    window = [[NSWindow alloc]initWithContentRect:rect
                                        styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskClosable|NSWindowStyleMaskMiniaturizable|NSWindowStyleMaskResizable
                                          backing:NSBackingStoreBuffered
                                            defer:NO]; // True and false in ObjC are YES and NO.
    [window setTitle:@"MacOS Window:SAB"];
    NSColor *backgroundColor = [NSColor blackColor];
    [window setBackgroundColor:backgroundColor];
    [window center];
    view = [[MyView alloc]initWithFrame:rect];
    [window setContentView:view];
    // Set Window's delegate to this object
    [window setDelegate:self];
    [window makeKeyAndOrderFront:self];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
    // Code
    if(gpFile)
    {
        fprintf(gpFile, "Program is terminated successfully.\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}

- (void)windowWillClose:(NSNotification *)notification
{
    // Code
    [self release];
    [NSApp terminate:self];
}

- (void)dealloc
{
    // Code
    if(view)
    {
        [view release];
        view = nil;
    }
    if(window)
    {
        [window release];
        window = nil; // NULL is nil
    }
    [super dealloc];
}
@end

// Implement MyView
@implementation MyView
- (id)initWithFrame:(NSRect)frame
{
    // Code
    self = [super initWithFrame:frame];
    if(self)
    {
        
    }
    return self;
}

- (BOOL)acceptsFirstResponder
{
    // Code
    [[self window]makeFirstResponder:self];
    return YES;
}

- (void)keyDown:(NSEvent *)event
{
    // Code
    int key = (int)[[event characters]characterAtIndex:0];
    switch(key)
    {
        case 27:
            [self release];
            [NSApp terminate:self];
            break;
        case 'F':
        case 'f':
            [[self window]toggleFullScreen:self];
            break;
        default:
            break;
    }
}

- (void)mouseDown:(NSEvent *)event
{
    // Code
}

- (void)dealloc
{
    // Code
    [super dealloc];
}
@end
