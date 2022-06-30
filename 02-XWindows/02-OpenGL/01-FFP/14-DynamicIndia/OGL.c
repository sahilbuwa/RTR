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
float x=0.0f, y=0.0f ;
float R=0.0f, G=0.0f, B=0.0f;
float jet1_x= -2.0f,   jet1_y= -1.0f,
      jet2_x = -6.0f, 
      jet3_x = -2.0f,  jet3_y = 1.0f;
float angleRotationJet1 = -90.0f, angleRotationJet3 = 90.0f;
float angleRevolutionJet1 = 180.0f, angleRevolutionJet3 = 180.0f;
float translatorI1x = -2.0f, translatorNy = 1.5f, translatorI2y = -2.0f, translatorAx = 3.5f; 
float colorMaxOrRe = 0.0f, colorMaxOrGr = 0.0f, colorMaxOrBl = 0.0f,
      colorMaxGrRe = 0.0f, colorMaxGrGr = 0.0f, colorMaxGrBl = 0.0f,
      colorMaxWh = 0.0f;

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
    void toggleFullscreen(void);
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

    // AadhiPaasun Fullscreen
    toggleFullscreen();

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
	// Function Declarations
    void DrawLetterI(float,float);
    void DrawLetterN(float,float);
    void DrawLetterD(float,float);
    void DrawLetterA(float,float);
    void DrawJet(float,float,float,float,float);
    // Code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

    glTranslatef(0.0f, 0.0f, -4.0f);
    glScalef(1.0f,1.5f,0.0f);

    DrawLetterI(translatorI1x, -0.1f);
    DrawLetterN(-0.5f, translatorNy);
    DrawLetterD(0.15f,  -0.1f);
    DrawLetterI(0.65f,  translatorI2y);
    DrawLetterA(translatorAx,  -0.1f);

    if(jet1_x<1.6f && translatorAx <= 1.25f)
    {
        // Jet1
        glLoadIdentity();
        glTranslatef(jet1_x,jet1_y,-4.0f);
        glRotatef(angleRotationJet1,0.0f, 0.0f, 1.0f);
        DrawJet(0.0f, -0.2f , 1.0f, (GLfloat)153/255, (GLfloat)51/255);

        // Jet2
        glLoadIdentity();
        glTranslatef(jet2_x, -0.05f,-4.0f);
        DrawJet(0.0f, -0.2f , 1.0f, 1.0f, 1.0f);

        // Jet3
        glLoadIdentity();
        glTranslatef(jet3_x, jet3_y,-4.0f);
        glRotatef(angleRotationJet3,0.0f, 0.0f, 1.0f);
        DrawJet(0.0f, -0.2f ,(GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    }
    else if(jet1_x>=1.6f && translatorAx <= 1.25f)
    {
         // Jet1
        glLoadIdentity();
        glTranslatef(jet1_x,jet1_y,-4.0f);
        glRotatef(angleRotationJet1,0.0f, 0.0f, 1.0f);
        DrawJet(0.0f, -0.2f , 1.0f, (GLfloat)153/255, (GLfloat)51/255);

        // Jet2
        glLoadIdentity();
        glTranslatef(jet2_x, -0.05f,-4.0f);
        DrawJet(0.0f, -0.2f , 1.0f, 1.0f, 1.0f);

        // Jet3
        glLoadIdentity();
        glTranslatef(jet3_x, jet3_y,-4.0f);
        glRotatef(angleRotationJet3,0.0f, 0.0f, 1.0f);
        DrawJet(0.0f, -0.2f ,(GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    }

	glXSwapBuffers(display, window);
}

void DrawLetterI(float x, float y)
{
    // Upper quad
    glBegin(GL_QUADS);
    glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
    glVertex3f(x+0.25f, y+0.5f, 0.0f);
    glVertex3f(x-0.25f, y+0.5f, 0.0f);
    glVertex3f(x-0.25f, y+0.4f, 0.0f);
    glVertex3f(x+0.25f, y+0.4f, 0.0f);
    glEnd();
    
    // Middle Quad Upper
    glBegin(GL_QUADS);
    glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
    glVertex3f(x+0.05f, y+0.4f, 0.0f);
    glVertex3f(x-0.05f, y+0.4f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.05f, y+0.1f, 0.0f);
    glVertex3f(x+0.05f, y+0.1f, 0.0f);
    glEnd();

    // Middle Quad Lower
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x+0.05f, y+0.1f, 0.0f);
    glVertex3f(x-0.05f, y+0.1f, 0.0f);
    glColor3f((GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    glVertex3f(x-0.05f, y-0.3f, 0.0f);
    glVertex3f(x+0.05f, y-0.3f, 0.0f);
    glEnd();

    // Lower Quad
    glBegin(GL_QUADS);
    glColor3f((GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    glVertex3f(x+0.25f, y-0.2f, 0.0f);
    glVertex3f(x-0.25f, y-0.2f, 0.0f);
    glVertex3f(x-0.25f, y-0.3f, 0.0f);
    glVertex3f(x+0.25f, y-0.3f, 0.0f);
    glEnd();
}

void DrawLetterN(float x, float y)
{   
    // Right Quad Upper
    glBegin(GL_QUADS);
    glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
    glVertex3f(x+0.1f, y+0.5f, 0.0f);
    glVertex3f(x+0.0f, y+0.5f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x+0.0f, y+0.1f, 0.0f);
    glVertex3f(x+0.1f, y+0.1f, 0.0f);
    glEnd();
    
    // Right Quad Lower
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x+0.1f, y+0.1f, 0.0f);
    glVertex3f(x+0.0f, y+0.1f, 0.0f);
    glColor3f((GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    glVertex3f(x+0.0f, y-0.3f, 0.0f);
    glVertex3f(x+0.1f, y-0.3f, 0.0f);
    glEnd();
    
    // Middle Tirka Quad Upper
    glBegin(GL_QUADS);
    glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
    glVertex3f(x-0.3f, y+0.5f, 0.0f);
    glVertex3f(x-0.4f, y+0.5f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.2f, y+0.1f, 0.0f);
    glVertex3f(x-0.1f, y+0.1f, 0.0f);
    glEnd();

    // Middle Tirka Quad Lower
    glBegin(GL_QUADS);
    glColor3f((GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    glVertex3f(x+0.0f, y-0.3f, 0.0f);
    glVertex3f(x+0.1f, y-0.3f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.1f, y+0.1f, 0.0f);
    glVertex3f(x-0.2f, y+0.1f, 0.0f);
    glEnd();

    // Left Quad Upper
    glBegin(GL_QUADS);
    glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
    glVertex3f(x-0.3f, y+0.5f, 0.0f);
    glVertex3f(x-0.4f, y+0.5f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.4f, y+0.1f, 0.0f);
    glVertex3f(x-0.3f, y+0.1f, 0.0f);
    glEnd();

    // Left Quad Lower
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.3f, y+0.1f, 0.0f);
    glVertex3f(x-0.4f, y+0.1f, 0.0f);
    glColor3f((GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    glVertex3f(x-0.4f, y-0.3f, 0.0f);
    glVertex3f(x-0.3f, y-0.3f, 0.0f);
    glEnd();
}

void DrawLetterD(float x, float y)
{
    // Left Vertical Quad Upper
    glBegin(GL_QUADS);
    glColor3f(colorMaxOrRe, colorMaxOrGr, 0.0f);
    glVertex3f(x-0.3f, y+0.5f, 0.0f);
    glVertex3f(x-0.4f, y+0.5f, 0.0f);
    glColor3f(colorMaxWh, colorMaxWh, colorMaxWh);
    glVertex3f(x-0.4f, y+0.1f, 0.0f);
    glVertex3f(x-0.3f, y+0.1f, 0.0f);
    glEnd();

    // Left Vertical Quad Lower
    glBegin(GL_QUADS);
    glColor3f(colorMaxWh, colorMaxWh, colorMaxWh);
    glVertex3f(x-0.3f, y+0.1f, 0.0f);
    glVertex3f(x-0.4f, y+0.1f, 0.0f);
    glColor3f(0.0f, colorMaxGrGr, 0.0f);
    glVertex3f(x-0.4f, y-0.3f, 0.0f);
    glVertex3f(x-0.3f, y-0.3f, 0.0f);
    glEnd();

    // Top Horizontal Quad
    glBegin(GL_QUADS);
    glColor3f(colorMaxOrRe, colorMaxOrGr, 0.0f);
    glVertex3f(x-0.3f, y+0.5f, 0.0f);
    glVertex3f(x-0.3f, y+0.4f, 0.0f);
    glVertex3f(x+0.0f, y+0.4f, 0.0f);
    glVertex3f(x+0.0f, y+0.5f, 0.0f);
    glEnd();

    // Right Vertical Upper Quad
    glBegin(GL_QUADS);
    glColor3f(colorMaxOrRe, colorMaxOrGr, 0.0f);
    glVertex3f(x+0.1f, y+0.4f, 0.0f);
    glVertex3f(x+0.0f, y+0.4f, 0.0f);
    glColor3f(colorMaxWh, colorMaxWh, colorMaxWh);
    glVertex3f(x+0.0f, y+0.1f, 0.0f);
    glVertex3f(x+0.1f, y+0.1f, 0.0f);
    glEnd();

    // Right Vertical Lower Quad
    glBegin(GL_QUADS);
    glColor3f(colorMaxWh, colorMaxWh, colorMaxWh);
    glVertex3f(x+0.1f, y+0.1f, 0.0f);
    glVertex3f(x+0.0f, y+0.1f, 0.0f);
    glColor3f(0.0f, colorMaxGrGr, 0.0f);
    glVertex3f(x+0.0f, y-0.2f, 0.0f);
    glVertex3f(x+0.1f, y-0.2f, 0.0f);
    glEnd();

    // Bottom Horizontal Quad
    glBegin(GL_QUADS);
    glColor3f(0.0f, colorMaxGrGr, 0.0f);
    glVertex3f(x-0.3f, y-0.3f, 0.0f);
    glVertex3f(x-0.3f, y-0.2f, 0.0f);
    glVertex3f(x+0.0f, y-0.2f, 0.0f);
    glVertex3f(x+0.0f, y-0.3f, 0.0f);
    glEnd();

    // Bottom Right Corner
    glBegin(GL_QUADS);
    glColor3f(0.0f, colorMaxGrGr, 0.0f);
    glVertex3f(x+0.0f, y-0.2f, 0.0f);
    glVertex3f(x+0.0f, y-0.3f, 0.0f);
    glVertex3f(x+0.1f, y-0.2f, 0.0f);
    glVertex3f(x+0.0f, y-0.2f, 0.0f);
    glEnd();

    // Top Right Corner
    glBegin(GL_QUADS);
    glColor3f(colorMaxOrRe, colorMaxOrGr, 0.0f);
    glVertex3f(x+0.0f, y+0.4f, 0.0f);
    glVertex3f(x+0.0f, y+0.5f, 0.0f);
    glVertex3f(x+0.1f, y+0.4f, 0.0f);
    glVertex3f(x+0.0f, y+0.4f, 0.0f);
    glEnd();
    
}

void DrawLetterA(float x, float y)
{
    // Left Upper Half
    glBegin(GL_QUADS);
    glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
    glVertex3f(x+0.05f, y+0.5f, 0.0f);
    glVertex3f(x-0.05f, y+0.5f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.15f, y+0.1f, 0.0f);
    glVertex3f(x-0.05f, y+0.1f, 0.0f);
    glEnd();

    // Left Lower Half
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.05f, y+0.1f, 0.0f);
    glVertex3f(x-0.15f, y+0.1f, 0.0f);
    glColor3f((GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    glVertex3f(x-0.25f, y-0.3f, 0.0f);
    glVertex3f(x-0.15f, y-0.3f, 0.0f);
    glEnd();
    
    // Right Upper Half 
    glBegin(GL_QUADS);
    glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
    glVertex3f(x+0.05f, y+0.5f, 0.0f);
    glVertex3f(x-0.05f, y+0.5f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x+0.05f, y+0.1f, 0.0f);
    glVertex3f(x+0.15f, y+0.1f, 0.0f);
    glEnd();

    // Right Lower Half
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x+0.15f, y+0.1f, 0.0f);
    glVertex3f(x+0.05f, y+0.1f, 0.0f);
    glColor3f((GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    glVertex3f(x+0.15f, y-0.3f, 0.0f);
    glVertex3f(x+0.25f, y-0.3f, 0.0f);
    glEnd();

    if(jet2_x>1.66f)
    {
        // Madhli Advi Dandi
        glBegin(GL_QUADS);
        // Hirvi Dandi
        glColor3f((GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
        glVertex3f(x+0.06f, y+0.05f, 0.0f);
        glVertex3f(x+0.06f, y+0.08f, 0.0f);
        glVertex3f(x-0.06f, y+0.08f, 0.0f);
        glVertex3f(x-0.06f, y+0.05f, 0.0f);
        // White Dandi  
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(x+0.06f, y+0.08f, 0.0f);
        glVertex3f(x+0.06f, y+0.11f, 0.0f);
        glVertex3f(x-0.06f, y+0.11f, 0.0f);
        glVertex3f(x-0.06f, y+0.08f, 0.0f);
        // Orange Dandi
        glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
        glVertex3f(x+0.05f, y+0.11f, 0.0f);
        glVertex3f(x+0.05f, y+0.14f, 0.0f);
        glVertex3f(x-0.05f, y+0.14f, 0.0f);
        glVertex3f(x-0.05f, y+0.11f, 0.0f);
        glEnd();
    }
}

void DrawJet(float x, float y, float R, float G, float B)
{
    glScalef(0.25f, 0.25f, 0.0f);
    glColor3f(0.0f, 0.5f, 0.8f);
    // Fuselage
    glBegin(GL_QUADS);
    glVertex3f(x-0.3f, y+0.5f, 0.0f);
    glVertex3f(x-0.9f, y+0.5f, 0.0f);
    glVertex3f(x-0.9f, y+0.3f, 0.0f);
    glVertex3f(x-0.3f, y+0.3f, 0.0f);
    glEnd();

    // Nose
    glBegin(GL_TRIANGLES);
    glVertex3f(x-0.3f, y+0.5f, 0.0f);
    glVertex3f(x-0.3f, y+0.3f, 0.0f);
    glVertex3f(x+0.2f,  y+0.4f, 0.0f);
    glEnd();

    // Big Wings
    glBegin(GL_QUADS);
    glVertex3f(x-0.3f, y+0.5f, 0.0f);
    glVertex3f(x-0.6f, y+0.5f, 0.0f);
    glVertex3f(x-0.65f, y+0.8f, 0.0f);
    glVertex3f(x-0.5f, y+0.8f, 0.0f);

    glVertex3f(x-0.3f, y+0.3f, 0.0f);
    glVertex3f(x-0.6f, y+0.3f, 0.0f);
    glVertex3f(x-0.65f, y+0.0f, 0.0f);
    glVertex3f(x-0.5f, y+0.0f, 0.0f);
    
    glEnd();

    // Tail wings
    glBegin(GL_QUADS);
    glVertex3f(x-0.9f, y+0.5f, 0.0f);
    glVertex3f(x-0.7f, y+0.5f, 0.0f);
    glVertex3f(x-0.8f, y+0.65f, 0.0f);
    glVertex3f(x-0.9f, y+0.61f, 0.0f);

    glVertex3f(x-0.9f, y+0.3f, 0.0f);
    glVertex3f(x-0.7f, y+0.3f, 0.0f);
    glVertex3f(x-0.8f, y+0.11f, 0.0f);
    glVertex3f(x-0.9f, y+0.15f, 0.0f);
    
    glEnd();

    // Beautification Lines
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x+0.2f,  y+0.4f, 0.0f);
    glVertex3f(x+0.3f,  y+0.4f, 0.0f);

    glVertex3f(x-0.5f, y+0.8f, 0.0f);
    glVertex3f(x-0.4f, y+0.8f, 0.0f);

    glVertex3f(x-0.5f, y+0.0f, 0.0f);
    glVertex3f(x-0.4f, y+0.0f, 0.0f);
    glEnd();

    // Smoke line Behind
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.9f, y+0.4f, 0.0f);
    glColor3f(R, G, B);
    glVertex3f(x-1.8f, y+0.7f, 0.0f);
    glColor3f(R, G, B);
    glVertex3f(x-1.8f, y+0.0f, 0.0f);
    glEnd();

    // IAF
    glScalef(0.15f,0.15f,0.0f);
    DrawLetterI(-5.0f,1.4f);
    DrawLetterA(-4.0f,1.4f);
    glScalef(1.0f,1.0f,0.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
    glVertex3f(-3.2f,1.8f,0.0f);
    glVertex3f(-2.8f,1.8f,0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-3.2f,1.5f,0.0f);
    glVertex3f(-3.0f,1.5f,0.0f);

    glColor3f(1.0f, (GLfloat)153/255, (GLfloat)51/255);
    glVertex3f(-3.2f,1.8f,0.0f);

    glColor3f((GLfloat)19/255, (GLfloat)136/255, (GLfloat)8/255);
    glVertex3f(-3.2f,1.2f,0.0f);

    glEnd();

}

void update(void)
{
    // Code
    // Alphabets Translations
    if(translatorI1x<=-1.3f)
        translatorI1x = translatorI1x + 0.005f;
    else
    {
        if(translatorNy>=-0.1f)
            translatorNy = translatorNy - 0.005f;
        else
        {
            if(colorMaxWh <= 1.0f)
            {
                colorMaxWh = colorMaxWh + 0.005f;
                colorMaxOrRe = colorMaxOrRe + 0.005;

                if(colorMaxOrGr<(GLfloat)153/255)
                {
                    colorMaxOrGr = colorMaxOrGr + 0.005;
                }
                if(colorMaxOrBl< (GLfloat)51/255)
                {
                    colorMaxOrBl = colorMaxOrBl + 0.005;
                }
                if(colorMaxGrRe < (GLfloat)19/255)
                {
                    colorMaxGrRe = colorMaxGrRe + 0.005;
                }
                if(colorMaxGrGr < (GLfloat)136/255)
                {
                    colorMaxGrGr = colorMaxGrGr + 0.005;
                }
                if(colorMaxGrBl < (GLfloat)8/255)
                {
                    colorMaxGrBl = colorMaxGrBl + 0.005;
                }
            }
            else
            {
                if(translatorI2y<=-0.1f)
                    translatorI2y = translatorI2y + 0.005f;
                else
                {
                    if(translatorAx>=1.25f)
                        translatorAx = translatorAx - 0.009f;
                }
            }
        }
    }
    if(translatorAx <= 1.25f)
    {
        // Jet 2 x movement
        jet2_x = jet2_x + 0.005;

        // Jet Rotation Values
        if(angleRotationJet1<= 0.0f && jet1_x<1.66f)
            angleRotationJet1 = angleRotationJet1 + 0.1f;
        else if(angleRotationJet1 <= 90.f && jet1_x >= 1.66f)
            angleRotationJet1 = angleRotationJet1 + 0.1f;
        if(angleRotationJet3>= 0.0f && jet3_x<1.66f)
            angleRotationJet3 = angleRotationJet3 - 0.1f;
        else if (angleRotationJet3 >= -90.0f && jet3_x>=1.66f)
            angleRotationJet3 = angleRotationJet3 - 0.1f;
        
        // Jet 1 and 3 movements
        // Curved start
        if(angleRevolutionJet1<270.0f && jet1_x < 0.0f)
        {
            angleRevolutionJet1 = angleRevolutionJet1 + 0.1f;
            float angle= angleRevolutionJet1 * (M_PI / 180);
            jet1_x =  1.5 * cos(angle) - 1.55f;
            jet1_y =  1.45 * sin(angle) + 1.45f;
        }
        if(angleRevolutionJet3>=90.0f && jet3_x < 0.0f)
        {
            angleRevolutionJet3 = angleRevolutionJet3 - 0.1f;
            float angle = angleRevolutionJet3 * (M_PI / 180);
            jet3_x =  1.5 * cos(angle) - 1.55f;
            jet3_y =  1.45 * sin(angle) - 1.55f;
        }

        // Horizontal Travel
        if(angleRevolutionJet1>270.0f && jet1_x < 1.63f)
        {
            jet1_x = jet1_x + 0.005f;
        }
        if(angleRevolutionJet3<90.0f && jet3_x < 1.63f)
        {
            jet3_x = jet3_x + 0.005f;
        }
        // Curved End
        if(jet1_x >= 1.5f && angleRevolutionJet1 <= 360.0f && angleRevolutionJet3 >= 0.0f)
        {
            angleRevolutionJet1 += 0.1f;
            float angle= angleRevolutionJet1 * (M_PI / 180);
            jet1_x =  1.65 * cos(angle) + 1.5f;
            jet1_y =  1.45 * sin(angle) + 1.45f;
            angleRevolutionJet3 -= 0.1f;
            angle = angleRevolutionJet3 * (M_PI / 180);
            jet3_x =  1.55 * cos(angle) + 1.5f;
            jet3_y =  1.45 * sin(angle) - 1.55f;
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
