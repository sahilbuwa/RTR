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
#import "Sphere.h" // Sphere sathi

using namespace vmath;

// Defines
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Function Declarations
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);

// Global variable Declarations
FILE *gpFile = NULL;

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
    
    // Programmable Pipeline Related Variables
    GLuint shaderProgramObject;
    GLuint vao_sphere;
    GLuint vbo_sphere_position;
    GLuint vbo_sphere_normal;
    GLuint vbo_sphere_element;
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    // Sphere arrays
    float sphere_vertices[1146];
    float sphere_normals[1146];
    float sphere_textures[764];
    short sphere_elements[2280];
    GLuint numElements;
    GLuint numVertices;
    mat4 perspectiveProjectionMatrix;
    
    // Lights Variables
    GLuint ldUniform;
    GLuint kdUniform;
    GLuint lightPositionUniform;

    GLuint lightingEnableUniform;
    BOOL bLight;

    GLfloat lightDiffuse[4];
    GLfloat materialDiffuse[4];
    GLfloat lightPosition[4];

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
    "in vec4 a_position;" \
    "in vec3 a_normal;" \
    "uniform mat4 u_modelMatrix;" \
    "uniform mat4 u_viewMatrix;" \
    "uniform mat4 u_projectionMatrix;" \
    "uniform vec3 u_ld;" \
    "uniform vec3 u_kd;" \
    "uniform vec4 u_lightPosition;" \
    "uniform int u_lightingEnabled;" \
    "out vec3 diffuse_light_color;" \
    "void main(void)" \
    "{" \
    "if(u_lightingEnabled == 1)" \
    "{" \
    "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" \
    "mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));" \
    "vec3 transformedNormals = normalize(normalMatrix * a_normal);" \
    "vec3 lightDirection = normalize(vec3(u_lightPosition - eyeCoordinates));" \
    "diffuse_light_color = u_ld * u_kd * max(dot(lightDirection, transformedNormals), 0.0);" \
    "}" \
    "gl_Position =  u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
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

    // Fragment Shader
    const GLchar *fragmentShaderSourceCode =
    "#version 410 core" \
    "\n" \
    "in vec3 diffuse_light_color;" \
    "uniform int u_lightingEnabled;" \
    "out vec4 FragColor;" \
    "void main(void)" \
    "{" \
    "if(u_lightingEnabled == 1)" \
    "{" \
    "FragColor = vec4(diffuse_light_color, 1.0);" \
    "}" \
    "else" \
    "{" \
    "FragColor = vec4(1.0, 1.0, 1.0, 1.0);" \
    "}" \
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
    glAttachShader(shaderProgramObject, fragmentShaderObject);
    glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_POSITION, "a_position"); // Andhaar
    glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_NORMAL, "a_normal");
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
    modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
    viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
    projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
    
    ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
    kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
    lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");
    lightingEnableUniform = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");
    
    // Declaration of vertex data arrays
    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    numVertices = getNumberOfSphereVertices();
    numElements = getNumberOfSphereElements();

    // vao
    glGenVertexArrays(1, &vao_sphere);
    glBindVertexArray(vao_sphere);

    // position vbo
    glGenBuffers(1, &vbo_sphere_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // normal vbo
    glGenBuffers(1, &vbo_sphere_normal);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

    glVertexAttribPointer(SAB_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(SAB_ATTRIBUTE_NORMAL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // element vbo
    glGenBuffers(1, &vbo_sphere_element);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // unbind vao
    glBindVertexArray(0);

    // Clear the screen using black color
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    
    // Arrays Initialization
    lightDiffuse[0] = 1.0f;
    lightDiffuse[1] = 1.0f;
    lightDiffuse[2] = 1.0f;
    lightDiffuse[3] = 1.0f;
    
    materialDiffuse[0] = 0.5f;
    materialDiffuse[1] = 0.5f;
    materialDiffuse[2] = 0.5f;
    materialDiffuse[3] = 1.0f;
    
    lightPosition[0] = 0.0f;
    lightPosition[1] = 0.0f;
    lightPosition[2] = 2.0f;
    lightPosition[3] = 1.0f;
    
    // bLight Initialize
    bLight = NO;

    // Depth Related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

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
    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();
    mat4 translationMatrix = translate(0.0f, 0.0f, -2.0f);
    modelMatrix = translationMatrix;

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
    
    if(bLight == YES)
    {
        glUniform1i(lightingEnableUniform, 1);
        glUniform3fv(ldUniform, 1, lightDiffuse);
        glUniform3fv(kdUniform, 1, materialDiffuse);
        glUniform4fv(lightPositionUniform, 1, lightPosition);
    }
    else
    {
         glUniform1i(lightingEnableUniform, 0);
    }
     // *** bind vao ***
    glBindVertexArray(vao_sphere);

    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

    // *** unbind vao ***
    glBindVertexArray(0);
    // Unuse the shader program object
    glUseProgram(0);
}

- (void)myupdate
{
    // Code
}

- (void)uninitialize
{
    // Code
    // Deletion and uninitialization of vbo
    if(vbo_sphere_element)
    {
        glDeleteBuffers(1, &vbo_sphere_element);
        vbo_sphere_element = 0;
    }
    if(vbo_sphere_normal)
    {
        glDeleteBuffers(1, &vbo_sphere_normal);
        vbo_sphere_normal = 0;
    }
    if(vbo_sphere_position)
    {
        glDeleteBuffers(1, &vbo_sphere_position);
        vbo_sphere_position = 0;
    }

    // Deletion and uninitialization of vao
    if(vao_sphere)
    {
        glDeleteVertexArrays(1, &vao_sphere);
        vao_sphere = 0;
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
        case 'L':
         case 'l':
             if (bLight == NO)
             {
                 
                 bLight = YES;
             }
             else
             {
                 bLight = NO;
             }
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
