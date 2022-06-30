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
XVisualInfo visualInfo; // 10 member struct - 1 member 'visual' which has 8 members.
Colormap colormap;
Window window;
Bool fullscreen = False;

// Entry-point function
int main(void)
{
	// Function Declarations
	void uninitialize(void);
	void toggleFullscreen(void);
	
	// Local Variables
	int defaultScreen;
	int defaultDepth;
	Status status;
	XSetWindowAttributes windowAttributes;
	int styleMask;
	Atom wm_delete_window_atom;
	XEvent event; // Among 33 member structures 31 are structs and 2 unions
	KeySym keysym;
	int screenWidth;
	int screenHeight;
	char keys[26];

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
	status = XMatchVisualInfo(display, defaultScreen, defaultDepth, TrueColor /*colorClass*/, &visualInfo); //StaticColor pn chalel 4th param , frame buffer is still visual here as OpenGL is not present yet, similar to pixelformatdescriptor
	if(status == 0)
	{
		printf("ERROR:XMatchVisualInfo() failed.\n");
		uninitialize();
		exit(1);
	}	
	memset(&windowAttributes, 0, sizeof(XSetWindowAttributes));
	windowAttributes.border_pixel = 0;
	windowAttributes.background_pixel = XBlackPixel(display, defaultScreen); // BLACK_BRUSH similar
	windowAttributes.background_pixmap = 0;
	windowAttributes.colormap = XCreateColormap(display, 
												RootWindow(display, visualInfo.screen), // DefaultColorScreen pn Chalel ,MoolPurush Window
												visualInfo.visual,
												AllocNone); // 2^32 colors astat color map madhe , smallest part is one color->colorcell->contains RGB
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
	XStoreName(display, window, "SAB:Window"); // Name the created window
	wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", True); // WM - window manager
	XSetWMProtocols(display, window, &wm_delete_window_atom, 1);
	XMapWindow(display, window); //Similar to ShowWindow() 
	
	// Centering of window
	screenWidth = XWidthOfScreen(XScreenOfDisplay(display, defaultScreen));
	screenHeight = XHeightOfScreen(XScreenOfDisplay(display, defaultScreen));
	XMoveWindow(display, window, (screenWidth-WIN_WIDTH)/2, (screenHeight-WIN_HEIGHT)/2);

	// Message Loop
	while(1)
	{
		XNextEvent(display, &event); // GetMessage() similar 
		switch(event.type)
		{
			case MapNotify: // WM_CREATE
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
			case 33:
				uninitialize();
				exit(0);
				break;
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
	event.xclient.data.l[0] = fullscreen? 0 : 1;
	event.xclient.data.l[1] = wm_fullscreen_state_atom;
	XSendEvent( display, 
			    RootWindow(display,visualInfo.screen),
				False,
				SubstructureNotifyMask,
				&event);
	
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
