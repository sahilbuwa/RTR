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
using namespace vmath;

enum
{
	SAB_ATTRIBUTE_POSITION = 0,
	SAB_ATTRIBUTE_COLOR,
	SAB_ATTRIBUTE_NORMAL,
	SAB_ATTRIBUTE_TEXTURE0
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
	GLuint vao_pyramid;
	GLuint vbo_pyramid_position;
	GLuint vbo_pyramid_texcoord;
	GLuint vao_cube;
	GLuint vbo_cube_position;
	GLuint vbo_cube_texcoord;
	GLuint mvpMatrixUniform;

	GLuint textureSamplerUniform;

	GLuint texture_stone;
	GLuint texture_kundali;

	GLfloat anglepyramid;
	GLfloat anglecube;
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
	// Code
	// Vertex Shader
	// Shader Source Code
	const GLchar *vertexShaderSourceCode =
	"#version 300 es" \
	"\n" \
	"precision highp float;" \
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
	glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_TEXTURE0, "a_texcoord");  
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
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

	// Declaration of vertex data arrays
	const GLfloat pyramidPosition[] =
	{
		// front
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		// right
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		// back
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		// left
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f

	};

	const GLfloat pyramidTexcoords[] =
	{
		0.5, 1.0, // front-top
		0.0, 0.0, // front-left
		1.0, 0.0, // front-right

		0.5, 1.0, // right-top
		1.0, 0.0, // right-left
		0.0, 0.0, // right-right

		0.5, 1.0, // back-top
		1.0, 0.0, // back-left
		0.0, 0.0, // back-right

		0.5, 1.0, // left-top
		0.0, 0.0, // left-left
		1.0, 0.0 // left-right
	};

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

	// pyramid
	// Vao related code
	glGenVertexArrays(1, &vao_pyramid);
	glBindVertexArray(vao_pyramid);
	// Vbo for position
	glGenBuffers(1, &vbo_pyramid_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPosition), pyramidPosition, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Vbo for color
	glGenBuffers(1, &vbo_pyramid_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid_texcoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidTexcoords), pyramidTexcoords, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_TEXTURE0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Vao unbind
	glBindVertexArray(0);

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
	// Vbo for color

	glGenBuffers(1, &vbo_cube_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexcoords), cubeTexcoords, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Vao unbind
	glBindVertexArray(0);
	
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	texture_stone = [self LoadGLTexture:@"Stone" :@"bmp"];
	if(texture_stone == 0)
	{
		printf("initialize:LoadGLTexture Stone failed.\n");
		[self uninitialize];
		[self release];
		exit(0);
	}
	
	texture_kundali = [self LoadGLTexture:@"Kundali" :@"bmp"];
	if(texture_kundali == 0)
	{
		printf("initialize:LoadGLTexture Kundali failed.\n");
		[self uninitialize];
		[self release];
		exit(0);
	}
	
	perspectiveProjectionMatrix = mat4::identity();
	return 0;
}

- (GLuint)LoadGLTexture:(NSString*)textureFileName :(NSString*)extension
{
	// Code
	NSBundle *appBundle = [NSBundle mainBundle];
	NSString* textureFileNameWithExtension = [appBundle pathForResource:textureFileName ofType:extension];
	// GetNSImage representation of texture file
	UIImage *uiImage = [[UIImage alloc]initWithContentsOfFile:textureFileNameWithExtension];
	if(uiImage == nil)
	{
		printf("uiImage failed");
		[self uninitialize];
		[self release];
		return 0;
	}
	// From above NSImage representation obtain CGImage representation of texture file
	CGImageRef cgImage = [uiImage CGImage];
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
	// pyramid
	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	translationMatrix = translate(-1.5f, 0.0f, -6.0f);
	rotationMatrix = rotate(anglepyramid, 0.0f, 1.0f, 0.0f);
	modelViewMatrix = translationMatrix * rotationMatrix;   // Order is very important. (Matrix multiplication is not commutative.)

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_stone);
	glUniform1i(textureSamplerUniform, 0);

	glBindVertexArray(vao_pyramid);

	// Here there should be draw code (12 lakh)
	glDrawArrays(GL_TRIANGLES, 0, 12);

	glBindVertexArray(0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	// cube
	// Transformations
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	modelViewMatrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();
	translationMatrix = translate(1.5f, 0.0f, -6.0f);
	rotationMatrix_x = rotate(anglepyramid, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = rotate(anglepyramid, 0.0f, 1.0f, 0.0f);
	rotationMatrix_z = rotate(anglepyramid, 0.0f, 0.0f, 1.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	scaleMatrix = scale(0.75f, 0.75f, 0.75f);
	modelViewMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_kundali);
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
	anglepyramid = anglepyramid + 1.0f;
	if(anglepyramid >= 360.0f)
		anglepyramid -= 360.0f;
		
	anglecube = anglecube + 1.0f;
	if(anglecube >= 360.0f)
		anglecube -= 360.0f;
}

- (void)uninitialize
{
	// Code
	if(texture_stone)
	{
		glDeleteTextures(1, &texture_stone);
		texture_stone = 0;
	}
	if(texture_kundali)
	{
		glDeleteTextures(1, &texture_kundali);
		texture_kundali = 0;
	}
	if(vbo_cube_position)
	{
		glDeleteBuffers(1, &vbo_cube_position);
		vbo_pyramid_position = 0;
	}
	// Deletion and uninitialization of vbo_position
	if(vbo_pyramid_position)
	{
		glDeleteBuffers(1, &vbo_pyramid_position);
		vbo_pyramid_position = 0;
	}
	// Deletion and uninitialization of vao_cube
	if(vao_cube)
	{
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}
	// Deletion and uninitialization of vao_pyramid
	if(vao_pyramid)
	{
		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid = 0;
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
}

- (void)onDoubleTap:(UITapGestureRecognizer*)gr
{
    // Code
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
