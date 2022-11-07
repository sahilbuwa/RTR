// Standard Headers
#include<stdio.h> // For standard IO
#include<stdlib.h> // exit() sathi
#include<memory.h> // For memset()

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

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
// Visual info is not uninitialized as we have not created it but matched its params....
Display *display = NULL; // 77 member struct
XVisualInfo *visualInfo = NULL; // 10 member struct - 1 member 'visual' which has 8 members. Haa ata apan pointer kelai mhanun . la -> madhe badalne
Colormap colormap;
Window window;
Bool fullscreen = False;
FILE *gpFile = NULL;
int retVal;

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
    SAB_ATTRIBUTE_TEXURE0
};

GLuint vao_cube;
GLuint vbo_cube_position;
GLuint mvpMatrixUniform;

GLfloat anglecube = 0.0f;
mat4 perspectiveProjectionMatrix;

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
    const GLchar *vertexShaderSourceCode = "#version 460 core" \
    "\n" \
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
                fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", log);
                free(log);
                uninitialize();
				exit(1);
            }
        }
    }

    // Fragment Shader
    const GLchar *fragmentShaderSourceCode = "#version 460 core" \
    "\n" \
    "out vec4 FragColor;" \
    "void main(void)" \
    "{" \
    "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);" \
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
    // Vao unbind
    glBindVertexArray(0);

    // Clear the screen using black color
    glClearColor(0.0f,0.0f,0.0f,1.0f);

	// Depth Related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
	perspectiveProjectionMatrix = mat4::identity();

    // Warmup Resize Call
    resize(WIN_WIDTH,WIN_HEIGHT);
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

    glViewport(0,0,(GLsizei)width,(GLsizei)height);
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void draw(void)
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
    mat4 modelViewMatrix = mat4::identity();
    mat4 modelViewProjectionMatrix = mat4::identity();
    translationMatrix = translate(0.0f, 0.0f, -5.0f);
    rotationMatrix_x = rotate(anglecube, 1.0f, 0.0f, 0.0f);
    rotationMatrix_y = rotate(anglecube, 0.0f, 1.0f, 0.0f);
    rotationMatrix_z = rotate(anglecube, 0.0f, 0.0f, 1.0f);
    rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
    modelViewMatrix = translationMatrix * rotationMatrix;

    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    
    glBindVertexArray(vao_cube);

    // Here there should be draw code (12 lakh)
    glDrawArrays(GL_TRIANGLE_FAN, 0, 24);

    glBindVertexArray(0);
    // Unuse the shader program object
    glUseProgram(0);

	glXSwapBuffers(display, window);
}

void update(void)
{
	// Code
    anglecube = anglecube + 0.1f;
    if(anglecube >= 360.0f)
        anglecube -= 360.0f;
}

void uninitialize(void)
{
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
