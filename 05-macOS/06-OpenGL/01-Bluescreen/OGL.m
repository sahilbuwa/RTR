//
//  Window.m
//  Created by user224299 on 12/12/22.
// Header files
#import <foundation/foundation.h> // Stdio.h ithli
#import <cocoa/cocoa.h> // Windows.h ithli

#import <quartzcore/CVDisplayLink.h>

#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>

// Global Function Declarations
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);

// Global variable Declarations
FILE *gpFile = NULL;


// Interface / Class Declarations (Should be in header file)
@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@interface GLView : NSOpenGLView
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
    GLView *view;
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
    view = [[GLView alloc]initWithFrame:rect];
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
@implementation GLView
{
    @private
    CVDisplayLinkRef displayLink;
}

- (id)initWithFrame:(NSRect)frame
{
    // Code
    self = [super initWithFrame:frame];
    if(self)
    {
        // Initialize array of opengl piexl format attributes
        NSOpenGLPixelFormatAttribute openGLPixelFormatAttributes[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
            NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFADoubleBuffer,
            0
        };
        
        // Create Opengl pixel format using above attribute
        NSOpenGLPixelFormat *glPixelFormat = [[[NSOpenGLPixelFormat alloc]initWithAttributes : openGLPixelFormatAttributes]autorelease];
        
        if (glPixelFormat == nil)
        {
            fprintf(gpFile, "Failed to create pfa.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }
        
        // Create opengl context using above pixel format
        NSOpenGLContext *glContext = [[[NSOpenGLContext alloc]initWithFormat : glPixelFormat shareContext: nil]autorelease];
        
        if (glContext == nil)
        {
            fprintf(gpFile, "Failed to create opengl context.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }
        
        // Set this view's pixel format by using above pixel format
        [self setPixelFormat: glPixelFormat];
        
        // set view's opengl context using above opengl context
        [self setOpenGLContext: glContext];
    }
    return self;
}

// Define get frame for time with custome method for predefined signature
- (CVReturn)getFrameForTime:(const CVTimeStamp *)outputTime
{
    // Code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
    
    [self drawView];
    
    [pool release];
    
    return kCVReturnSuccess;
}

- (void)prepareOpenGL   // Overidable
{
    // Code
    [super prepareOpenGL];
    
    // Make the OpenGL context the current
    [[self openGLContext]makeCurrentContext];
    
    // Set double buffer swapping interval to one
    GLint swapInterval = 1;
    
    [[self openGLContext]setValues: &swapInterval forParameter: NSOpenGLCPSwapInterval]; // CP = context parameter
    
    fprintf(gpFile, "OpenGL Renderer = %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL version = %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "GLSL version = %s\n ", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    // CALL our initialize
    [self Initialize];
    
    // create, configure, start display link
    // Create display link
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // set callback with display link
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
    
    // convert NSOpenGL format to CGLPixelFormat
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];
    
    // Convert NSOpenGLContext to CGLContext
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];
    
    // Using above info set current CG display to CGLPixelFormat and context
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    // Start the display link
    CVDisplayLinkStart(displayLink);
    
}

- (void)reshape
{
    // Code
    [super reshape];
    
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    NSRect rect = [self bounds];
    
    int width = rect.size.width;
    int height = rect.size.height;
    
    // call our resize
    [self resize:width :height];
    
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
}

// WM_PAINT / XPose
- (void)drawRect:(NSRect)dirtyRect
{
    // Code
    [self drawView];
}

- (void)drawView
{
    // Code
    [[self openGLContext]makeCurrentContext];
    
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    
    // Call our display here
    [self display];
    
    // DO DOUBLE BUFFERING
    CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
    
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
}

- (int)Initialize
{
    // Code
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    
    return 0;
}

- (void)resize:(int)width :(int)height
{
    // Code
    if (height < 0)
        height = 1;
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

- (void)display
{
    // Code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

- (void)myupdate
{
    // Code
}

- (void)uninitialize
{
    // Code
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
            [self uninitialize];
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
    if(displayLink)
        CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
    displayLink = nil;
}
@end

// Implement the display link callback function
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* currentTime, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* view)
{
    // Code
    CVReturn result = [(GLView*)view getFrameForTime: outputTime];
    
    return result;
}
