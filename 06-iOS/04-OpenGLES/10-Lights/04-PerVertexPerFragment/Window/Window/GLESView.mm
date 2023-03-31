//
//  MyView.m
//  Window
//
//  Created by user224299 on 12/25/22.
//
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#import "GLESView.h"
#import "vmath.h"
#import "Sphere.h" // Sphere sathi
using namespace vmath;


enum
{
	SAB_ATTRIBUTE_POSITION = 0,
	SAB_ATTRIBUTE_COLOR,
	SAB_ATTRIBUTE_NORMAL,
	SAB_ATTRIBUTE_TEXURE0
};

@implementation GLESView
{
	@private
	// Class variables
	EAGLContext *eaglContext;
	CAEAGLLayer *eaglLayer;
	
	GLuint defaultFramebuffer;
	GLuint colorRenderbuffer;
	GLuint depthRenderbuffer;
	
	CADisplayLink *displayLink;	//CoreAnimation
	GLint fps;
	BOOL isAnimating;
	
	// Programmable Pipeline Related Variables
	GLuint shaderProgramObject1;
	GLuint shaderProgramObject2;
	GLchar choosenProgramObject;
	
	GLuint vao_sphere;
	GLuint vbo_sphere_position;
	GLuint vbo_sphere_normal;
	GLuint vbo_sphere_element;
	
	GLuint modelMatrixUniformV;
	GLuint viewMatrixUniformV;
	GLuint projectionMatrixUniformV;

	GLuint modelMatrixUniformF;
	GLuint viewMatrixUniformF;
	GLuint projectionMatrixUniformF;

	// Sphere arrays
	float sphere_vertices[1146];
	float sphere_normals[1146];
	float sphere_textures[764];
	short sphere_elements[2280];
	GLuint numElements;
	GLuint numVertices;
	
	// Lights Variables
	GLuint ldUniformV;
	GLuint laUniformV;
	GLuint lsUniformV;
	GLuint lightPositionUniformV;

	GLuint kaUniformV;
	GLuint kdUniformV;
	GLuint ksUniformV;
	GLuint materialShininessUniformV;

	GLuint lightingEnabledUniformV;

	GLuint ldUniformF;
	GLuint laUniformF;
	GLuint lsUniformF;
	GLuint lightPositionUniformF;

	GLuint kaUniformF;
	GLuint kdUniformF;
	GLuint ksUniformF;
	GLuint materialShininessUniformF;

	GLuint lightingEnabledUniformF;
	BOOL bLight;
	
	GLfloat lightAmbient[4];
	GLfloat lightDiffuse[4];
	GLfloat lightSpecular[4];
	GLfloat lightPosition[4];
	
	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];
	GLfloat materialShininess;
	
	mat4 perspectiveProjectionMatrix;
}
- (id)initWithFrame:(CGRect)frame
{
    // Code
    self = [super initWithFrame:frame];
    if(self)
    {
		// OpenGL code
		// Create Drawable surface (layer)
		// To allow this to succeed, we have to implement static layer class
		eaglLayer = (CAEAGLLayer*)[super layer];
		
		// Set EAGLLayer properties (similar to setting attributes of pixelformatdescriptor
		[eaglLayer setOpaque:YES];
		
		NSNumber *boolNumber = [NSNumber numberWithBool:NO];
		NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:boolNumber,kEAGLDrawablePropertyRetainedBacking,kEAGLColorFormatRGBA8,kEAGLDrawablePropertyColorFormat,nil];
		[eaglLayer setDrawableProperties:dictionary];
		
		// Create EAGL context
		eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
		if(eaglContext == nil)
		{
			printf("failed to create eagl context.\n");
			[self uninitialize];
			[self release];
			exit(0);
		}
		
		// Set this context as current context
		[EAGLContext setCurrentContext:eaglContext];
		// OpenGL info
		printf("Renderer : %s\n", glGetString(GL_RENDERER));
		printf("Version : %s\n", glGetString(GL_VERSION));
		printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		
		// Create framebuffer
		glGenFramebuffers(1, &defaultFramebuffer);
		
		// Bind with above framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
		
		// Create color renderbuffer
		glGenRenderbuffers(1, &colorRenderbuffer);
		// Bind with above renderbuffer
		glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
		
		// Provide storage to renderbuffer
		[eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
		// Set framebuffer according to renderbuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
		
		// Declare backing width and height
		GLint backingWidth;
		GLint backingHeight;
		
		// Get color renderbuffer width into backing width
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
		
		// Create depth renderbuffer
		glGenRenderbuffers(1, &depthRenderbuffer);
		// Bind with above renderbuffer
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
		
		// Provide storage to depth renderbuffer
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
		
		// Connect renderbuffer to framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
		
		// Check the status of the framebuffer
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("Incomplete Framebuffer.\n");
			[self uninitialize];
			[self release];
			exit(0);
		}
		
		// Initialize call
		int result = [self initialize];
		if(result == 0)
		{
			printf("initialize successful.\n");
		}
		fps = 60;	// This started from IOS 8.0
		isAnimating = NO;
		
		// GestureRecognizer Code
        // Single Tap
        // Target Action pattern is followed by all gestureRecognizers
        UITapGestureRecognizer* singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];

        // Double Tap
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];

        // Differentiate between single and double tap
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];

        // Swipe
        UISwipeGestureRecognizer* swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [self addGestureRecognizer:swipeGestureRecognizer];

        // Long press
        UILongPressGestureRecognizer* longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
    }
    return self;
}

+ (Class)layerClass
{
	// Code
	return [CAEAGLLayer class];
}

- (void)layoutSubviews
{
	// Code
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	[eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];	// By default UI view conforms CALayerDelegate protocol
	
	// Declare backing width and height
	GLint backingWidth;
	GLint backingHeight;
	
	// Get color renderbuffer width into backing width
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
	
	// Create depth renderbuffer
	glGenRenderbuffers(1, &depthRenderbuffer);
	// Bind with above renderbuffer
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	
	// Provide storage to depth renderbuffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
	
	// Connect renderbuffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	
	// Check the status of the framebuffer
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Incomplete Framebuffer.\n");
		[self uninitialize];
		[self release];
		exit(0);
	}
	// Call our resize here
	[self resize:backingWidth :backingHeight];
	// Call drawview after resize
	[self drawView:self];
}

//- (void)drawRect:(CGRect)dirtyRect
//{
//	// Code
//}

- (void)drawView:(id)sender
{
	// Code
	[EAGLContext setCurrentContext:eaglContext];
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
	
	// Call our display and update here
	[self display];
	[self myupdate];
	
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	// For double buffering
	[eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)startAnimation
{
	// Code
	if(isAnimating == NO)
	{
		// Create display link
		displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
		
		// Set frames per second for the display link
		[displayLink setPreferredFramesPerSecond:fps];
		// Add our displaylink to run loop/ game loop
		[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		
		isAnimating = YES;
	}
}

- (void)stopAnimation
{
	// Code
	if(isAnimating == YES)
	{
		// Remove displaylink from the run loop
		[displayLink invalidate];
		
		isAnimating = NO;
	}
}

- (int)initialize
{
	// Vertex Shader
	// Shader Source Code
	const GLchar *vertexShaderSourceCode1 =
	"#version 300 es" \
	"\n" \
	"precision highp float;" \
	"precision highp int;" \
	"in vec4 a_position;\n" \
	"in vec3 a_normal;\n" \
	"uniform mat4 u_modelMatrix;\n" \
	"uniform mat4 u_viewMatrix;\n" \
	"uniform mat4 u_projectionMatrix;\n" \
	"uniform vec3 u_la;\n" \
	"uniform vec3 u_ld;" \
	"uniform vec3 u_ls;" \
	"uniform vec4 u_lightPosition;" \
	"uniform vec3 u_ka;" \
	"uniform vec3 u_kd;" \
	"uniform vec3 u_ks;" \
	"uniform float u_materialShininess;" \
	"uniform int u_lightingEnabled;" \
	"out vec3 fong_ads_light;" \
	"void main(void)" \
	"{" \
	"if(u_lightingEnabled == 1)" \
	"{" \
	"vec3 ambient = u_la * u_ka;" \
	"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" \
	"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" \
	"vec3 transformedNormals = normalize(normalMatrix * a_normal);" \
	"vec3 lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);" \
	"vec3 diffuse = u_ld * u_kd * max(dot(lightDirection, transformedNormals), 0.0);" \
	"vec3 reflectionVector = reflect(-lightDirection, transformedNormals);" \
	"vec3 viewerVector = normalize(-eyeCoordinates.xyz);" \
	"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, viewerVector), 0.0), u_materialShininess);" \
	"fong_ads_light = ambient + diffuse + specular;" \
	"}" \
	"else" \
	"{" \
	"fong_ads_light = vec3(1.0, 1.0, 1.0);" \
	"}" \
	"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
	"}";
	// Vertex Shader cha Object tayar kela
	GLuint vertexShaderObject1 = glCreateShader(GL_VERTEX_SHADER);
	// Object la Source code dila
	glShaderSource(vertexShaderObject1, 1, (const GLchar**)&vertexShaderSourceCode1, NULL);
	// GPU chya inline compiler la code compile karaila dila
	glCompileShader(vertexShaderObject1);
	// Error checking
	GLint status;
	GLint infoLogLength;
	char *log = NULL;

	glGetShaderiv(vertexShaderObject1, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject1, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject1, infoLogLength, &written, log);
				printf("Vertex Shader Compilation Log : %s\n", log);
				free(log);
				[self uninitialize];
			}
		}
	}

	// Fragment Shader
	const GLchar *fragmentShaderSourceCode1 =
	"#version 300 es" \
	"\n" \
	"precision highp float;" \
	"precision highp int;" \
	"in vec3 fong_ads_light;" \
	"out vec4 FragColor;" \
	"void main(void)" \
	"{" \
	"FragColor = vec4(fong_ads_light,1.0);" \
	"}";

	GLuint fragmentShaderObject1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject1, 1, (const GLchar **)&fragmentShaderSourceCode1, NULL);
	glCompileShader(fragmentShaderObject1);
	// Error Checking
	status = 0;
	infoLogLength = 0;
	log = NULL;

	glGetShaderiv(fragmentShaderObject1, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject1, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject1, infoLogLength, &written, log);
				printf("Fragment Shader Compilation Log : %s\n", log);
				free(log);
				[self uninitialize];
			}
		}
	}

	// Shader Program Object
	shaderProgramObject1 = glCreateProgram();
	glAttachShader(shaderProgramObject1, vertexShaderObject1);
	glAttachShader(shaderProgramObject1, fragmentShaderObject1);
	glBindAttribLocation(shaderProgramObject1, SAB_ATTRIBUTE_POSITION, "a_position"); // Andhaar
	glBindAttribLocation(shaderProgramObject1, SAB_ATTRIBUTE_NORMAL, "a_normal");
	glLinkProgram(shaderProgramObject1);
	// Error Checking
	status = 0;
	infoLogLength = 0;
	log = NULL;
	glGetProgramiv(shaderProgramObject1, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject1, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject1, infoLogLength, &written, log);
				printf("Shader Program Link Log : %s\n", log);
				free(log);
				[self uninitialize];
			}
		}
	}

	// Shader 2 for per fragment
	// Vertex Shader
	// Shader Source Code
	const GLchar *vertexShaderSourceCode2 =
	"#version 300 es" \
	"\n" \
	"precision highp float;" \
	"precision highp int;" \
	"in vec4 a_position;\n" \
	"in vec3 a_normal;\n" \
	"uniform mat4 u_modelMatrix;\n" \
	"uniform mat4 u_viewMatrix;\n" \
	"uniform mat4 u_projectionMatrix;\n" \
	"uniform vec4 u_lightPosition;\n" \
	"uniform int u_lightingEnabled;\n" \
	"out vec3 transformedNormals;\n" \
	"out vec3 lightDirection;\n" \
	"out vec3 viewerVector;\n" \
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
	"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" \
	"}\n";
	// Vertex Shader cha Object tayar kela
	GLuint vertexShaderObject2 = glCreateShader(GL_VERTEX_SHADER);
	// Object la Source code dila
	glShaderSource(vertexShaderObject2, 1, (const GLchar**)&vertexShaderSourceCode2, NULL);
	// GPU chya inline compiler la code compile karaila dila
	glCompileShader(vertexShaderObject2);
	// Error checking
	status = 0;
	infoLogLength = 0;
	log = NULL;

	glGetShaderiv(vertexShaderObject2, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject2, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject2, infoLogLength, &written, log);
				printf("Vertex Shader Compilation Log : %s\n", log);
				free(log);
				[self uninitialize];
			}
		}
	}

	// Fragment Shader
	const GLchar *fragmentShaderSourceCode2 =
	"#version 300 es" \
	"\n" \
	"precision highp float;" \
	"precision highp int;" \
	"in vec3 transformedNormals;\n" \
	"in vec3 lightDirection;\n" \
	"in vec3 viewerVector;\n" \
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
	"vec3 normalized_lightDirection = normalize(lightDirection);\n"
	"vec3 diffuse = u_ld * u_kd * max(dot(normalized_lightDirection, normalized_transformedNormals), 0.0);\n" \
	"vec3 reflectionVector = reflect(-normalized_lightDirection, normalized_transformedNormals);\n" \
	"vec3 normalized_viewerVector = normalize(viewerVector);\n"
	"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewerVector), 0.0), u_materialShininess);\n" \
	"fong_ads_light = vec4(ambient + diffuse + specular, 1.0);\n" \
	"}\n" \
	"else\n" \
	"{\n" \
	"fong_ads_light = vec4(1.0, 1.0, 1.0, 1.0);\n" \
	"}\n" \
	"FragColor = fong_ads_light;\n" \
	"}\n";

	GLuint fragmentShaderObject2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject2, 1, (const GLchar **)&fragmentShaderSourceCode2, NULL);
	glCompileShader(fragmentShaderObject2);
	// Error Checking
	status = 0;
	infoLogLength = 0;
	log = NULL;

	glGetShaderiv(fragmentShaderObject2, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject2, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject2, infoLogLength, &written, log);
				printf("Fragment Shader Compilation Log : %s\n", log);
				free(log);
				[self uninitialize];
			}
		}
	}

	// Shader Program Object
	shaderProgramObject2 = glCreateProgram();
	glAttachShader(shaderProgramObject2, vertexShaderObject2);
	glAttachShader(shaderProgramObject2, fragmentShaderObject2);
	glBindAttribLocation(shaderProgramObject2, SAB_ATTRIBUTE_POSITION, "a_position"); // Andhaar
	glBindAttribLocation(shaderProgramObject2, SAB_ATTRIBUTE_NORMAL, "a_normal");
	glLinkProgram(shaderProgramObject2);
	// Error Checking
	status = 0;
	infoLogLength = 0;
	log = NULL;
	glGetProgramiv(shaderProgramObject2, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject2, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject2, infoLogLength, &written, log);
				printf("Shader Program Link Log : %s\n", log);
				free(log);
				[self uninitialize];
			}
		}
	}

	// Shader 1 uniforms
	modelMatrixUniformV = glGetUniformLocation(shaderProgramObject1, "u_modelMatrix");
	viewMatrixUniformV = glGetUniformLocation(shaderProgramObject1, "u_viewMatrix");
	projectionMatrixUniformV = glGetUniformLocation(shaderProgramObject1, "u_projectionMatrix");
	
	laUniformV = glGetUniformLocation(shaderProgramObject1, "u_la");
	ldUniformV = glGetUniformLocation(shaderProgramObject1, "u_ld");
	lsUniformV = glGetUniformLocation(shaderProgramObject1, "u_ls");
	lightPositionUniformV = glGetUniformLocation(shaderProgramObject1, "u_lightPosition");

	kaUniformV = glGetUniformLocation(shaderProgramObject1, "u_ka");
	kdUniformV = glGetUniformLocation(shaderProgramObject1, "u_kd");
	ksUniformV = glGetUniformLocation(shaderProgramObject1, "u_ks");
	materialShininessUniformV = glGetUniformLocation(shaderProgramObject1, "u_materialShininess");

	lightingEnabledUniformV = glGetUniformLocation(shaderProgramObject1, "u_lightingEnabled");

	// Shader 2 uniforms
	modelMatrixUniformF = glGetUniformLocation(shaderProgramObject2, "u_modelMatrix");
	viewMatrixUniformF = glGetUniformLocation(shaderProgramObject2, "u_viewMatrix");
	projectionMatrixUniformF = glGetUniformLocation(shaderProgramObject2, "u_projectionMatrix");
	
	laUniformF = glGetUniformLocation(shaderProgramObject2, "u_la");
	ldUniformF = glGetUniformLocation(shaderProgramObject2, "u_ld");
	lsUniformF = glGetUniformLocation(shaderProgramObject2, "u_ls");
	lightPositionUniformF = glGetUniformLocation(shaderProgramObject2, "u_lightPosition");

	kaUniformF = glGetUniformLocation(shaderProgramObject2, "u_ka");
	kdUniformF = glGetUniformLocation(shaderProgramObject2, "u_kd");
	ksUniformF = glGetUniformLocation(shaderProgramObject2, "u_ks");
	materialShininessUniformF = glGetUniformLocation(shaderProgramObject2, "u_materialShininess");

	lightingEnabledUniformF = glGetUniformLocation(shaderProgramObject2, "u_lightingEnabled");
	// Default Initialization of Toggled state
	choosenProgramObject = 'v';
	
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
	lightAmbient[0] = 0.1f;
	lightAmbient[1] = 0.1f;
	lightAmbient[2] = 0.1f;
	lightAmbient[3] = 1.0f;
	
	lightDiffuse[0] = 1.0f;
	lightDiffuse[1] = 1.0f;
	lightDiffuse[2] = 1.0f;
	lightDiffuse[3] = 1.0f;
	
	lightSpecular[0] = 1.0f;
	lightSpecular[1] = 1.0f;
	lightSpecular[2] = 1.0f;
	lightSpecular[3] = 1.0f;
	
	lightPosition[0] = 100.0f;
	lightPosition[1] = 100.0f;
	lightPosition[2] = 100.0f;
	lightPosition[3] = 1.0f;
	
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 1.0f;
	materialDiffuse[1] = 1.0f;
	materialDiffuse[2] = 1.0f;
	materialDiffuse[3] = 1.0f;
	
	materialSpecular[0] = 1.0f;
	materialSpecular[1] = 1.0f;
	materialSpecular[2] = 1.0f;
	materialSpecular[3] = 1.0f;
	
	materialShininess = 50.0f;
	
	// bLight Initialize
	bLight = NO;

	// Depth Related Changes
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	perspectiveProjectionMatrix = mat4::identity();
	return 0;
}

- (void)resize:(int)width :(int)height
{
	// Code
	if(height < 0)
		height = 1;
	glViewport(0, 0, width, height);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

- (void)display
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Transformations
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 translationMatrix = translate(0.0f, 0.0f, -2.0f);
	modelMatrix = translationMatrix;

	// Use the Shader Program Object
	if(choosenProgramObject == 'v')
	{
		glUseProgram(shaderProgramObject1);

		glUniformMatrix4fv(modelMatrixUniformV, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniformV, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniformV, 1, GL_FALSE, perspectiveProjectionMatrix);
		
		if(bLight == YES)
		{
			glUniform1i(lightingEnabledUniformV, 1);

			glUniform3fv(laUniformV, 1, lightAmbient);
			glUniform3fv(ldUniformV, 1, lightDiffuse);
			glUniform3fv(lsUniformV, 1, lightSpecular);
			glUniform4fv(lightPositionUniformV, 1, lightPosition);

			glUniform3fv(kaUniformV, 1, materialAmbient);
			glUniform3fv(kdUniformV, 1, materialDiffuse);
			glUniform3fv(ksUniformV, 1, materialSpecular);
			glUniform1f(materialShininessUniformV, materialShininess);

		}
		else
		{
			glUniform1i(lightingEnabledUniformV, 0);
		}
	}
	else
	{
		glUseProgram(shaderProgramObject2);

		glUniformMatrix4fv(modelMatrixUniformF, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniformF, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniformF, 1, GL_FALSE, perspectiveProjectionMatrix);
		
		if(bLight == YES)
		{
			glUniform1i(lightingEnabledUniformF, 1);

			glUniform3fv(laUniformF, 1, lightAmbient);
			glUniform3fv(ldUniformF, 1, lightDiffuse);
			glUniform3fv(lsUniformF, 1, lightSpecular);
			glUniform4fv(lightPositionUniformF, 1, lightPosition);

			glUniform3fv(kaUniformF, 1, materialAmbient);
			glUniform3fv(kdUniformF, 1, materialDiffuse);
			glUniform3fv(ksUniformF, 1, materialSpecular);
			glUniform1f(materialShininessUniformF, materialShininess);

		}
		else
		{
			glUniform1i(lightingEnabledUniformF, 0);
		}
	}
	 // *** bind vao ***
	glBindVertexArray(vao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
}

- (void)myupdate
{
	// Code
}

- (void)uninitialize
{
	// Code
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
   if(shaderProgramObject1)
   {
	   glUseProgram(shaderProgramObject1);
	   GLsizei numAttachedShaders;
	   glGetProgramiv(shaderProgramObject1, GL_ATTACHED_SHADERS, &numAttachedShaders);
	   GLuint *shaderObjects;
	   shaderObjects = (GLuint *)malloc(numAttachedShaders*sizeof(GLuint));
	   glGetAttachedShaders(shaderProgramObject1, numAttachedShaders, &numAttachedShaders, shaderObjects);
	   for(GLsizei i = 0; i < numAttachedShaders; i++)
	   {
		   glDetachShader(shaderProgramObject1, shaderObjects[i]);
		   glDeleteShader(shaderObjects[i]);
		   shaderObjects[i] = 0;
	   }
	   free(shaderObjects);
	   shaderObjects = NULL;
	   glUseProgram(0);
	   glDeleteProgram(shaderProgramObject1);
	   shaderProgramObject1 = 0;
   }
   if(shaderProgramObject2)
   {
	   glUseProgram(shaderProgramObject2);
	   GLsizei numAttachedShaders;
	   glGetProgramiv(shaderProgramObject2, GL_ATTACHED_SHADERS, &numAttachedShaders);
	   GLuint *shaderObjects;
	   shaderObjects = (GLuint *)malloc(numAttachedShaders*sizeof(GLuint));
	   glGetAttachedShaders(shaderProgramObject2, numAttachedShaders, &numAttachedShaders, shaderObjects);
	   for(GLsizei i = 0; i < numAttachedShaders; i++)
	   {
		   glDetachShader(shaderProgramObject2, shaderObjects[i]);
		   glDeleteShader(shaderObjects[i]);
		   shaderObjects[i] = 0;
	   }
	   free(shaderObjects);
	   shaderObjects = NULL;
	   glUseProgram(0);
	   glDeleteProgram(shaderProgramObject2);
	   shaderProgramObject2 = 0;
   }
	if(depthRenderbuffer)
	{
		glDeleteRenderbuffers(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
	if(colorRenderbuffer)
	{
		glDeleteRenderbuffers(1, &colorRenderbuffer);
		colorRenderbuffer = 0;
	}
	if(defaultFramebuffer)
	{
		glDeleteFramebuffers(1, &defaultFramebuffer);
		defaultFramebuffer = 0;
	}
	if([EAGLContext currentContext] == eaglContext)
	{
		[EAGLContext setCurrentContext:nil];
		[eaglContext release];
		eaglContext = nil;
	}
}

- (BOOL)acceptsFirstResponder
{
    // Code
    return YES;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)withEvent
{
    // Code
}

- (void)onSingleTap:(UITapGestureRecognizer*)gr
{
    // Code
	if(bLight == NO)
	{
		bLight = YES;
	}
	else
	{
		bLight = NO;
	}
}

- (void)onDoubleTap:(UITapGestureRecognizer*)gr
{
    // Code
	if(choosenProgramObject == 'v')
	{
		choosenProgramObject = 'f';
	}
	else
	{
		choosenProgramObject = 'v';
	}
}

- (void)onSwipe:(UISwipeGestureRecognizer*)sgr
{
    // Code
	[self uninitialize];
    [self release];
    exit(0);
}

- (void)onLongPress:(UILongPressGestureRecognizer*)lpgr
{
    // Code
}

- (void)dealloc
{
    // Code
	[self uninitialize];
	if(displayLink)
	{
		[displayLink release];
		displayLink = nil;
	}
    [super dealloc];
}

@end
