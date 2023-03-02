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
    GLuint vbo_cube;
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint textureSamplerUniform;
    GLuint texture_marble;

    GLuint ldUniform;
    GLuint laUniform;
    GLuint lsUniform;
    GLuint lightPositionUniform;

    GLuint kaUniform;
    GLuint kdUniform;
    GLuint ksUniform;
    GLuint materialShininessUniform;

    GLuint lightingEnabledUniform;
    BOOL bLight;
    
    vec4 lightAmbient;
    vec4 lightDiffuse;
    vec4 lightSpecular;
    vec4 lightPosition;

    vec4 materialAmbient;
    vec4 materialDiffuse;
    vec4 materialSpecular;
    GLfloat materialShininess;

    GLfloat anglecube;
    
    mat4 perspectiveProjectionMatrix;
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
    // Call our update here
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
    "in vec4 a_position;\n" \
    "in vec3 a_normal;\n" \
    "in vec2 a_texcoord;\n" \
    "in vec4 a_color;\n" \
    "uniform mat4 u_modelMatrix;\n" \
    "uniform mat4 u_viewMatrix;\n" \
    "uniform mat4 u_projectionMatrix;\n" \
    "uniform vec4 u_lightPosition;\n" \
    "uniform int u_lightingEnabled;\n" \
    "out vec3 transformedNormals;\n" \
    "out vec3 lightDirection;\n" \
    "out vec3 viewerVector;\n" \
    "out vec2 a_texcoord_out;\n" \
    "out vec4 a_color_out;\n" \
    "void main(void)\n" \
    "{\n" \
    "if(u_lightingEnabled == 1)\n" \
    "{\n" \
    "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" \
    "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" \
    "transformedNormals = normalMatrix * a_normal;\n" \
    "lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;\n" \
    "viewerVector = -eyeCoordinates.xyz;\n" \
    "}\n" \
    "a_texcoord_out = a_texcoord;\n" \
    "a_color_out = a_color;\n" \
    "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" \
    "}\n";
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
    "in vec4 a_color_out;" \
    "in vec2 a_texcoord_out;\n" \
    "in vec3 transformedNormals;\n" \
    "in vec3 lightDirection;\n" \
    "in vec3 viewerVector;\n" \
    "uniform sampler2D u_textureSampler;\n" \
    "uniform vec3 u_la;\n" \
    "uniform vec3 u_ld;\n" \
    "uniform vec3 u_ls;\n" \
    "uniform vec3 u_ka;\n" \
    "uniform vec3 u_kd;\n" \
    "uniform vec3 u_ks;\n" \
    "uniform float u_materialShininess;\n" \
    "uniform int u_lightingEnabled;\n" \
    "out vec4 FragColor;\n" \
    "void main(void)\n" \
    "{\n" \
    "vec4 fong_ads_light;" \
    "if(u_lightingEnabled == 1)\n" \
    "{\n" \
    "vec3 ambient = u_la * u_ka;\n" \
    "vec3 normalized_transformedNormals = normalize(transformedNormals);\n" \
    "vec3 normalized_lightDirection = normalize(lightDirection);\n" \
    "vec3 diffuse = u_ld * u_kd * max(dot(normalized_lightDirection, normalized_transformedNormals), 0.0);\n" \
    "vec3 reflectionVector = reflect(-normalized_lightDirection, normalized_transformedNormals);\n" \
    "vec3 normalized_viewerVector = normalize(viewerVector);\n" \
    "vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewerVector), 0.0), u_materialShininess);\n" \
    "fong_ads_light = vec4(ambient + diffuse + specular, 1.0);\n" \
    "}\n" \
    "else\n" \
    "{\n" \
    "fong_ads_light = vec4(1.0, 1.0, 1.0, 1.0);\n" \
    "}\n" \
    "vec4 tex = texture(u_textureSampler, a_texcoord_out);\n" \
    "FragColor = fong_ads_light * tex * a_color_out;\n" \
    "}\n";

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
    glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_COLOR, "a_color");
    glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_TEXTURE0, "a_texcoord");       // Andhaar
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
    
    laUniform = glGetUniformLocation(shaderProgramObject, "u_la");
    ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
    lsUniform = glGetUniformLocation(shaderProgramObject, "u_ls");
    lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");

    kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
    kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
    ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
    materialShininessUniform = glGetUniformLocation(shaderProgramObject, "u_materialShininess");

    lightingEnabledUniform = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");
    textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

    // Declaration of vertex data arrays
    const GLfloat cubepcnt[] =
    {
        //front             //color-red         //normal-front      //texture-front
        1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,

        //right             //color-green       //normal-right      //texture-right
        1.0f, 1.0f, -1.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,

        //back              //color-blue        //normal-back       //texture-back
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f,

        //left              //color-cyan        //normal-left       //texture-back
        -1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

        //top               //color-magenta     //normal-top        //texture-top
        1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,

        //bottom            //color-yellow      //normal-bottom     //texture-bottom
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,1.0f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
        1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f
    };

    // Cube
    // Vao related code
    glGenVertexArrays(1, &vao_cube);
    glBindVertexArray(vao_cube);
    // Vbo for position
    glGenBuffers(1, &vbo_cube);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubepcnt), cubepcnt, GL_STATIC_DRAW);    // sizeof(cubepcnt) is nothing but (11*24)*sizeof(float).
    // Position
    glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (void *)(0));
    glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);
    // Color
    glVertexAttribPointer(SAB_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(SAB_ATTRIBUTE_COLOR);
    // Normals
    glVertexAttribPointer(SAB_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (void *)(6 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(SAB_ATTRIBUTE_NORMAL);
    // Texture
    glVertexAttribPointer(SAB_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (void *)(9 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(SAB_ATTRIBUTE_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Vao unbind
    glBindVertexArray(0);
    
    // Clear the screen using black color
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    // Depth Related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // Enabling the texture
    glEnable(GL_TEXTURE_2D);
    
    // Arrays Initialization
    lightAmbient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    lightDiffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    lightSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    lightPosition = vec4(100.0f, 100.0f, 100.0f, 1.0f);
    
    materialAmbient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    materialDiffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialShininess = 50.0f;
    
    // bLight Initialize
    bLight = NO;
    
    texture_marble = [self LoadGLTexture:"marble.bmp"];
    if(texture_marble == 0)
    {
        fprintf(gpFile, "initialize:LoadGLTexture failed.\n");
        [self uninitialize];
        [self release];
        [NSApp terminate:self];
    }

    perspectiveProjectionMatrix = mat4::identity();
    // Warmup Resize Call
    [self resize:WIN_WIDTH :WIN_HEIGHT];
    return 0;
}

- (GLuint)LoadGLTexture:(const char*)textureFileName
{
    // Code
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appDirPath = [appBundle bundlePath];
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *textureFileNameWithPath = [NSString stringWithFormat:@"%@/%s",parentDirPath,textureFileName];
    if(textureFileNameWithPath == nil)
    {
        fprintf(gpFile, "Failed to create textureFilePath.\n");
        [NSApp terminate:self];
        return 0;
    }
    
    // GetNSImage representation of texture file
    NSImage *nsImage = [[NSImage alloc]initWithContentsOfFile:textureFileNameWithPath];
    if(nsImage == nil)
    {
        fprintf(gpFile, "nsImage failed");
        [NSApp terminate:self];
        return 0;
    }
    // From above NSImage representation obtain CGImage representation of texture file
    CGImageRef cgImage = [nsImage CGImageForProposedRect:nil context:nil hints:nil];
    // From this CGImage representation get width and height of the image
    int width = (int)CGImageGetWidth(cgImage);
    int height = (int)CGImageGetHeight(cgImage);
    // From this CGImage representation get CGDataProvider
    CGDataProviderRef cgDataProvider = CGImageGetDataProvider(cgImage);
    // Using CGDataProvider get imageData in the form of CFData(Core Foundation)
    CFDataRef imageData = CGDataProviderCopyData(cgDataProvider);
    // Convert this CFData formatted imageData into void* from const UINT8*
    void* pixels = (void*)CFDataGetBytePtr(imageData);
    // Proceed with usual textureCreation code from onwards
    GLuint texture = 0;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Create The Texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    // Release imageData
    CFRelease(imageData);
    
    return texture;
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
    // cube
    // Transformations
    mat4 translationMatrix = mat4::identity();
    mat4 rotationMatrix = mat4::identity();
    mat4 rotationMatrix_x = mat4::identity();
    mat4 rotationMatrix_y = mat4::identity();
    mat4 rotationMatrix_z = mat4::identity();
    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();
    translationMatrix = translate(0.0f, 0.0f, -6.0f);
    rotationMatrix_x = rotate(anglecube, 1.0f, 0.0f, 0.0f);
    rotationMatrix_y = rotate(anglecube, 0.0f, 1.0f, 0.0f);
    rotationMatrix_z = rotate(anglecube, 0.0f, 0.0f, 1.0f);
    rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
    modelMatrix = translationMatrix * rotationMatrix;

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
    
    if(bLight == TRUE)
    {
        glUniform1i(lightingEnabledUniform, 1);

        glUniform3fv(laUniform, 1, lightAmbient);
        glUniform3fv(ldUniform, 1, lightDiffuse);
        glUniform3fv(lsUniform, 1, lightSpecular);
        glUniform4fv(lightPositionUniform, 1, lightPosition);

        glUniform3fv(kaUniform, 1, materialAmbient);
        glUniform3fv(kdUniform, 1, materialDiffuse);
        glUniform3fv(ksUniform, 1, materialSpecular);
        glUniform1f(materialShininessUniform, materialShininess);

    }
    else
    {
        glUniform1i(lightingEnabledUniform, 0);
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_marble);
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
    anglecube = anglecube + 1.0f;
    if(anglecube >= 360.0f)
        anglecube -= 360.0f;
}

- (void)uninitialize
{
    // Code
    // Deletion and uninitialization of vbo_cube_normal
    if(texture_marble)
    {
        glDeleteTextures(1, &texture_marble);
        texture_marble = 0;
    }
    // Deletion and uninitialization of vbo_cube_position
    if(vbo_cube)
    {
        glDeleteBuffers(1, &vbo_cube);
        vbo_cube = 0;
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
