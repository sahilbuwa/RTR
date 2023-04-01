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
	BOOL bLight;
	GLuint ldUniform;
	GLuint laUniform;
	GLuint lsUniform;
	GLuint lightPositionUniform;

	GLuint kaUniform;
	GLuint kdUniform;
	GLuint ksUniform;
	GLuint materialShininessUniform;

	GLuint lightingEnabledUniform;

	struct Light
	{
		vec4 lightAmbient;
		vec4 lightDiffuse;
		vec4 lightSpecular;
		vec4 lightPosition;
	};
	Light light;

	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];
	GLfloat materialShininess;

	GLfloat angleForXRotation;
	GLfloat angleForYRotation;
	GLfloat angleForZRotation;

	GLint keyPressed;
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
	// Code
	// Vertex Shader
	// Shader Source Code
	const GLchar *vertexShaderSourceCode =
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
				printf("Vertex Shader Compilation Log : %s\n", log);
				free(log);
				[self uninitialize];
			}
		}
	}

	// Fragment Shader
	const GLchar *fragmentShaderSourceCode =
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
	"FragColor = fong_ads_light;\n" \
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
				printf("Fragment Shader Compilation Log : %s\n", log);
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
				printf("Shader Program Link Log : %s\n", log);
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
	
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// Clear the screen using grey color
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	
	// Arrays Initialization
	light.lightAmbient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light.lightDiffuse = vec4(1.0f, 1.0f , 1.0f , 1.0f);
	light.lightSpecular = vec4(1.0f, 1.0f , 1.0f , 1.0f);
	light.lightPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// bLight Initialize
	bLight = NO;
	keyPressed = -1;
	
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
	// Use the Shader Program Object
	glUseProgram(shaderProgramObject);
	
	// Transformations
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = lookat(vec3(-1.0f, 0.0f, 0.0f),
							 vec3(0.0f, 0.0f, 50.0f),
							 vec3(0.0f, 1.0f, 0.0f));
	mat4 scaleMatrix = mat4::identity();
	mat4 translationMatrix = mat4::identity();

	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	
	if(bLight == YES)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, light.lightAmbient);
		glUniform3fv(ldUniform, 1, light.lightDiffuse);
		glUniform3fv(lsUniform, 1, light.lightSpecular);
		glUniform4fv(lightPositionUniform, 1, light.lightPosition);

	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	// Code
	// ***** 1st sphere on 1st column, emerald *****
	// ambient material
	materialAmbient[0] = 0.0215; // r
	materialAmbient[1] = 0.1745; // g
	materialAmbient[2] = 0.0215; // b
	materialAmbient[3] = 1.0f;   // a

	// diffuse material
	materialDiffuse[0] = 0.07568; // r
	materialDiffuse[1] = 0.61424; // g
	materialDiffuse[2] = 0.07568; // b
	materialDiffuse[3] = 1.0f;    // a
	
	// specular material
	materialSpecular[0] = 0.633;    // r
	materialSpecular[1] = 0.727811; // g
	materialSpecular[2] = 0.633;    // b
	materialSpecular[3] = 1.0f;     // a
	
	// materialShininess
	materialShininess = 0.6 * 128;
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(4.0f, 3.15f, 10.0f);
	// scaleMatrix = scale(1.0f, 1.0f, 0.8f);
	modelMatrix = translationMatrix * scaleMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	
	// // *******************************************************

	// ***** 2nd sphere on 1st column, jade *****
	// ambient material
	materialAmbient[0] = 0.135;  // r
	materialAmbient[1] = 0.2225; // g
	materialAmbient[2] = 0.1575; // b
	materialAmbient[3] = 1.0f;   // a
	

	// diffuse material
	materialDiffuse[0] = 0.54; // r
	materialDiffuse[1] = 0.89; // g
	materialDiffuse[2] = 0.63; // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.316228; // r
	materialSpecular[1] = 0.316228; // g
	materialSpecular[2] = 0.316228; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.1 * 128;
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(4.0f,1.9f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 3rd sphere on 1st column, obsidian *****
	// ambient material
	materialAmbient[0] = 0.05375; // r
	materialAmbient[1] = 0.05;    // g
	materialAmbient[2] = 0.06625; // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 0.18275; // r
	materialDiffuse[1] = 0.17;    // g
	materialDiffuse[2] = 0.22525; // b
	materialDiffuse[3] = 1.0f;    // a
	

	// specular material
	materialSpecular[0] = 0.332741; // r
	materialSpecular[1] = 0.328634; // g
	materialSpecular[2] = 0.346435; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.3 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(4.0f,0.65f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 4th sphere on 1st column, pearl *****
	// ambient material
	materialAmbient[0] = 0.25;    // r
	materialAmbient[1] = 0.20725; // g
	materialAmbient[2] = 0.20725; // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 1.0;   // r
	materialDiffuse[1] = 0.829; // g
	materialDiffuse[2] = 0.829; // b
	materialDiffuse[3] = 1.0f;  // a
	

	// specular material
	materialSpecular[0] = 0.296648; // r
	materialSpecular[1] = 0.296648; // g
	materialSpecular[2] = 0.296648; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.088 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(4.0f, -0.6f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 5th sphere on 1st column, ruby *****
	// ambient material
	materialAmbient[0] = 0.1745;  // r
	materialAmbient[1] = 0.01175; // g
	materialAmbient[2] = 0.01175; // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 0.61424; // r
	materialDiffuse[1] = 0.04136; // g
	materialDiffuse[2] = 0.04136; // b
	materialDiffuse[3] = 1.0f;    // a
	

	// specular material
	materialSpecular[0] = 0.727811; // r
	materialSpecular[1] = 0.626959; // g
	materialSpecular[2] = 0.626959; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.6 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(4.0f, -1.85f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 6th sphere on 1st column, turquoise *****
	// ambient material
	materialAmbient[0] = 0.1;     // r
	materialAmbient[1] = 0.18725; // g
	materialAmbient[2] = 0.1745;  // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 0.396;   // r
	materialDiffuse[1] = 0.74151; // g
	materialDiffuse[2] = 0.69102; // b
	materialDiffuse[3] = 1.0f;    // a
	

	// specular material
	materialSpecular[0] = 0.297254; // r
	materialSpecular[1] = 0.30829;  // g
	materialSpecular[2] = 0.306678; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.1 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(4.0f, -3.1f, 10.0f);
	// scaleMatrix = scale(1.0f, 1.0f, 0.8f);
	modelMatrix = translationMatrix * scaleMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 2nd column, brass *****
	// ambient material
	materialAmbient[0] = 0.329412; // r
	materialAmbient[1] = 0.223529; // g
	materialAmbient[2] = 0.027451; // b
	materialAmbient[3] = 1.0f;     // a
	

	// diffuse material
	materialDiffuse[0] = 0.780392; // r
	materialDiffuse[1] = 0.568627; // g
	materialDiffuse[2] = 0.113725; // b
	materialDiffuse[3] = 1.0f;     // a
	

	// specular material
	materialSpecular[0] = 0.992157; // r
	materialSpecular[1] = 0.941176; // g
	materialSpecular[2] = 0.807843; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.21794872 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(1.0f,3.15f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 2nd sphere on 2nd column, bronze *****
	// ambient material
	materialAmbient[0] = 0.2125; // r
	materialAmbient[1] = 0.1275; // g
	materialAmbient[2] = 0.054;  // b
	materialAmbient[3] = 1.0f;   // a
	

	// diffuse material
	materialDiffuse[0] = 0.714;   // r
	materialDiffuse[1] = 0.4284;  // g
	materialDiffuse[2] = 0.18144; // b
	materialDiffuse[3] = 1.0f;    // a
	

	// specular material
	materialSpecular[0] = 0.393548; // r
	materialSpecular[1] = 0.271906; // g
	materialSpecular[2] = 0.166721; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.2 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(1.0f,1.9f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 3rd sphere on 2nd column, chrome *****
	// ambient material
	materialAmbient[0] = 0.25; // r
	materialAmbient[1] = 0.25; // g
	materialAmbient[2] = 0.25; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.774597; // r
	materialSpecular[1] = 0.774597; // g
	materialSpecular[2] = 0.774597; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.6 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(1.0f,0.65f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 4th sphere on 2nd column, copper *****
	// ambient material
	materialAmbient[0] = 0.19125; // r
	materialAmbient[1] = 0.0735;  // g
	materialAmbient[2] = 0.0225;  // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 0.7038;  // r
	materialDiffuse[1] = 0.27048; // g
	materialDiffuse[2] = 0.0828;  // b
	materialDiffuse[3] = 1.0f;    // a
	

	// specular material
	materialSpecular[0] = 0.256777; // r
	materialSpecular[1] = 0.137622; // g
	materialSpecular[2] = 0.086014; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.1 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(1.0f, -0.6f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 5th sphere on 2nd column, gold *****
	// ambient material
	materialAmbient[0] = 0.24725; // r
	materialAmbient[1] = 0.1995;  // g
	materialAmbient[2] = 0.0745;  // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 0.75164; // r
	materialDiffuse[1] = 0.60648; // g
	materialDiffuse[2] = 0.22648; // b
	materialDiffuse[3] = 1.0f;    // a
	

	// specular material
	materialSpecular[0] = 0.628281; // r
	materialSpecular[1] = 0.555802; // g
	materialSpecular[2] = 0.366065; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.4 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(1.0f, -1.85f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 6th sphere on 2nd column, silver *****
	// ambient material
	materialAmbient[0] = 0.19225; // r
	materialAmbient[1] = 0.19225; // g
	materialAmbient[2] = 0.19225; // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 0.50754; // r
	materialDiffuse[1] = 0.50754; // g
	materialDiffuse[2] = 0.50754; // b
	materialDiffuse[3] = 1.0f;    // a
	

	// specular material
	materialSpecular[0] = 0.508273; // r
	materialSpecular[1] = 0.508273; // g
	materialSpecular[2] = 0.508273; // b
	materialSpecular[3] = 1.0f;     // a
	

	// materialShininess
	materialShininess = 0.4 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(1.0f, -3.1f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 3rd column, black *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.50; // r
	materialSpecular[1] = 0.50; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-2.0f,3.15f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 2nd sphere on 3rd column, cyan *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.1;  // g
	materialAmbient[2] = 0.06; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.0;        // r
	materialDiffuse[1] = 0.50980392; // g
	materialDiffuse[2] = 0.50980392; // b
	materialDiffuse[3] = 1.0f;       // a
	

	// specular material
	materialSpecular[0] = 0.50196078; // r
	materialSpecular[1] = 0.50196078; // g
	materialSpecular[2] = 0.50196078; // b
	materialSpecular[3] = 1.0f;       // a
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-2.0f,1.9f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 3rd sphere on 2nd column, green *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.1;  // r
	materialDiffuse[1] = 0.35; // g
	materialDiffuse[2] = 0.1;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.45; // r
	materialSpecular[1] = 0.55; // g
	materialSpecular[2] = 0.45; // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-2.0f,0.65f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 4th sphere on 3rd column, red *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.0;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.6;  // g
	materialSpecular[2] = 0.6;  // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-2.0f, -0.6f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 5th sphere on 3rd column, white *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.55; // r
	materialDiffuse[1] = 0.55; // g
	materialDiffuse[2] = 0.55; // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.70; // r
	materialSpecular[1] = 0.70; // g
	materialSpecular[2] = 0.70; // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-2.0f, -1.85f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 6th sphere on 3rd column, yellow plastic *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.60; // r
	materialSpecular[1] = 0.60; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.25 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-2.0f, -3.1f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 4th column, black *****
	// ambient material
	materialAmbient[0] = 0.02; // r
	materialAmbient[1] = 0.02; // g
	materialAmbient[2] = 0.02; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.4;  // r
	materialSpecular[1] = 0.4;  // g
	materialSpecular[2] = 0.4;  // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-5.0f,3.15f, 10.0f);
	// scaleMatrix = scale(1.0f, 1.0f, 0.8f);
	modelMatrix = translationMatrix * scaleMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 2nd sphere on 4th column, cyan *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.05; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.7;  // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-5.0f,1.9f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 3rd sphere on 4th column, green *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-5.0f,0.65f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 4th sphere on 4th column, red *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.04; // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-5.0f, -0.6f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 5th sphere on 4th column, white *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.05; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.7;  // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-5.0f, -1.85f, 10.0f);
	modelMatrix = translationMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// *******************************************************

	// ***** 6th sphere on 4th column, yellow rubber *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a
	

	// materialShininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);
	glUniform1f(materialShininessUniform, materialShininess);

	// geometry
	translationMatrix = translate(-5.0f, -3.1f, 10.0f);
	// scaleMatrix = scale(1.0f, 1.0f, 0.8f);
	modelMatrix = translationMatrix * scaleMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// Unuse the shader program object
	glUseProgram(0);
}

- (void)myupdate
{
	// Code
	if(keyPressed == 1)
	{
	   angleForXRotation += 1.0f;
	   if(angleForXRotation >= 360.0f)
		  angleForXRotation -= 360.0f;
	   light.lightPosition = vec4(0.0f, 100.0f * cosf(angleForXRotation * M_PI / 180.0f), 100.0f * sinf(angleForXRotation * M_PI / 180.0f), 1.0f);
	}
	else if(keyPressed == 2)
	{
	   angleForYRotation += 1.0f;
	   if(angleForYRotation >= 360.0f)
		  angleForYRotation -= 360.0f;
	   light.lightPosition = vec4(100.0f * cosf(angleForYRotation * M_PI / 180.0f), 0.0f, 100.0f * sinf(angleForYRotation * M_PI / 180.0f), 1.0f);
	}
	else if(keyPressed == 3)
	{
	   angleForZRotation += 1.0f;
	   if(angleForZRotation >= 360.0f)
		  angleForZRotation -= 360.0f;
	   light.lightPosition = vec4(100.0f * cosf(angleForZRotation * M_PI / 180.0f), 100.0f * sinf(angleForZRotation * M_PI / 180.0f), 0.0f, 1.0f);
	}
	else
	{
	   light.lightPosition = vec4(0.0f, 0.0f, 10.0f, 1.0f);
	}
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
	if(keyPressed < 4)
	{
		keyPressed++;
		angleForXRotation = 0.0f;
		angleForYRotation = 0.0f;
		angleForZRotation = 0.0f;
	}
	else
	{
		keyPressed = 0;
	}
}

- (void)onDoubleTap:(UITapGestureRecognizer*)gr
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
