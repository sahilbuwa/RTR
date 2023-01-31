// Standard Headers
#include<stdio.h> // For standard IO
#include<stdlib.h> // exit() sathi
#include<memory.h> // For memset()
#include"Sphere.h" // Sphere sathi

// X11 Headers
#include<X11/Xlib.h> // windows.h saarkhi 
#include<X11/Xutil.h> // XVisualInfo() 
#include<X11/XKBlib.h>

//OpenGL Headers
#include<GL/glew.h> // PP Functionality
#include<GL/gl.h>  // For OpenGL functionality
#include<GL/glx.h> // Bridging APIs
#include"vmath.h" // Maths (RedBook)
using namespace vmath;

// Texture Library Header
#include<SOIL/SOIL.h> // Simple OpenGL Imaging Library

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define FBO_WIDTH 1024
#define FBO_HEIGHT 1024

// Global Variables
// Visual info is not uninitialized as we have not created it but matched its params....
Display *display = NULL; // 77 member struct
XVisualInfo *visualInfo = NULL; // 10 member struct - 1 member 'visual' which has 8 members. Haa ata apan pointer kelai mhanun . la -> madhe badalne
Colormap colormap;
Window window;
Bool fullscreen = False;
FILE *gpFile = NULL;
int retVal;
int winWidth;
int winHeight;

// OpenGL related variables
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig glxFBConfig;
GLXContext glxContext;
Bool bActiveWindow = False;

GLuint shaderProgramObject;

enum
{
	SAB_ATTRIBUTE_POSITION = 0,
	SAB_ATTRIBUTE_COLOR,
	SAB_ATTRIBUTE_NORMAL,
	SAB_ATTRIBUTE_TEXTURE0
};

GLuint vao_cube;
GLuint vbo_cube_position;
GLuint vbo_cube_texcoord;
GLuint mvpMatrixUniform;

GLuint textureSamplerUniform;

GLfloat angleCube = 0.0f;
mat4 perspectiveProjectionMatrix;

// FBO related global variables
GLuint fbo;
GLuint rbo;
GLuint fbo_texture;
bool bFBOResult = false;

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
unsigned short sphere_elements[2280];
GLuint numElements_sphere;
GLuint numVertices_sphere;

mat4 perspectiveProjectionMatrix_sphere;
// Lights Variables
bool bLight = false;

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

GLfloat materialAmbient_sphere[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialDiffuse_sphere[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialSpecular_sphere[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialShininess_sphere = 50.0f;

GLfloat redLightAngle_sphere = 0.0f,
		greenLightAngle_sphere = 0.0f,
		blueLightAngle_sphere = 0.0f;

// Entry-point function
int main(void)
{
	// Function Declarations
	void uninitialize(void);
	void toggleFullscreen(void);
	int initialize(void);
	void resize(int, int);
	void draw(void);
	void update(void);
	
	// Local Variables
	int defaultScreen;
	int defaultDepth;
	GLXFBConfig *glxFBConfigs = NULL;
	GLXFBConfig bestGLXFBConfig;
	XVisualInfo *tempXVisualInfo = NULL;
	int numFBConfigs = 0;
	XSetWindowAttributes windowAttributes;
	int styleMask;
	Atom wm_delete_window_atom;
	XEvent event; // Among 33 member structures 31 are structs and 2 unions
	KeySym keysym;
	int screenWidth;
	int screenHeight;
	char keys[26];
	static int frameBufferAttributes[] = 
	{
		GLX_DOUBLEBUFFER,True,
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_STENCIL_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		None
	};
	Bool bDone;
	static int winWidth, winHeight;
	

	// Code
	gpFile = fopen("Log.txt","w");
	if(gpFile == 0)
	{
		printf("Creation of log file failed. Exiting...\n");
		exit(0);
	}
	else
	{
		fprintf(gpFile,"Log File Is Successfully Created. \n");
	}	
	display = XOpenDisplay(NULL); // Availables the display , NULL can also be command line argument from int main() for networking
	if(display == NULL)
	{
		fprintf(gpFile,"ERROR:XOpenDisplay() failed.\n");
		uninitialize();
		exit(1);
	}
	defaultScreen = XDefaultScreen(display); // Primary monitor - VGA adapter available -> graphic card also available -> VRAM -> Frame buffer -> depth
	defaultDepth = XDefaultDepth(display, defaultScreen);
	glxFBConfigs = glXChooseFBConfig(display, defaultScreen, frameBufferAttributes, &numFBConfigs);
	if(glxFBConfigs == NULL)
	{
		fprintf(gpFile, "ERROR: glXChooseFBConfig() failed.\n");
		uninitialize();
		exit(1);
	}
	else
	{
		fprintf(gpFile, "Found FrameBuffer Configs are %d.\n", numFBConfigs);
	}
	// Find Best FrameBufferConfig from the array
	int bestFrameBufferConfig = -1;
	int worstFrameBufferConfig = -1;
	int bestNumberOfSamples = -1;
	int worseNumberOfSamples = 999;
	for(int i = 0; i < numFBConfigs; i++)
	{
		tempXVisualInfo = glXGetVisualFromFBConfig(display, glxFBConfigs[i]);
		if(tempXVisualInfo != NULL)
		{
			int samples, sampleBuffers;
			glXGetFBConfigAttrib(display, glxFBConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
			glXGetFBConfigAttrib(display, glxFBConfigs[i], GLX_SAMPLES, &samples);
			fprintf(gpFile, "visualInfo = 0x%lu found sample buffers = %d and samples = %d.\n", tempXVisualInfo->visualid, sampleBuffers, samples);
			if(bestFrameBufferConfig < 0 || sampleBuffers && samples > bestNumberOfSamples)
			{
				bestFrameBufferConfig = i;
				bestNumberOfSamples = samples;
			}
			if(worstFrameBufferConfig < 0 || !sampleBuffers || samples < worseNumberOfSamples)
			{
				worstFrameBufferConfig = i;
				worseNumberOfSamples = samples;
			}
		}
		XFree(tempXVisualInfo);
		tempXVisualInfo = NULL;
	}
	
	bestGLXFBConfig = glxFBConfigs[bestFrameBufferConfig];
	glxFBConfig = bestGLXFBConfig;
	XFree(glxFBConfigs);
	glxFBConfigs = NULL;
	visualInfo = glXGetVisualFromFBConfig(display, bestGLXFBConfig);
	fprintf(gpFile, "Visual ID of bestVisualInfo is 0x%lu \n", visualInfo->visualid);

	memset(&windowAttributes, 0, sizeof(XSetWindowAttributes));
	windowAttributes.border_pixel = 0;
	windowAttributes.background_pixel = XBlackPixel(display, defaultScreen); // BLACK_BRUSH similar
	windowAttributes.background_pixmap = 0;
	windowAttributes.colormap = XCreateColormap(display, 
												RootWindow(display, visualInfo->screen), // DefaultColorScreen pn Chalel ,MoolPurush Window
												visualInfo->visual,
												AllocNone); // 2^32 colors astat color map madhe , smallest part is one color->colorcell->contains RGB
	windowAttributes.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | FocusChangeMask;
	colormap = windowAttributes.colormap;
	styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;
	window = XCreateWindow(display, RootWindow(display, visualInfo->screen), 0, 0, WIN_WIDTH, WIN_HEIGHT, 0, visualInfo->depth, InputOutput, visualInfo->visual, styleMask, &windowAttributes);
	if(!window)
	{
		fprintf(gpFile, "ERROR:XCreateWindow() failed.\n");
		uninitialize();
		exit(1);
	}
	XStoreName(display, window, "SAB:OpenGL"); // Name the created window
	wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", True); // WM - window manager
	XSetWMProtocols(display, window, &wm_delete_window_atom, 1);
	XMapWindow(display, window); //Similar to ShowWindow() 
	
	// Centering of window
	screenWidth = XWidthOfScreen(XScreenOfDisplay(display, defaultScreen));
	screenHeight = XHeightOfScreen(XScreenOfDisplay(display, defaultScreen));
	XMoveWindow(display, window, (screenWidth-WIN_WIDTH)/2, (screenHeight-WIN_HEIGHT)/2);

	// Initialize Call
	retVal = initialize();
	if(retVal == -1)
	{
		uninitialize();
		exit(1);
	}
	else if(retVal == -2)
	{
		fprintf(gpFile, "glewInit gandle.\n");
	}
	else
	{
		fprintf(gpFile, "OpenGL Context Created Successfully.\n");
	}

	// Message Loop
	
	while(bDone == False)
	{
		while(XPending(display)) // Same PeekMessage()
		{
			XNextEvent(display, &event); // GetMessage() similar 
			switch(event.type)
			{
				case MapNotify: // WM_CREATE
					break;
				case FocusIn: // WM_SETFOCUS
					bActiveWindow = True;
					break;
				case FocusOut:
					bActiveWindow = False;
					break;
				case KeyPress:
					keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);			
					switch(keysym)
					{
						case XK_Escape:
							bDone = True;
							break;
					}
					XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL); // struct XComposeStatus (struct) , if keys are pressed repetatively then this struct is used... , WM_CHAR similar
					switch(keys[0])
					{
						case 'f':
						case 'F':
							if(fullscreen == False)
							{	
								toggleFullscreen();
								fullscreen = True;
							}	
							else
							{
								toggleFullscreen();
								fullscreen = False;
							}
							break;
						case 'L':
						case 'l':
							if (bLight == false)
							{
								bLight = true;
							}
							else
							{
								bLight = false;
							}
							break;
					}
					break;
				case ConfigureNotify:
					winWidth = event.xconfigure.width;
					winHeight = event.xconfigure.height;
					resize(winWidth, winHeight);
					break;
				case 33:
					bDone = True;
					break;
			}
		}
		if(bActiveWindow == True)
		{
			update();
			draw();
		}
	}
	
	uninitialize();
	return 0;
}

void toggleFullscreen(void)
{
	// Local Variables Declarations
	Atom wm_current_state_atom;
	Atom wm_fullscreen_state_atom;
	XEvent event;

	// Code
	wm_current_state_atom = XInternAtom(display, "_NET_WM_STATE", False);
	wm_fullscreen_state_atom = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

	memset(&event, 0, sizeof(XEvent));
	event.type = ClientMessage;
	event.xclient.window = window;
	event.xclient.message_type = wm_current_state_atom;
	event.xclient.format = 32;
	event.xclient.data.l[0] = fullscreen ? 0 : 1;
	event.xclient.data.l[1] = wm_fullscreen_state_atom;

	XSendEvent(display,
				RootWindow(display, visualInfo->screen),
				False,
				SubstructureNotifyMask,
				&event
				 );
}

int initialize(void)
{
	// Function Declarations
	void resize(int, int);
	void uninitialize(void);
	void printGLInfo(void);
	bool createFBO(GLint, GLint);
	int initialize_sphere(int, int);

	// Code
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");
	if(glXCreateContextAttribsARB == NULL)
	{
		fprintf(gpFile, "glXGetProcAddressARB() failed.\n");
		uninitialize();
		exit(1);
	}
	GLint contextAttributes[] = 
	{
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
		GLX_CONTEXT_MINOR_VERSION_ARB, 6,
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None
	};
	glxContext = glXCreateContextAttribsARB(display, glxFBConfig, NULL, True, contextAttributes);
	if(!glxContext)
	{
		GLint contextAttributes[] = 
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
			GLX_CONTEXT_MINOR_VERSION_ARB, 0,
			None
		};
		// Fallback madhle topmost de , 1.0 ch milel asa nai.
		glxContext = glXCreateContextAttribsARB(display, glxFBConfig, NULL, True, contextAttributes);
		fprintf(gpFile, "Cannot support 4.6, hence falling back to default version.\n");

	}
	else
	{
		fprintf(gpFile, "Found the support to 4.6 version.\n");
	}
	// Checking whether direct rendering or software rendering we are getting
	if(!glXIsDirect(display, glxContext))
	{
		fprintf(gpFile, "Direct Rendering i.e. hardware rendering not supported.\n");
	}
	else
	{
		fprintf(gpFile, "Direct Rendering i.e. hardware rendering is supported.\n");
	}
	glXMakeCurrent(display, window, glxContext);
	// Here starts OpenGL Functionality
	// GLEW initialization
	if(glewInit() != GLEW_OK)
		return -2;
	// Print OpenGL Info
	printGLInfo();

	// Vertex Shader
	// Shader Source Code
	const GLchar *vertexShaderSourceCode = 
	"#version 460 core" \
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
				uninitialize();
				exit(1);
			}
		}
	}

	// Fragment Shader
	const GLchar *fragmentShaderSourceCode = 
	"#version 460 core" \
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
				uninitialize();
				exit(1);
			}
		}
	}

	// Shader Program Object
	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);
	glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_POSITION, "a_position"); // Andhaar
	glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_TEXTURE0, "a_texcoord");  // Andhaar
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
				uninitialize();
				exit(1);
			}
		}
	}

	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

	// Declaration of vertex data array
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
	// Vbo for color

	glGenBuffers(1, &vbo_cube_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexcoords), cubeTexcoords, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Vao unbind
	glBindVertexArray(0);

	// Clear the screen using white color
	glClearColor(1.0f,1.0f,1.0f,1.0f);

	// Depth Related Changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Enabling the texture
	glEnable(GL_TEXTURE_2D);
	
	perspectiveProjectionMatrix = mat4::identity();

	// Warmup Resize Call
	resize(WIN_WIDTH,WIN_HEIGHT);
		// FBO initialize code
	bFBOResult = createFBO(FBO_WIDTH, FBO_HEIGHT);
	int iRetval;
	if(bFBOResult == true)
	{
		iRetval = initialize_sphere(FBO_WIDTH, FBO_HEIGHT);
		// Error Checking
	}
	else
	{
		fprintf(gpFile, "Create FBO failed.\n");
		return -6;
	}
	return 0;
}

bool createFBO(GLint textureWidth, GLint textureHeight)
{
	// Local Variable Declarations
	int maxRenderbufferSize = 0;
	// Code
	// Step 1: Check available renderbuffer size.
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
	if(maxRenderbufferSize < textureWidth || maxRenderbufferSize < textureHeight)
	{
		fprintf(gpFile, "Insufficient renderbuffer size.\n");
		return false;
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
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Texture and rbo will be detached automatically.
	return true;
}

int initialize_sphere(int width, int height)
{
	// Function Declarations
	void resize_sphere(int, int);
	void uninitialize_sphere(void);
	// Variable Declarations

	// Per fragment
	// Vertex Shader
	// Shader Source Code
	const GLchar *vertexShaderSourceCodeF = 
	"#version 460 core" \
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
				uninitialize_sphere();
			}
		}
	}

	// Fragment Shader
	const GLchar *fragmentShaderSourceCodeF = 
	"#version 460 core" \
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
	"vec4 fong_ads_light;" \
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
				uninitialize_sphere();
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
				uninitialize_sphere();
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

	perspectiveProjectionMatrix_sphere = mat4::identity();
	// Warmup Resize Call
	resize_sphere(FBO_WIDTH, FBO_HEIGHT);
	return 0;
}

void printGLInfo(void)
{
	// Local Variable Declarations
	GLint numExtensions = 0;

	// Code
	fprintf(gpFile, "OpenGL Vendor : %s\n",glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n",glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version : %s\n",glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version : %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION)); // Graphic Library Shading Language

	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	
	fprintf(gpFile, "Number of supported extensions : %d\n", numExtensions);
	for(int i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}
}

void resize(int width, int height)
{
	// Code
	if(height==0)
		height=1; // To avoid divided by 0 error(illegal statement) in future calls..

	winWidth = width;
	winHeight = height;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void resize_sphere(int width, int height)
{
	// Code
	if(height == 0)
		height = 1; // To avoid divided by 0 error(illegal statement) in future calls..

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	
	perspectiveProjectionMatrix_sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void draw(void)
{
	// Function declarations
	void draw_sphere(GLint, GLint);
	void update_sphere(void);
	// Code
	if(bFBOResult == true)
	{
		draw_sphere(FBO_WIDTH, FBO_HEIGHT);
		update_sphere();
	}
	// Screen background white kar
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	resize(winWidth, winHeight);
	
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

	glXSwapBuffers(display, window);
}

void update(void)
{
	// Code
	angleCube = angleCube + 0.1f;
	if(angleCube >= 360.0f)
		angleCube -= 360.0f;
}

void draw_sphere(GLint textureWidth, GLint textureHeight)
{
	// Code
	// Bind framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// FBO background black kar
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	resize_sphere(textureWidth, textureHeight);

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
		
	if(bLight == true)
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

void update_sphere(void)
{
	// Code
	redLightAngle_sphere += 0.2f;
	if(redLightAngle_sphere >= 360.0f)
		redLightAngle_sphere -= 360.0f;
	lights[0].lightPosition = vec4(0.0f, 10.0f * cosf(redLightAngle_sphere * M_PI / 180.0f), 10.0f * sinf(redLightAngle_sphere * M_PI / 180.0f), 1.0f);

	greenLightAngle_sphere += 0.2f;
	if(greenLightAngle_sphere >= 360.0f)
		greenLightAngle_sphere -= 360.0f;
	lights[1].lightPosition = vec4(10.0f * cosf(greenLightAngle_sphere * M_PI / 180.0f), 0.0f, 10.0f * sinf(greenLightAngle_sphere * M_PI / 180.0f), 1.0f);

	blueLightAngle_sphere += 0.2f;
	if(blueLightAngle_sphere >= 360.0f)
		blueLightAngle_sphere -= 360.0f;
	lights[2].lightPosition = vec4(10.0f * cosf(blueLightAngle_sphere * M_PI / 180.0f), 10.0f * sinf(blueLightAngle_sphere * M_PI / 180.0f), 0.0f, 1.0f);
}

void uninitialize(void)
{
	// Function Declarations
	void uninitialize_sphere(void);
	// Code
	GLXContext currentContext;
	currentContext = glXGetCurrentContext();
	if(currentContext && currentContext == glxContext)
	{
		glXMakeCurrent(display, 0, 0);
	}
	if(glxContext)
	{
		glXDestroyContext(display, glxContext);
		glxContext = NULL;
	}
	if(visualInfo)
	{
		free(visualInfo);
		visualInfo = NULL;
	}
	uninitialize_sphere();
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
	if(window)
	{
		XDestroyWindow(display, window);
	}
	if(colormap)
	{
		XFreeColormap(display, colormap);
	}
	if(display)
	{
		XCloseDisplay(display);
		display = NULL;
	}
	if(gpFile)
	{
		fprintf(gpFile,"Log File Is Successfully Closed.\n");
		fclose(gpFile);
		gpFile=NULL;
	}
}

void uninitialize_sphere(void)
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
	if(shaderProgramObject_sphere)
	{
		glUseProgram(shaderProgramObject_sphere);
		GLsizei numAttachedShaders;
		glGetProgramiv(shaderProgramObject_sphere, GL_ATTACHED_SHADERS, &numAttachedShaders);
		GLuint *shaderObjects;
		shaderObjects = (GLuint *)malloc(numAttachedShaders*sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject_sphere, numAttachedShaders, &numAttachedShaders, shaderObjects);
		for(GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject_sphere, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}
		free(shaderObjects);
		shaderObjects = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject_sphere);
		shaderProgramObject_sphere = 0;
	}
}
