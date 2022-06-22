// Standard Headers
#include<stdio.h> // For standard IO
#include<stdlib.h> // exit() sathi
#include<memory.h> // For memset()

// X11 Headers
#include<X11/Xlib.h> // windows.h saarkhi 
#include<X11/Xutil.h> // XVisualInfo() 
#include<X11/XKBlib.h>

//OpenGL Headers
#include<GL/gl.h>  // For OpenGL functionality
#include<GL/glx.h> // Bridging APIs
#include<GL/glu.h> // OpenGL utility

// Texture Library Header
//#include<SOIL/SOIL.h> // Simple OpenGL Imaging Library // No need here as we are making our own texture.

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define Checkerboard_Width 64
#define Checkerboard_Height 64


// Global Variables
// Visual info is not uninitialized as we have not created it but matched its params.
Display *display = NULL; // 77 member struct
XVisualInfo *visualInfo = NULL; // 10 member struct - 1 member 'visual' which has 8 members. Haa ata apan pointer kelai mhanun . la -> madhe badalne
Colormap colormap;
Window window;
Bool fullscreen = False;
// OpenGL related variables
GLXContext glxContext;
Bool bActiveWindow = False;
FILE *gpFile=NULL;
int gKeypressed = -1;
GLubyte checkerboard[Checkerboard_Width][Checkerboard_Height][4];
GLuint texture_checkerboard;

// Entry-point function
int main(void)
{
	// Function Declarations
	void uninitialize(void);
	void toggleFullscreen(void);
	int initialize(void);
    void resize(int, int);
    void draw(void);
	
	// Local Variables
	int defaultScreen;
	int defaultDepth;
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
		GLX_RGBA,
		GLX_RED_SIZE,8,
		GLX_GREEN_SIZE,8,
		GLX_BLUE_SIZE,8,
		GLX_ALPHA_SIZE,8,
		GLX_DEPTH_SIZE,24,
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
		printf("ERROR:XOpenDisplay() failed.\n");
		uninitialize();
		exit(1);
	}
	defaultScreen = XDefaultScreen(display); // Primary monitor - VGA adapter available -> graphic card also available -> VRAM -> Frame buffer -> depth
	defaultDepth = XDefaultDepth(display, defaultScreen);
	visualInfo = glXChooseVisual(display, defaultScreen, frameBufferAttributes); // Major Change - changing to opengl for graphics
	if(visualInfo == NULL)
	{
		printf("ERROR:glxChooseVisual() failed.\n");
		uninitialize();
		exit(1);
	}	
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
		printf("ERROR:XCreateWindow() failed.\n");
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
	initialize();
	
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
			//update();
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
	// Declaration of user-defined functions
    Bool loadGLTexture(void);
    // Function Declarations
    void resize(int,int);
    void uninitialize(void);
    
	// Code
	glxContext = glXCreateContext(display, visualInfo, NULL, True); // Sharing of existing glxContext in 3rd param for other gpus
	glXMakeCurrent(display, window, glxContext);

    // Here Starts OpenGL code
    // Clear the screen using black color
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    // Depth Related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    
    // Enabling the texture
    glEnable(GL_TEXTURE_2D);
    loadGLTexture();

    // Warmup Resize Call
    resize(WIN_WIDTH,WIN_HEIGHT);
    return 0;

}

void resize(int width, int height)
{
    // Code
    if(height==0)
        height=1; // To avoid divided by 0 error(illegal statement) in future calls..

    glViewport(0,0,(GLsizei)width,(GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

}

void draw(void)
{
	// Variable Declarations
    
    // Code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(0.0f,0.0f,-4.0f);
    glBindTexture(GL_TEXTURE_2D, texture_checkerboard);

    // Checkerboard che ujwikadche 2 points XY plane varun 45 degrees -Z axis kade rotate kele mhanun he x ani z chya navya values 2.41421f and -1.41421f alyat....
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.41421f, 1.0f, -1.41421f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.41421f, -1.0f, -1.41421f);
	glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
	
	glXSwapBuffers(display, window);
}

Bool loadGLTexture(void)
{
	// Function Declarations
    void MakeCheckerBoard(void);
	// Variable Declarations
	int width, height;
	unsigned char* imageData = NULL;

    // Code
    MakeCheckerBoard();


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture_checkerboard);
    glBindTexture(GL_TEXTURE_2D, texture_checkerboard);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Checkerboard_Width, Checkerboard_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, 0);
    return True;
}

void MakeCheckerBoard(void)
{
    // Variable Declarations
    int c;

    // Code
    for(int i=0; i < Checkerboard_Width; i++)
    {
        for(int j=0; j < Checkerboard_Height; j++)
        {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0))*255;

            checkerboard[i][j][0] = (GLubyte)c;
            checkerboard[i][j][1] = (GLubyte)c;
            checkerboard[i][j][2] = (GLubyte)c;
            checkerboard[i][j][3] = (GLubyte)255;
        }
    }
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
	if(window)
	{
		XDestroyWindow(display, window);
	}
	if(texture_checkerboard)
    {
        glDeleteTextures(1, &texture_checkerboard);
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

