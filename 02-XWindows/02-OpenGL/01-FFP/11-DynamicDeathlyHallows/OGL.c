// Standard Headers
#include<stdio.h> // For standard IO
#include<stdlib.h> // exit() sathi
#include<memory.h> // For memset()
#define _USE_MATH_DEFINES
#include<math.h> // cos() , sin() sathi

// X11 Headers
#include<X11/Xlib.h> // windows.h saarkhi 
#include<X11/Xutil.h> // XVisualInfo() 
#include<X11/XKBlib.h>

//OpenGL Headers
#include<GL/gl.h>  // For OpenGL functionality
#include<GL/glx.h> // Bridging APIs
#include<GL/glu.h> // GL utilities

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
// OpenGL related variables
GLXContext glxContext;
Bool bActiveWindow = False;
float trianglex = -3.5f , triangley = -3.5f;
float circlex = 3.5f , circley = -3.5f;
float wandy = 3.5f;
float spinAngleTriangle = 0.0f , spinAngleCircle = 0.0f;
int triangleSpinCount = 0, circleSpinCount = 0; 

// Entry-point function
int main(void)
{
	// Function Declarations
	void uninitialize(void);
	void toggleFullscreen(void);
	void initialize(void);
    void resize(int, int);
    void draw(void);
	void update(void);
	
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

void initialize(void)
{
	// Function Declarations
    void resize(int,int);
	// Code
	glxContext = glXCreateContext(display, visualInfo, NULL, True); // Sharing of existing glxContext in 3rd param for other gpus
	glXMakeCurrent(display, window, glxContext);

	// Here starts OpenGL Functionality
	// Clearing the screen by Black Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Depth Related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	// Warmup Resize Call
    resize(WIN_WIDTH,WIN_HEIGHT);

}

void resize(int width, int height)
{
	if(height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}

void draw(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glEnable(GL_LINE_SMOOTH);
    // Invisibility Cloak
    glTranslatef(trianglex,triangley,-6.0f);
    glRotatef(spinAngleTriangle, 0.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.75f, 0.75f, 0.75f);
    glVertex3f(cos(M_PI_2), sin(M_PI_2), 0.0f);
    glVertex3f(cos(-(M_PI/6.0f)), sin(-(M_PI/6.0f)) , 0.0f);
    glVertex3f(cos((7.0f*M_PI)/6.0f), sin((7.0f*M_PI)/6.0f), 0.0f);
	glEnd();

    // Elder Wand
    glLoadIdentity();
    glTranslatef(0.0f,wandy,-6.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(0.625f, 0.32f, 0.15f);
    glVertex3f(cos(M_PI_2), sin(M_PI_2)-0.05f, 0.0f);
    glVertex3f(0.0f, sin((7.0f*M_PI)/6.0f), 0.0f); 
    glEnd();

    // Philosopher's Stone
    glLoadIdentity();
    glTranslatef(circlex,circley,-6.0f);
    glRotatef(spinAngleCircle, 0.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 0.0f, 0.0f);
    for(int i=0;i<360;i++)
    {
        float angle= i*M_PI / 180;
        glVertex3f(0.5f*cos(angle),0.5f*sin(angle), 0.0f);
    }
    glEnd();
	glDisable(GL_LINE_SMOOTH);

	glXSwapBuffers(display, window);
}

void update(void)
{
    // Code
	if(trianglex <= 0.0f && triangley <= 0.0f)
    {    
        trianglex = trianglex + 0.001f;
        triangley = triangley + 0.001f;
        spinAngleTriangle += 0.5f;
        if(spinAngleTriangle>=360.0f)
            spinAngleTriangle = spinAngleTriangle - 360.0f;
    }
    else if(trianglex >= 0.0f && triangley >= 0.0f && triangleSpinCount <= 4)
    {
        spinAngleTriangle += 0.5f;
        if(spinAngleTriangle>=360.0f)
        {
            spinAngleTriangle = spinAngleTriangle - 360.0f;
            triangleSpinCount += 1;
        }
    }
    else if(triangleSpinCount>4)
    {   
        if(circlex >= 0.0f && circley <= 0.0f)
        {
            circlex = circlex - 0.001f;
            circley = circley + 0.001f;
            spinAngleTriangle += 0.5f;
            spinAngleCircle += 0.5f;
            if(spinAngleTriangle>=360.0f)
                spinAngleTriangle = spinAngleTriangle - 360.0f;
            if(spinAngleCircle>=360.0f)
                spinAngleCircle = spinAngleCircle - 360.0f;
        }
        else if(circlex <= 0.0f && circley >= 0.0f && circleSpinCount <= 4)
        {
            spinAngleTriangle += 0.5f;
            if(spinAngleTriangle>=360.0f)
                spinAngleTriangle = spinAngleTriangle - 360.0f;
            spinAngleCircle += 0.5f;
            if(spinAngleCircle>=360.0f)
            {
                spinAngleCircle = spinAngleCircle - 360.0f;
                circleSpinCount += 1;
            }
        }
        else if(circleSpinCount > 4)
        {
            if(wandy >= 0.0f )
            {
                wandy = wandy - 0.001f;
                spinAngleTriangle += 0.5f;
                spinAngleCircle += 0.5f;
            }
            else if(wandy <= 0.0f && (int)spinAngleTriangle % (int)180.0f != 0)
            {
                spinAngleTriangle += 0.5f;
                spinAngleCircle += 0.5f;
            }
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
	if(colormap)
	{
		XFreeColormap(display, colormap);
	}
	if(display)
	{
		XCloseDisplay(display);
		display = NULL;
	}
}