//
//  OGL.mm
//  Created by user224299 on 12/12/22.
// Header files
#import <foundation/foundation.h> // Stdio.h ithli
#import <cocoa/cocoa.h> // Windows.h ithli

#import <quartzcore/CVDisplayLink.h>

#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>
#import "vmath.h"

using namespace vmath;

// Defines
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Function Declarations
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);

// Global variable Declarations
FILE *gpFile = NULL;
NSWindow *window;

enum
{
    SAB_ATTRIBUTE_POSITION = 0,
    SAB_ATTRIBUTE_COLOR,
    SAB_ATTRIBUTE_NORMAL,
    SAB_ATTRIBUTE_TEXTURE0
};

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
    
    // Programmable Pipeline Related Variables
    GLuint shaderProgramObject;
    GLuint vao;
    GLuint vbo;
    GLuint mvpMatrixUniform;
    mat4 perspectiveProjectionMatrix;
    
    // Tesselation related variables
    GLuint numberOfSegmentsUniform;
    GLuint numberOfStripsUniform;
    GLuint lineColorUniform;
    GLuint uiNumberOfSegments;
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
    // Vertex Shader
    // Shader Source Code
    const GLchar *vertexShaderSourceCode =
    "#version 410 core" \
    "\n" \
    "in vec2 a_position;" \
    "void main(void)" \
    "{" \
    "gl_Position = vec4(a_position, 0.0, 1.0);" \
    "}";
    // Vertex Shader cha Object tayar kela
    GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    // Object la Source code dila
    glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);
    // GPU chya inline compiler la code compile karaila dila
    glCompileShader(vertexShaderObject);
    // Error checking
    GLint status;
    GLint infoLogLength;
    char *log = NULL;

    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
                fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", log);
                free(log);
                [self uninitialize];
            }
        }
    }

    // Tessellation Control Shader
    // Shader Source Code
    const GLchar *tessellationControlShaderSourceCode =
    "#version 410 core" \
    "\n" \
    "layout(vertices = 4) out;" \
    "uniform int u_numberOfSegments;" \
    "uniform int u_numberOfStrips;" \
    "void main(void)" \
    "{" \
    "gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;" \
    "gl_TessLevelOuter[0] = float(u_numberOfStrips);" \
    "gl_TessLevelOuter[1] = float(u_numberOfSegments);" \
    "}";
    // Tessellation C Shader cha Object tayar kela
    GLuint tessellationControlShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);
    // Object la Source code dila
    glShaderSource(tessellationControlShaderObject, 1, (const GLchar**)&tessellationControlShaderSourceCode, NULL);
    // GPU chya inline compiler la code compile karaila dila
    glCompileShader(tessellationControlShaderObject);
    // Error checking
    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(tessellationControlShaderObject, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(tessellationControlShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(tessellationControlShaderObject, infoLogLength, &written, log);
                fprintf(gpFile, "Tessellation Control Shader Compilation Log : %s\n", log);
                free(log);
                [self uninitialize];
            }
        }
    }

    // Tessellation Evaluation Shader
    // Shader Source Code
    const GLchar *tessellationEvaluationShaderSourceCode =
    "#version 410 core" \
    "\n" \
    "layout(isolines) in;" \
    "uniform mat4 u_mvpMatrix;" \
    "void main(void)" \
    "{" \
    "vec3 p0 = gl_in[0].gl_Position.xyz;" \
    "vec3 p1 = gl_in[1].gl_Position.xyz;" \
    "vec3 p2 = gl_in[2].gl_Position.xyz;" \
    "vec3 p3 = gl_in[3].gl_Position.xyz;" \
    "float u = gl_TessCoord.x;" \
    "vec3 p = p0 * (1-u) * (1-u) * (1-u) +" \
             "p1 * 3 * u * (1-u) * (1-u) +" \
             "p2 * 3 * u * u * (1-u) +" \
             "p3 * u * u * u;" \
    "gl_Position = u_mvpMatrix * vec4(p, 1.0);" \
    "}";
    // Tessellation E Shader cha Object tayar kela
    GLuint tessellationEvaluationShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);
    // Object la Source code dila
    glShaderSource(tessellationEvaluationShaderObject, 1, (const GLchar**)&tessellationEvaluationShaderSourceCode, NULL);
    // GPU chya inline compiler la code compile karaila dila
    glCompileShader(tessellationEvaluationShaderObject);
    // Error checking
    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(tessellationEvaluationShaderObject, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(tessellationEvaluationShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(tessellationEvaluationShaderObject, infoLogLength, &written, log);
                fprintf(gpFile, "Tessellation Evaluation Shader Compilation Log : %s\n", log);
                free(log);
                [self uninitialize];
            }
        }
    }

    // Fragment Shader
    const GLchar *fragmentShaderSourceCode =
    "#version 410 core" \
    "\n" \
    "uniform vec4 u_lineColor;" \
    "out vec4 FragColor;" \
    "void main(void)" \
    "{" \
    "FragColor = u_lineColor;" \
    "}";

    GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);
    glCompileShader(fragmentShaderObject);
    // Error Checking
    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
                fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", log);
                free(log);
                [self uninitialize];
            }
        }
    }

    // Shader Program Object
    shaderProgramObject = glCreateProgram();
    glAttachShader(shaderProgramObject, vertexShaderObject);
    glAttachShader(shaderProgramObject, tessellationControlShaderObject);
    glAttachShader(shaderProgramObject, tessellationEvaluationShaderObject);
    glAttachShader(shaderProgramObject, fragmentShaderObject);
    glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_POSITION, "a_position"); // Andhaar
    glLinkProgram(shaderProgramObject);
    // Error Checking
    status = 0;
    infoLogLength = 0;
    log = NULL;
    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
                fprintf(gpFile, "Shader Program Link Log : %s\n", log);
                free(log);
                [self uninitialize];
            }
        }
    }
    mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
    numberOfSegmentsUniform = glGetUniformLocation(shaderProgramObject, "u_numberOfSegments");
    numberOfStripsUniform = glGetUniformLocation(shaderProgramObject, "u_numberOfStrips");
    lineColorUniform = glGetUniformLocation(shaderProgramObject, "u_lineColor");

    // Declaration of vertex data arrays
    const GLfloat vertices[] =
    {
        -1.0f, -1.0f,
        -0.5f, 1.0f,
        0.5f, -1.0f,
        1.0f, 1.0f
    };

    // Vao and Vbo related code
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Clear the screen using black color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Depth Related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Our line has at least 1 geometry
    uiNumberOfSegments = 1;

    perspectiveProjectionMatrix = mat4::identity();
    // Warmup Resize Call
    [self resize:WIN_WIDTH :WIN_HEIGHT];
    return 0;
}

- (void)resize:(int)width :(int)height
{
    // Code
    if (height < 0)
        height = 1;
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

- (void)display
{
    // Code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Use the Shader Program Object
    glUseProgram(shaderProgramObject);
    
    // Transformations
    mat4 translationMatrix = mat4::identity();
    mat4 modelViewMatrix = mat4::identity();
    mat4 modelViewProjectionMatrix = mat4::identity();
    translationMatrix = translate(0.0f, 0.0f, -4.0f);
    modelViewMatrix = translationMatrix;
    
    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    glUniform1i(numberOfSegmentsUniform, uiNumberOfSegments);
    // Caption change of window
//    char str[255];
//    sprintf(str, "MacOS Window:SAB || Number of segments = %d", uiNumberOfSegments);
//    NSString *nsString = [NSString stringWithCString:str encoding:NSASCIIStringEncoding];
//    [window setTitle:nsString];

    glUniform1i(numberOfStripsUniform, 1); // 1 strip is there.
    glUniform4fv(lineColorUniform, 1, vec4(1.0f, 1.0f, 0.0f, 1.0f));

    glBindVertexArray(vao);

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    // Here there should be draw code (12 lakh)
    glDrawArrays(GL_PATCHES, 0, 4);

    glBindVertexArray(0);
    // Unuse the shader program object
    glUseProgram(0);}

- (void)myupdate
{
    // Code
}

- (void)uninitialize
{
    // Code
    // Deletion and uninitialization of vbo
    if(vbo)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    // Deletion and uninitialization of vao
    if(vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
     // Shader Uninitialization
     if(shaderProgramObject)
     {
         glUseProgram(shaderProgramObject);
         GLsizei numAttachedShaders;
         glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShaders);
         GLuint *shaderObjects;
         shaderObjects = (GLuint *)malloc(numAttachedShaders*sizeof(GLuint));
         glGetAttachedShaders(shaderProgramObject, numAttachedShaders, &numAttachedShaders, shaderObjects);
         for(GLsizei i = 0; i < numAttachedShaders; i++)
         {
             glDetachShader(shaderProgramObject, shaderObjects[i]);
             glDeleteShader(shaderObjects[i]);
             shaderObjects[i] = 0;
         }
         free(shaderObjects);
         shaderObjects = NULL;
         glUseProgram(0);
         glDeleteProgram(shaderProgramObject);
         shaderProgramObject = 0;
     }
    if(gpFile)
    {
        fprintf(gpFile,"Log File Is Successfully Closed.\n");
        fclose(gpFile);
        gpFile=NULL;
    }
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
        case NSUpArrowFunctionKey:
            uiNumberOfSegments++;
            if(uiNumberOfSegments >= 30)
                uiNumberOfSegments = 30;
            break;
        case NSDownArrowFunctionKey:
            uiNumberOfSegments--;
            if(uiNumberOfSegments <= 1)
                uiNumberOfSegments = 1;
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
