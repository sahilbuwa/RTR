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

mat4 perspectiveProjectionMatrix;

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
	GLuint vao_square;
	GLuint vbo_quad;
	GLuint vao_circle;
	GLuint vbo_circle_position;
	GLuint vao_triangle;
	GLuint vbo_triangle_position;
	GLuint vao_point;
	GLuint vbo_point_position;
	GLuint vao_graphs;
	GLuint vao_axes;
	GLuint vbo_graph_lines;
	GLuint vbo_axes_lines;

	GLuint mvpMatrixUniform;
	GLuint colorUniform;
	GLfloat circleVertices[1080];
	GLfloat graphLineVertices[480];
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
	"uniform mat4 u_mvpMatrix;" \
	"void main(void)" \
	"{" \
	"gl_Position = u_mvpMatrix * a_position;" \
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
	"uniform vec3 u_color;" \
	"out vec4 FragColor;" \
	"void main(void)" \
	"{" \
	"FragColor = vec4(u_color, 1.0);" \
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
	colorUniform = glGetUniformLocation(shaderProgramObject, "u_color");

	// Declaration of vertex data arrays
	const GLfloat squareVertices[] =
	{
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f , 0.0f
	};
	const GLfloat triangleVertices[] =
	{
		cosf(M_PI_2), sinf(M_PI_2), 0.0f,
		cosf(-(M_PI/6.0f)), sinf(-(M_PI/6.0f)) , 0.0f,
		cosf((7.0f*M_PI)/6.0f), sinf((7.0f*M_PI)/6.0f), 0.0f
	};
	GLfloat angle;
	for(int i = 0; i < 360; i++)
	{
		angle= i * M_PI / 180.0f;
		circleVertices[i * 3] = cosf(angle);
		circleVertices[i * 3 + 1] = sinf(angle);
		circleVertices[i * 3 + 2] = 0.0f;
	}
	int index =0;
	for(float i = -1.25f;i <= 1.25f; i += 0.0625f)
	{
		if(i==0.0f)
			continue;
		graphLineVertices[index * 3] = 1.25f;
		graphLineVertices[index * 3 + 1] = i;
		graphLineVertices[index * 3 + 2] = 0.0f;
		index++;
		graphLineVertices[index * 3] = -1.25f;
		graphLineVertices[index * 3 + 1] = i;
		graphLineVertices[index * 3 + 2] = 0.0f;
		index++;
		graphLineVertices[index * 3] = i;
		graphLineVertices[index * 3 + 1] = 1.25f;
		graphLineVertices[index * 3 + 2] = 0.0f;
		index++;
		graphLineVertices[index * 3] = i;
		graphLineVertices[index * 3 + 1] = -1.25f;
		graphLineVertices[index * 3 + 2] = 0.0f;
		index++;
	}
	const GLfloat axesVertices[] =
	{
		0.0f, 1.25f, 0.0f,
		0.0f, -1.25f, 0.0f,
		1.25f, 0.0f, 0.0f,
		-1.25f, 0.0f, 0.0f
	};
	const GLfloat pointVertex[] =
	{
		0.0f, 0.0f, 0.0f
	};

	// Vao and vbo_quad related code
	glGenVertexArrays(1, &vao_square);
	glBindVertexArray(vao_square);
	glGenBuffers(1, &vbo_quad);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// VBO for graph lines
	glGenVertexArrays(1, &vao_graphs);
	glBindVertexArray(vao_graphs);
	glGenBuffers(1, &vbo_graph_lines);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_graph_lines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(graphLineVertices), graphLineVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_axes);
	glBindVertexArray(vao_axes);
	glGenBuffers(1, &vbo_axes_lines);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes_lines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices), axesVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_circle);
	glBindVertexArray(vao_circle);
	glGenBuffers(1, &vbo_circle_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_circle_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_triangle);
	glBindVertexArray(vao_triangle);
	glGenBuffers(1, &vbo_triangle_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_point);
	glBindVertexArray(vao_point);
	glGenBuffers(1, &vbo_point_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_point_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertex), pointVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
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
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	translationMatrix = translate(0.0f, 0.0f, -3.25f);
	modelViewMatrix = translationMatrix;
	
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	
	// Graph Lines
	glLineWidth(1.0f);
	glUniform3f(colorUniform, 0.0f, 0.0f, 1.0f);
	glBindVertexArray(vao_graphs);

	glDrawArrays(GL_LINES, 0, 320);

	glBindVertexArray(0);
	// Axes
	glLineWidth(2.0f);
	glUniform3f(colorUniform, 0.0f, 1.0f, 0.0f);
	glBindVertexArray(vao_axes);

	glDrawArrays(GL_LINES, 0, 2);

	glUniform3f(colorUniform, 1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_LINES, 2, 2);

	glBindVertexArray(0);

	// Square
	glUniform3f(colorUniform, 1.0f, 1.0f, 0.0f);
	glBindVertexArray(vao_square);

	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glBindVertexArray(0);

	// Triangle
	glBindVertexArray(vao_triangle);

	glDrawArrays(GL_LINE_LOOP, 0, 3);

	glBindVertexArray(0);

	// Circle
	glBindVertexArray(vao_circle);

	glDrawArrays(GL_LINE_LOOP, 0, 360);

	glBindVertexArray(0);

	// Point
	glBindVertexArray(vao_point);

	glDrawArrays(GL_POINTS, 0, 1);

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
	if(vbo_quad)
	{
		glDeleteBuffers(1, &vbo_quad);
		vbo_quad = 0;
	}
	if(vbo_axes_lines)
	{
		glDeleteBuffers(1, &vbo_axes_lines);
		vbo_axes_lines = 0;
	}
	if(vbo_circle_position)
	{
		glDeleteBuffers(1, &vbo_circle_position);
		vbo_circle_position = 0;
	}
	if(vbo_point_position)
	{
		glDeleteBuffers(1, &vbo_point_position);
		vbo_point_position = 0;
	}
	if(vbo_triangle_position)
	{
		glDeleteBuffers(1, &vbo_triangle_position);
		vbo_triangle_position = 0;
	}
	if(vbo_quad)
	{
		glDeleteBuffers(1, &vbo_quad);
		vbo_quad = 0;
	}
	// Deletion and uninitialization of vao
	if(vao_square)
	{
		glDeleteVertexArrays(1, &vao_square);
		vao_square = 0;
	}
	if(vao_graphs)
	{
		glDeleteVertexArrays(1, &vao_graphs);
		vao_graphs = 0;
	}
	if(vao_axes)
	{
		glDeleteVertexArrays(1, &vao_axes);
		vao_axes = 0;
	}
	if(vao_triangle)
	{
		glDeleteVertexArrays(1, &vao_triangle);
		vao_triangle = 0;
	}
	if(vao_point)
	{
		glDeleteVertexArrays(1, &vao_point);
		vao_point = 0;
	}
	if(vao_triangle)
	{
		glDeleteVertexArrays(1, &vao_triangle);
		vao_triangle = 0;
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
