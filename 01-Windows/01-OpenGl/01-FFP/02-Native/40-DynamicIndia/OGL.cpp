// Header files
#include<windows.h> //SDK path madhli header file declare karaichi padhhat
#include"OGL.h"  //Aplya path (local) madhli header file declare karaichi padhhat
#include<stdio.h> // For FileIO()
#include<stdlib.h> // For Exit()
#define _USE_MATH_DEFINES
#include<math.h> // cos() , sin() sathi

// OpenGL header files
#include<GL/gl.h>
#include<GL/glu.h>

// OpenGL Libraries
#pragma comment(lib,"OpenGL32.lib") 
#pragma comment(lib,"GLU32.lib") 

// Defines
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variable Declarations
HWND ghwnd=NULL;
HDC ghdc=NULL;
HGLRC ghrc=NULL;
BOOL gbFullScreen=FALSE;
FILE *gpFile=NULL;
BOOL gbActiveWindow=FALSE;
float x=0.0f, y=0.0f ;
float R=0.0f, G=0.0f, B=0.0f;
float jet1_x= -2.0f,   jet1_y= -1.0f,
      jet2_x = -6.0f, 
      jet3_x = -2.0f,  jet3_y = 1.0f;
float angleRotationJet1 = -90.0f, angleRotationJet3 = 90.0f;
float angleRevolutionJet1 = 180.0f, angleRevolutionJet3 = 180.0f;
float translatorI1x = -2.0f, translatorNy = 1.5f, translatorI2y = -2.0f, translatorAx = 3.0f; 
float colorMaxOrRe = 0.0f, colorMaxOrGr = 0.0f, colorMaxGrGr = 0.0f, colorMaxWh = 0.0f;

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    // Function Declarations
    int initialize(void);
    void display(void);
    void update(void);
    void uninitialize(void);

    // Variable Declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[]= TEXT("MaziWindow");
    BOOL bDone=FALSE;
    int iRetval=0;

    // Code
    if(fopen_s(&gpFile,"Log.txt","w")!=0)// fopen_s(File Open Secured)
    {
        MessageBox(NULL,TEXT("Creation of log file failed. Exiting...\n"),TEXT("FileIO Error"),MB_OK);
        exit(0);
    }
    else
    {
        fprintf(gpFile,"Log File Is Successfully Created. \n");
    }

    // Initialization of WNDCLASSEX structure
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // BLACK_BRUSH
    wndclass.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(MYICON)); //ID of icon , MYICON int aslyane tyala string madhe convert karnyasathi MAKEINTRESOURCE macro waparaava laagto
    wndclass.hCursor = LoadCursor(NULL , IDC_ARROW); // ID of cursor
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE(MYICON)); //Chotu Icon sathi 

    // Register The WNDCLASSEX
    RegisterClassEx(&wndclass);

    // Create The Window
    hwnd = CreateWindowEx ( WS_EX_APPWINDOW,
                            szAppName,
                            TEXT("Sahil Ajit Buwa - OGL window"),
                            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                            (GetSystemMetrics(SM_CXSCREEN)-WIN_WIDTH)/2, 
                            (GetSystemMetrics(SM_CYSCREEN)-WIN_HEIGHT)/2,
                            WIN_WIDTH,
                            WIN_HEIGHT,
                            NULL,
                            NULL, //Menu Handle , refer 28
                            hInstance, 
                            NULL // Extra Maahiti dya ki
                        );

    ghwnd=hwnd;
    // Initialize
    iRetval=initialize();
    if(iRetval==-1)
    {
        fprintf(gpFile,"Choose pixel format failed.\n");
        uninitialize();
    }
    else if(iRetval==-2)
    {
        fprintf(gpFile,"Set pixel format failed.\n");
        uninitialize();
    }

    else if(iRetval==-3)
    {
        fprintf(gpFile,"Create OpenGL context failed.\n");
        uninitialize();
    }

    else if(iRetval==-4)
    {
        fprintf(gpFile,"Making OpenGL context as current context failed.\n");
        uninitialize();
    }
    else
    {
        fprintf(gpFile,"OpenGL context setting successfull.\n");
    }

    // Show Window
    ShowWindow(hwnd, iCmdShow  ); // SW_SHOWNORMAL
    // SW_MAXIMIZE |SW_MINIMIZE |SW_HIDE (2nd parameter)
    // Window default kashi dakhav te OS che behaviour function cha 4th parameter

    // Foregrounding and focusing the window
    SetForegroundWindow(hwnd); // Both(hwnd,ghwnd) will do good. 
    SetFocus(hwnd);

    // Game Loop
    while(bDone==FALSE)
    {
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if(msg.message==WM_QUIT)
                bDone=TRUE;
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if(gbActiveWindow==TRUE)
            {
                // Render the scene
                display();

                // Update the scene
                update();

            }
        }
    }
    uninitialize();
    return (int)msg.wParam; 
}

// Callback Function

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    // Function Declarations
    void ToggleFullScreen(void);
    void resize(int,int);

    // Variable Declarations
    TCHAR str[255];

    // Code
    switch(iMsg)
    {
        case WM_SETFOCUS:
            gbActiveWindow=TRUE;
            break;
        case WM_KILLFOCUS:
            gbActiveWindow=FALSE;
            break;
        case WM_ERASEBKGND:
            return 0;
        // As this is retained mode graphics, there is WM_PAINT to paint.    
        case WM_CHAR:
            switch(wParam)
            {
                case 'F':
                case 'f':
                    ToggleFullScreen();
                    break;
                default:
                    break;
            }
            break;
        case WM_KEYDOWN:
            switch(wParam)
            {
                case 27:
                    DestroyWindow(hwnd);
                    break;
                default:
                    break;
            }
            break;
        case WM_SIZE:
            resize(LOWORD(lParam),HIWORD(lParam));
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void ToggleFullScreen(void)
{
    // Variable Declarations
    static DWORD dwStyle;
    static WINDOWPLACEMENT wp;
    MONITORINFO mi;

    // Code
    wp.length=sizeof(WINDOWPLACEMENT);

    if(gbFullScreen==FALSE)
    {
        dwStyle=GetWindowLong(ghwnd,GWL_STYLE);
        if(dwStyle & WS_OVERLAPPEDWINDOW)
        {
            mi.cbSize=sizeof(MONITORINFO);

            if( GetWindowPlacement( ghwnd, &wp) && GetMonitorInfo( MonitorFromWindow( ghwnd, MONITORINFOF_PRIMARY), &mi))
            {
                SetWindowLong( ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW );
                SetWindowPos( ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right-mi.rcMonitor.left, mi.rcMonitor.bottom-mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); //WM_NCCalcSize non client area calculate kar ani consider kar
            }    
            ShowCursor(FALSE);
            gbFullScreen=TRUE;
        }
    }
    else
    {
        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement( ghwnd, &wp);
        SetWindowPos( ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED );

        ShowCursor(TRUE);
        gbFullScreen=FALSE;
    }

}

int initialize(void)
{
    // Function Declarations
    void resize(int,int);
    // Variable Declarations
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex=0;
    // Code
    ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));
    // Initialization of PIXELFORMATDESCRIPTOR structure
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;
    
    // GetDC
    ghdc = GetDC(ghwnd);

    // Choose Pixel Format
    iPixelFormatIndex = ChoosePixelFormat( ghdc, &pfd);
    if(iPixelFormatIndex==0)
        return -1;

    // Set Chosen Pixel Format
    if(SetPixelFormat(ghdc,iPixelFormatIndex,&pfd)==FALSE)
        return -2;
    
    // Create OpenGL rendereing context
    ghrc = wglCreateContext(ghdc);
    if(ghrc==NULL)
        return -3;

    // Make the rendering context as current context
    if(wglMakeCurrent(ghdc,ghrc)==FALSE)
        return -4;

    // Here Starts OpenGL code
    // Clear the screen using blue color
    glClearColor(0.0f,0.0f,0.0f,1.0f);
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

void display(void)
{
    // Function Declarations
    void DrawLetterI(float,float);
    void DrawLetterN(float,float);
    void DrawLetterD(float,float);
    void DrawLetterA(float,float);
    void DrawJet(float,float,float,float,float);
    // Code
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

    glTranslatef(0.0f, 0.0f, -4.0f);
    glScalef(1.0f,1.5f,0.0f);

    DrawLetterI(translatorI1x, -0.1f);
    DrawLetterN(-0.5f, translatorNy);
    DrawLetterD(0.2f,  -0.1f);
    DrawLetterI(0.7f,  translatorI2y);
    DrawLetterA(translatorAx,  -0.1f);
    if(jet1_x<1.6f && translatorAx <= 1.25f)
    {
        // Jet1
        glLoadIdentity();
        glTranslatef(jet1_x,jet1_y,-4.0f);
        glRotatef(angleRotationJet1,0.0f, 0.0f, 1.0f);
        DrawJet(0.0f, -0.2f , 1.0f, 0.5f, 0.0f);

        // Jet2
        glLoadIdentity();
        glTranslatef(jet2_x, -0.05f,-4.0f);
        DrawJet(0.0f, -0.2f , 1.0f, 1.0f, 1.0f);

        // Jet3
        glLoadIdentity();
        glTranslatef(jet3_x, jet3_y,-4.0f);
        glRotatef(angleRotationJet3,0.0f, 0.0f, 1.0f);
        DrawJet(0.0f, -0.2f , 0.0f, 1.0f, 0.0f);
    }
    else if(jet1_x>=1.6f && translatorAx <= 1.25f)
    {
         // Jet1
        glLoadIdentity();
        glTranslatef(jet1_x,jet1_y,-4.0f);
        glRotatef(angleRotationJet1,0.0f, 0.0f, 1.0f);
        DrawJet(0.0f, -0.2f , 1.0f, 0.5f, 0.0f);

        // Jet2
        glLoadIdentity();
        glTranslatef(jet2_x, -0.05f,-4.0f);
        DrawJet(0.0f, -0.2f , 1.0f, 1.0f, 1.0f);

        // Jet3
        glLoadIdentity();
        glTranslatef(jet3_x, jet3_y,-4.0f);
        glRotatef(angleRotationJet3,0.0f, 0.0f, 1.0f);
        DrawJet(0.0f, -0.2f , 0.0f, 1.0f, 0.0f);
    }
    SwapBuffers(ghdc);
}

void DrawLetterI(float x, float y)
{
    // Upper quad
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(x+0.25f, y+0.5f, 0.0f);
    glVertex3f(x-0.25f, y+0.5f, 0.0f);
    glVertex3f(x-0.25f, y+0.4f, 0.0f);
    glVertex3f(x+0.25f, y+0.4f, 0.0f);
    glEnd();
    
    // Middle Quad Upper
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
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
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x-0.05f, y-0.3f, 0.0f);
    glVertex3f(x+0.05f, y-0.3f, 0.0f);
    glEnd();

    // Lower Quad
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);
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
    glColor3f(1.0f, 0.5f, 0.0f);
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
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x+0.0f, y-0.3f, 0.0f);
    glVertex3f(x+0.1f, y-0.3f, 0.0f);
    glEnd();
    
    // Middle Tirka Quad Upper
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(x-0.3f, y+0.5f, 0.0f);
    glVertex3f(x-0.4f, y+0.5f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.2f, y+0.1f, 0.0f);
    glVertex3f(x-0.1f, y+0.1f, 0.0f);
    glEnd();

    // Middle Tirka Quad Lower
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x+0.0f, y-0.3f, 0.0f);
    glVertex3f(x+0.1f, y-0.3f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.1f, y+0.1f, 0.0f);
    glVertex3f(x-0.2f, y+0.1f, 0.0f);
    glEnd();

    // Left Quad Upper
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
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
    glColor3f(0.0f, 1.0f, 0.0f);
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
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x-0.4f, y+0.1f, 0.0f);
    glVertex3f(x-0.3f, y+0.1f, 0.0f);
    glEnd();

    // Left Vertical Quad Lower
    glBegin(GL_QUADS);
    glColor3f(colorMaxWh, colorMaxWh, colorMaxWh);
    glVertex3f(x-0.3f, y+0.1f, 0.0f);
    glVertex3f(x-0.4f, y+0.1f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
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
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x+0.0f, y+0.1f, 0.0f);
    glVertex3f(x+0.1f, y+0.1f, 0.0f);
    glEnd();

    // Right Vertical Lower Quad
    glBegin(GL_QUADS);
    glColor3f(colorMaxWh, colorMaxWh, colorMaxWh);
    glVertex3f(x+0.1f, y+0.1f, 0.0f);
    glVertex3f(x+0.0f, y+0.1f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
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
    glColor3f(1.0f, 0.5f, 0.0f);
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
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x-0.25f, y-0.3f, 0.0f);
    glVertex3f(x-0.15f, y-0.3f, 0.0f);
    glEnd();
    
    // Right Upper Half 
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
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
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x+0.15f, y-0.3f, 0.0f);
    glVertex3f(x+0.25f, y-0.3f, 0.0f);
    glEnd();

    if(jet2_x>1.66f)
    {
        // Madhli Advi Dandi
        glBegin(GL_QUADS);
        // Hirvi Dandi
        glColor3f(0.0f, 1.0f, 0.0f);
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
        glColor3f(1.0f, 0.7f, 0.0f);
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
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-3.2f,1.8f,0.0f);
    glVertex3f(-2.8f,1.8f,0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-3.2f,1.5f,0.0f);
    glVertex3f(-3.0f,1.5f,0.0f);
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-3.2f,1.8f,0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-3.2f,1.2f,0.0f);
    glEnd();

}


void update(void)
{
    // Code
    fprintf(gpFile,"%f\t%f\t%f\t%f\t",colorMaxGrGr,colorMaxOrRe,colorMaxOrGr,colorMaxWh);
    // Alphabets Translations
    if(translatorI1x<=-1.2f)
        translatorI1x = translatorI1x + 0.0005f;
    else
    {
        if(translatorNy>=-0.1f)
            translatorNy = translatorNy - 0.0005f;
        else
        {
            if(colorMaxWh <=1.0f && colorMaxOrRe <=1.0f && colorMaxOrGr<=0.5f && colorMaxGrGr <= 1.0f )
            {
                colorMaxWh = colorMaxWh + 0.0005f;
                colorMaxOrRe = colorMaxOrRe + 0.0005;
                colorMaxGrGr = colorMaxGrGr + 0.0005;
                colorMaxOrGr = colorMaxOrGr + 0.0005;
            }
            else
            {
                if(translatorI2y<=-0.1f)
                    translatorI2y = translatorI2y + 0.0005f;
                else
                {
                    if(translatorAx>=1.25f)
                        translatorAx = translatorAx - 0.0005f;
                }
            }
        }
    }
    if(translatorAx <= 1.25f)
    {
        // Jet 2 x movement
        jet2_x = jet2_x + 0.0005;

        // Jet Rotation Values
        if(angleRotationJet1<= 0.0f && jet1_x<1.66f)
            angleRotationJet1 = angleRotationJet1 + 0.01f;
        else if(angleRotationJet1 <= 90.f && jet1_x >= 1.66f)
            angleRotationJet1 = angleRotationJet1 + 0.01f;
        if(angleRotationJet3>= 0.0f && jet3_x<1.66f)
            angleRotationJet3 = angleRotationJet3 - 0.01f;
        else if (angleRotationJet3 >= -90.0f && jet3_x>=1.66f)
            angleRotationJet3 = angleRotationJet3 - 0.01f;
        
        // Jet 1 and 3 movements
        // Curved start
        if(angleRevolutionJet1<270.0f && jet1_x < 0.0f)
        {
            angleRevolutionJet1 = angleRevolutionJet1 + 0.01f;
            float angle= angleRevolutionJet1 * (M_PI / 180);
            jet1_x =  1.5 * cos(angle) - 1.55f;
            jet1_y =  1.45 * sin(angle) + 1.45f;
        }
        if(angleRevolutionJet3>=90.0f && jet3_x < 0.0f)
        {
            angleRevolutionJet3 = angleRevolutionJet3 - 0.01f;
            float angle = angleRevolutionJet3 * (M_PI / 180);
            jet3_x =  1.5 * cos(angle) - 1.55f;
            jet3_y =  1.45 * sin(angle) - 1.55f;
        }

        // Horizontal Travel
        if(angleRevolutionJet1>270.0f && jet1_x < 1.63f)
        {
            jet1_x = jet1_x + 0.0005f;
        }
        if(angleRevolutionJet3<90.0f && jet3_x < 1.63f)
        {
            jet3_x = jet3_x + 0.0005f;
        }
        // Curved End
        if(jet1_x >= 1.5f && angleRevolutionJet1 <= 360.0f && angleRevolutionJet3 >= 0.0f)
        {
            angleRevolutionJet1 += 0.01f;
            float angle= angleRevolutionJet1 * (M_PI / 180);
            jet1_x =  1.65 * cos(angle) + 1.5f;
            jet1_y =  1.45 * sin(angle) + 1.45f;
            angleRevolutionJet3 -= 0.01f;
            angle = angleRevolutionJet3 * (M_PI / 180);
            jet3_x =  1.55 * cos(angle) + 1.5f;
            jet3_y =  1.45 * sin(angle) - 1.55f;
        }
    }
}

void uninitialize(void)
{
    // Function Declarations
    void ToggleFullscreen(void);
    // Code
    if(gbFullScreen)
    {
        ToggleFullScreen();
    }
    if(wglGetCurrentContext()==ghrc)
    {
        wglMakeCurrent(NULL,NULL);
    }
    if(ghrc)
    {
        wglDeleteContext(ghrc);
        ghrc=NULL;
    }
    if(ghdc)
    {
        ReleaseDC(ghwnd,ghdc);
        ghdc=NULL;
    }
    if(ghwnd)
    {
        DestroyWindow(ghwnd);
        ghwnd=NULL;
    }
    if(gpFile)
    {
        fprintf(gpFile,"Log File Is Successfully Closed.\n");
        fclose(gpFile);
        gpFile=NULL;
    }
}
