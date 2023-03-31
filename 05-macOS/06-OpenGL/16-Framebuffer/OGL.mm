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
#define FBO_WIDTH 1024
#define FBO_HEIGHT 1024

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
    GLuint vao_cube;
    GLuint vbo_cube_position;
    GLuint vbo_cube_texcoord;
    GLuint mvpMatrixUniform;

    GLuint textureSamplerUniform;

    GLfloat angleCube;
    mat4 perspectiveProjectionMatrix;

    // FBO related global variables
    GLuint fbo;
    GLuint rbo;
    GLuint fbo_texture;
    BOOL bFBOResult;
    
    int gWidth;
    int gHeight;
    // Texture scene global variables
    GLuint shaderProgramObject_sphere;

    GLuint vao_sphere;
    GLuint vbo_sphere_position;
    GLuint vbo_sphere_normal;
    GLuint vbo_sphere_element;

    GLuint modelMatrixUniform_sphere;
    GLuint viewMatrixUniform_sphere;
    GLuint projectionMatrixUniform_sphere;

    // Sphere arrays
    float sphere_vertices[1146];
    float sphere_normals[1146];
    float sphere_textures[764];
    short sphere_elements[2280];
    GLuint numElements_sphere;
    GLuint numVertices_sphere;

    mat4 perspectiveProjectionMatrix_sphere;
    // Lights Variables
    BOOL bLight;

    GLuint ldUniform_sphere[3];
    GLuint laUniform_sphere[3];
    GLuint lsUniform_sphere[3];
    GLuint lightPositionUniform_sphere[3];

    GLuint kaUniform_sphere;
    GLuint kdUniform_sphere;
    GLuint ksUniform_sphere;
    GLuint materialShininessUniform_sphere;

    GLuint lightingEnabledUniform_sphere;

    struct Light
    {
        vec4 lightAmbient;
        vec4 lightDiffuse;
        vec4 lightSpecular;
        vec4 lightPosition;
    };
    Light lights[3];

    vec4 materialAmbient_sphere;
    vec4 materialDiffuse_sphere;
    vec4 materialSpecular_sphere;
    GLfloat materialShininess_sphere;

    GLfloat redLightAngle_sphere, greenLightAngle_sphere, blueLightAngle_sphere;
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
        
        [self  setWantsBestResolutionOpenGLSurface:YES];
        [self convertRectToBacking:[self bounds]];
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
    // Get view dimensions in pixels
    NSRect backingBounds = [self convertRectToBacking:[self bounds]];
 
    GLsizei backingPixelWidth  = (GLsizei)(backingBounds.size.width),
            backingPixelHeight = (GLsizei)(backingBounds.size.height);
 
    // Set viewport
    glViewport(0, 0, backingPixelWidth, backingPixelHeight);
    
    [self drawView];
}

- (void)drawView
{
    // Code
    [[self openGLContext]makeCurrentContext];
    
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    
    // Call our display here
    [self display];
    [self myupdate];
    
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
    "in vec2 a_texcoord;" \
    "uniform mat4 u_mvpMatrix;" \
    "out vec2 a_texcoord_out;" \
    "void main(void)" \
    "{" \
    "gl_Position = u_mvpMatrix * a_position;" \
    "a_texcoord_out = a_texcoord;" \
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
    "in vec2 a_texcoord_out;" \
    "uniform sampler2D u_textureSampler;" \
    "out vec4 FragColor;" \
    "void main(void)" \
    "{" \
    "FragColor = texture(u_textureSampler, a_texcoord_out);" \
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
    glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_TEXTURE0, "a_texcoord"); // Andhaar
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
    textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

    // Declaration of vertex data arrays
    const GLfloat cubePosition[] =
    {
        // top
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        // bottom
        1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,

        // front
        1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        // back
        1.0f, 1.0f, -1.0f,
       -1.0f, 1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        // right
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,

        // left
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,

    };

    const GLfloat cubeTexcoords[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Bottom
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    // Cube
    // Vao related code
    glGenVertexArrays(1, &vao_cube);
    glBindVertexArray(vao_cube);
    // Vbo for position
    glGenBuffers(1, &vbo_cube_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubePosition), cubePosition, GL_STATIC_DRAW);
    glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Vbo for texture
    glGenBuffers(1, &vbo_cube_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexcoords), cubeTexcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(SAB_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(SAB_ATTRIBUTE_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Vao unbind
    glBindVertexArray(0);

    // Clear the screen using white color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Depth Related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enabling the texture
    glEnable(GL_TEXTURE_2D);
    bFBOResult = NO;

    perspectiveProjectionMatrix = mat4::identity();
    
    // Warmup Resize Call
    [self resize:WIN_WIDTH :WIN_HEIGHT];
    // FBO initialize code
    bFBOResult = [self createFBO:FBO_WIDTH :FBO_HEIGHT];
    int iRetval;
    if(bFBOResult == YES)
    {
        iRetval = [self initialize_sphere:FBO_WIDTH :FBO_HEIGHT];
    }
    else
    {
        fprintf(gpFile, "Create FBO failed.\n");
        [self uninitialize];
        return 0;
    }
    return 0;
}

-(bool)createFBO:(GLint)textureWidth :(GLint) textureHeight
{
    // Local Variable Declarations
    int maxRenderbufferSize = 0;
    // Code
    // Step 1: Check available renderbuffer size.
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
    if(maxRenderbufferSize < textureWidth || maxRenderbufferSize < textureHeight)
    {
        fprintf(gpFile, "Insufficient renderbuffer size.\n");
        return NO;
    }
    // Step 2: Create framebuffer object.
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // Step 3: Create renderbuffer object.
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // Step 4: Storage and format of renderbuffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight);
    // Step 5: Create empty texture for upcoming scene
    glGenTextures(1, &fbo_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL); // 2nd last param matches with GL_DEPTH_COMPONENT16.
    // Step 6: Give above texture to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
    // Step 7 : Give renderbuffer to fbo.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    // Step 8: Check whether the framebuffer is create successfully or not.
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(result != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(gpFile, "Framebuffer is not complete.\n");
        return NO;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Texture and rbo will be detached automatically.
    return YES;
}

-(int) initialize_sphere:(int)width :(int)height
{
    // Per fragment
    // Vertex Shader
    // Shader Source Code
    const GLchar *vertexShaderSourceCodeF =
    "#version 410 core" \
    "\n" \
    "in vec4 a_position;\n" \
    "in vec3 a_normal;\n" \
    "uniform mat4 u_modelMatrix;\n" \
    "uniform mat4 u_viewMatrix;\n" \
    "uniform mat4 u_projectionMatrix;\n" \
    "uniform int u_lightingEnabled;\n" \
    "out vec3 transformedNormals;\n" \
    "out vec3 viewerVector;\n" \
    "void main(void)\n" \
    "{\n" \
    "if(u_lightingEnabled == 1)\n" \
    "{\n" \
    "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" \
    "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" \
    "transformedNormals = normalMatrix * a_normal;\n" \
    "viewerVector = -eyeCoordinates.xyz;\n" \
    "}\n" \
    "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" \
    "}\n";
    // Vertex Shader cha Object tayar kela
    GLuint vertexShaderObjectF = glCreateShader(GL_VERTEX_SHADER);
    // Object la Source code dila
    glShaderSource(vertexShaderObjectF, 1, (const GLchar**)&vertexShaderSourceCodeF, NULL);
    // GPU chya inline compiler la code compile karaila dila
    glCompileShader(vertexShaderObjectF);
    // Error checking
    GLint status = 0;
    GLint infoLogLength = 0;
    char* log = NULL;

    glGetShaderiv(vertexShaderObjectF, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObjectF, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObjectF, infoLogLength, &written, log);
                fprintf(gpFile, "Vertex Shader of sphere Compilation Log : %s\n", log);
                free(log);
                [self uninitialize_sphere];
            }
        }
    }

    // Fragment Shader
    const GLchar *fragmentShaderSourceCodeF =
    "#version 410 core" \
    "\n" \
    "in vec3 transformedNormals;\n" \
    "in vec3 viewerVector;\n" \
    "uniform vec3 u_la[3];\n" \
    "uniform vec3 u_ld[3];\n" \
    "uniform vec3 u_ls[3];\n" \
    "uniform vec4 u_lightPosition[3];\n" \
    "uniform vec3 u_ka;\n" \
    "uniform vec3 u_kd;\n" \
    "uniform vec3 u_ks;\n" \
    "uniform float u_materialShininess;\n" \
    "uniform int u_lightingEnabled;\n" \
    "out vec4 FragColor;\n" \
    "void main(void)\n" \
    "{\n" \
    "vec4 fong_ads_light = vec4(0.0, 0.0, 0.0, 0.0);" \
    "if(u_lightingEnabled == 1)\n" \
    "{\n" \
    "vec3 ambient[3];\n" \
    "vec3 diffuse[3];\n" \
    "vec3 specular[3];\n" \
    "vec3 normalized_lightDirection[3];\n" \
    "vec3 reflectionVector[3];\n" \
    "vec3 normalized_transformedNormals = normalize(transformedNormals);\n" \
    "vec3 normalized_viewerVector = normalize(viewerVector);\n" \
    "for(int i = 0; i<3; i++)\n" \
    "{\n" \
    "ambient[i] = u_la[i] * u_ka;\n" \
    "normalized_lightDirection[i] = normalize(vec3(u_lightPosition[i]));\n" \
    "diffuse[i] = u_ld[i] * u_kd * max(dot(normalized_lightDirection[i], normalized_transformedNormals), 0.0);\n" \
    "reflectionVector[i] = reflect(-normalized_lightDirection[i], normalized_transformedNormals);\n" \
    "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], normalized_viewerVector), 0.0), u_materialShininess);\n" \
    "fong_ads_light = fong_ads_light + vec4(ambient[i] + diffuse[i] + specular[i], 1.0);\n" \
    "}\n" \
    "}\n" \
    "else\n" \
    "{\n" \
    "fong_ads_light = vec4(1.0, 1.0, 1.0, 1.0);\n" \
    "}\n" \
    "FragColor = fong_ads_light;\n" \
    "}\n";

    GLuint fragmentShaderObjectF = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderObjectF, 1, (const GLchar **)&fragmentShaderSourceCodeF, NULL);
    glCompileShader(fragmentShaderObjectF);
    // Error Checking
    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(fragmentShaderObjectF, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObjectF, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(fragmentShaderObjectF, infoLogLength, &written, log);
                fprintf(gpFile, "Fragment Shader of sphere Compilation Log : %s\n", log);
                free(log);
                [self uninitialize_sphere];
            }
        }
    }

    // Shader Program Object
    shaderProgramObject_sphere = glCreateProgram();
    glAttachShader(shaderProgramObject_sphere, vertexShaderObjectF);
    glAttachShader(shaderProgramObject_sphere, fragmentShaderObjectF);
    glBindAttribLocation(shaderProgramObject_sphere, SAB_ATTRIBUTE_POSITION, "a_position"); // Andhaar
    glBindAttribLocation(shaderProgramObject_sphere, SAB_ATTRIBUTE_NORMAL, "a_normal");
    glLinkProgram(shaderProgramObject_sphere);
    // Error Checking
    status = 0;
    infoLogLength = 0;
    log = NULL;
    glGetProgramiv(shaderProgramObject_sphere, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject_sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject_sphere, infoLogLength, &written, log);
                fprintf(gpFile, "Shader Program Link Log : %s\n", log);
                free(log);
                [self uninitialize_sphere];
            }
        }
    }
    // Shader 2 uniforms
    modelMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_modelMatrix");
    viewMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_viewMatrix");
    projectionMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_projectionMatrix");

    laUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[0]");
    ldUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[0]");
    lsUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[0]");
    lightPositionUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[0]");

    laUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[1]");
    ldUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[1]");
    lsUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[1]");
    lightPositionUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[1]");

    laUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[2]");
    ldUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[2]");
    lsUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[2]");
    lightPositionUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[2]");

    kaUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_ka");
    kdUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_kd");
    ksUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_ks");
    materialShininessUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_materialShininess");

    lightingEnabledUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_lightingEnabled");

    // Declaration of vertex data arrays
    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    numVertices_sphere = getNumberOfSphereVertices();
    numElements_sphere = getNumberOfSphereElements();

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Depth Related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    lights[0].lightAmbient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    lights[0].lightDiffuse = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    lights[0].lightSpecular = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    lights[0].lightPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    lights[1].lightAmbient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    lights[1].lightDiffuse = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    lights[1].lightSpecular = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    lights[1].lightPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    lights[2].lightAmbient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    lights[2].lightDiffuse = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    lights[2].lightSpecular = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    lights[2].lightPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    materialAmbient_sphere = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    materialDiffuse_sphere = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialSpecular_sphere = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialShininess_sphere = 50.0f;

    perspectiveProjectionMatrix_sphere = mat4::identity();
    // Warmup Resize Call
    [self resize_sphere:FBO_WIDTH :FBO_HEIGHT];
    return 0;
}

- (void)resize:(int)width :(int)height
{
    // Code
    if (height < 0)
        height = 1;
    
    gWidth = width;
    gHeight = height;
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

- (void)resize_sphere:(int)width :(int)height
{
    // Code
    if (height < 0)
        height = 1;
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    perspectiveProjectionMatrix_sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

- (void)display
{
    // Code
    if(bFBOResult == YES)
    {
        [self display_sphere:FBO_WIDTH :FBO_HEIGHT];
        [self myupdate_sphere];
    }
    // Screen background white kar
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    [self resize:gWidth :gHeight];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Use the Shader Program Object
    glUseProgram(shaderProgramObject);
    // cube
    // Transformations
    mat4 translationMatrix = mat4::identity();
    mat4 rotationMatrix = mat4::identity();
    mat4 rotationMatrix_x = mat4::identity();
    mat4 rotationMatrix_y = mat4::identity();
    mat4 rotationMatrix_z = mat4::identity();
    mat4 scaleMatrix = mat4::identity();
    mat4 modelViewMatrix = mat4::identity();
    mat4 modelViewProjectionMatrix = mat4::identity();
    translationMatrix = translate(0.0f, 0.0f, -4.0f);
    rotationMatrix_x = rotate(angleCube, 1.0f, 0.0f, 0.0f);
    rotationMatrix_y = rotate(angleCube, 0.0f, 1.0f, 0.0f);
    rotationMatrix_z = rotate(angleCube, 0.0f, 0.0f, 1.0f);
    rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
    scaleMatrix = scale(0.75f, 0.75f, 0.75f);
    modelViewMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
    glUniform1i(textureSamplerUniform, 0);

    glBindVertexArray(vao_cube);

    // Here there should be draw code (12 lakh)
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    // Unuse the shader program object
    glUseProgram(0);
}

- (void)myupdate
{
    // Code
    angleCube = angleCube + 1.0f;
    if(angleCube >= 360.0f)
        angleCube -= 360.0f;
}

-(void) display_sphere:(GLint)textureWidth :(GLint)textureHeight
{
    // Code
    // Bind framebuffer object
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // FBO background black kar
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    [self resize_sphere:textureWidth :textureHeight];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Transformations
    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();
    mat4 translationMatrix = translate(0.0f, 0.0f, -1.5f);
    modelMatrix = translationMatrix;

    // Use the Shader Program Object
    glUseProgram(shaderProgramObject_sphere);

    glUniformMatrix4fv(modelMatrixUniform_sphere, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform_sphere, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projectionMatrixUniform_sphere, 1, GL_FALSE, perspectiveProjectionMatrix_sphere);
        
    if(bLight == YES)
    {
        glUniform1i(lightingEnabledUniform_sphere, 1);
        glUniform3fv(kaUniform_sphere, 1, materialAmbient_sphere);
        glUniform3fv(kdUniform_sphere, 1, materialDiffuse_sphere);
        glUniform3fv(ksUniform_sphere, 1, materialSpecular_sphere);
        glUniform1f(materialShininessUniform_sphere, materialShininess_sphere);

        for(int i = 0; i < 3; i++)
        {
            glUniform3fv(laUniform_sphere[i], 1, lights[i].lightAmbient);
            glUniform3fv(ldUniform_sphere[i], 1, lights[i].lightDiffuse);
            glUniform3fv(lsUniform_sphere[i], 1, lights[i].lightSpecular);
            glUniform4fv(lightPositionUniform_sphere[i], 1, lights[i].lightPosition);
        }
    }
    else
    {
        glUniform1i(lightingEnabledUniform_sphere, 0);
    }
     // *** bind vao ***
    glBindVertexArray(vao_sphere);

    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
    glDrawElements(GL_TRIANGLES, numElements_sphere, GL_UNSIGNED_SHORT, 0);

    // *** unbind vao ***
    glBindVertexArray(0);
    // Unuse the shader program object
    glUseProgram(0);
    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

-(void)myupdate_sphere
{
    // Code
    redLightAngle_sphere += 1.0f;
    if(redLightAngle_sphere >= 360.0f)
        redLightAngle_sphere -= 360.0f;
    lights[0].lightPosition = vec4(0.0f, 10.0f * cosf(redLightAngle_sphere * M_PI / 180.0f), 10.0f * sinf(redLightAngle_sphere * M_PI / 180.0f), 1.0f);

    greenLightAngle_sphere += 1.0f;
    if(greenLightAngle_sphere >= 360.0f)
        greenLightAngle_sphere -= 360.0f;
    lights[1].lightPosition = vec4(10.0f * cosf(greenLightAngle_sphere * M_PI / 180.0f), 0.0f, 10.0f * sinf(greenLightAngle_sphere * M_PI / 180.0f), 1.0f);

    blueLightAngle_sphere += 1.0f;
    if(blueLightAngle_sphere >= 360.0f)
        blueLightAngle_sphere -= 360.0f;
    lights[2].lightPosition = vec4(10.0f * cosf(blueLightAngle_sphere * M_PI / 180.0f), 10.0f * sinf(blueLightAngle_sphere * M_PI / 180.0f), 0.0f, 1.0f);
}

-(void)uninitialize
{
    // Code
    [self uninitialize_sphere];
    // Deletion and uninitialization of vbo_color
    if(vbo_cube_texcoord)
    {
        glDeleteBuffers(1, &vbo_cube_texcoord);
        vbo_cube_texcoord = 0;
    }
    // Deletion and uninitialization of vbo_cube_position
    if(vbo_cube_position)
    {
        glDeleteBuffers(1, &vbo_cube_position);
        vbo_cube_position = 0;
    }
    // Deletion and uninitialization of vao_cube
    if(vao_cube)
    {
        glDeleteVertexArrays(1, &vao_cube);
        vao_cube = 0;
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

- (void)uninitialize_sphere
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
