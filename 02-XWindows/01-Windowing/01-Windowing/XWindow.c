// Standard Headers
#include<stdio.h> // For standard IO
#include<stdlib.h> // exit() sathi
#include<memory.h> // For memset()

// X11 Headers
#include<X11/Xlib.h> // windows.h saarkhi 
#include<X11/Xutil.h> // XVisualInfo() 
#include<X11/XKBlib.h>

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
// Visual info is not uninitialized as we have not created it but matched its params....
Display *display = NULL; // 77 member struct
XVisualInfo visualInfo; // 10 member struct - 1 member visual which has 8 members.
Colormap colormap;
Window window;

// Entry-point function
int main(void)
{
	// Function Declarations
	void uninitialize(void);
	
	// Local Variables
	int defaultScreen;
	int defaultDepth;
	Status status;
	XSetWindowAttributes windowAttributes;
	int styleMask;
	Atom wm_delete_window_atom;
	XEvent event;
	KeySym keysym;
	
	// Code
	display = XOpenDisplay(NULL);
	if(display == NULL)
	{
		printf("ERROR:XOpenDisplay() failed.\n");
		uninitialize();
		exit(1);
	}
	defaultScreen = XDefaultScreen(display);
	defaultDepth = XDefaultDepth(display, defaultScreen);
	status = XMatchVisualInfo(display, defaultScreen, defaultDepth, TrueColor, &visualInfo); //StaticColor pn chalel 4th param
	if(status == 0)
	{
		printf("ERROR:XMatchVisualInfo() failed.\n");
		uninitialize();
		exit(1);
	}	
	memset(&windowAttributes, 0, sizeof(XSetWindowAttributes));
	windowAttributes.border_pixel = 0;
	windowAttributes.background_pixel = XBlackPixel(display, defaultScreen);
	windowAttributes.background_pixmap = 0;
	windowAttributes.colormap = XCreateColormap(display, 
												RootWindow(display, visualInfo.screen), // DefaultColorScreenChalel
												visualInfo.visual,
												AllocNone); // 2^32 colors astat color map madhe
	windowAttributes.event_mask = ExposureMask | KeyPressMask;
	colormap = windowAttributes.colormap;
	styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;
	window = XCreateWindow(display, RootWindow(display, visualInfo.screen), 0, 0, WIN_WIDTH, WIN_HEIGHT, 0, visualInfo.depth, InputOutput, visualInfo.visual, styleMask, &windowAttributes);
	if(!window)
	{
		printf("ERROR:XCreateWindow() failed.\n");
		uninitialize();
		exit(1);
	}
	XStoreName(display, window, "SAB:Window");
	wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", True); // WM - window manager
	XSetWMProtocols(display, window, &wm_delete_window_atom, 1);
	XMapWindow(display, window); //ShowWindow
	
	// Message Loop
	while(1)
	{
		XNextEvent(display, &event);
		switch(event.type)
		{
			case MapNotify:
				break;
			case KeyPress:
				keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);			
				switch(keysym)
				{
					case XK_Escape:
						uninitialize();
						exit(0);
						break;
				}
				break;
			case 33:
				uninitialize();
				exit(0);
				break;
		}
	}
	
	uninitialize();
	return 0;
}

void uninitialize(void)
{
	// Code
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

