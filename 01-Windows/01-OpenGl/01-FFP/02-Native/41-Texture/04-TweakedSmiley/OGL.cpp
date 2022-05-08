// Header files
#include<windows.h> //SDK path madhli header file declare karaichi padhhat
#include"OGL.h"  //Aplya path (local) madhli header file declare karaichi padhhat
#include<stdio.h> // For FileIO()
#include<stdlib.h> // For Exit()

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
GLuint texture_smiley;
int gKeypressed = -1;

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
                case 49:
                    gKeypressed = 1;
                    glEnable(GL_TEXTURE_2D);
                    break;
                case 50:
                    gKeypressed = 2;
                    glEnable(GL_TEXTURE_2D);
                    break;
                case 51:
                    gKeypressed = 3;
                    glEnable(GL_TEXTURE_2D);
                    break;
                case 52:
                    gKeypressed = 4;
                    glEnable(GL_TEXTURE_2D);
                    break;
                default:
                    glDisable(GL_TEXTURE_2D);
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
    // Declaration of user-defined functions
    BOOL LoadGLTexture(GLuint* ,TCHAR[]);
    // Function Declarations
    void resize(int,int);
    void uninitialize(void);
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
    pfd.cDepthBits = 32; // 24 pan chaltai
    
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
    if(LoadGLTexture(&texture_smiley, MAKEINTRESOURCE(IDBITMAP_SMILEY))==FALSE)
    {
        fprintf(gpFile,"LoadGLTexture for smiley Failed.\n");
        uninitialize();
        return -5;
    }

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
    // glEnable(GL_TEXTURE_2D);

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
    // Variable Declarations
    
    // Code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    fprintf(gpFile, "%d\n",texture_smiley); 
    glTranslatef(0.0f,0.0f,-4.0f);
    glScalef(0.75f, 0.75f, 0.75f);
    glBindTexture(GL_TEXTURE_2D, texture_smiley);
    if(gKeypressed == 1)
    {
        glBegin(GL_QUADS);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.5f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(0.5f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glEnd();
    }
    else if(gKeypressed == 2)
    {
        glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glEnd();
    }
    else if(gKeypressed == 3)
    {
        glBegin(GL_QUADS);
        glTexCoord2f(2.0f, 2.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 2.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(2.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glEnd();
    }
    else if(gKeypressed == 4)
    {
        glBegin(GL_QUADS);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glEnd();
    }
    else
    {
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glEnd();
    }
    SwapBuffers(ghdc);
}

void update(void)
{
    // Code
}

BOOL LoadGLTexture(GLuint *texture, TCHAR imageResourceID[])
{
    // Variable Declarations
    HBITMAP hBitmap = NULL;
    BITMAP bmp;
    BOOL bResult = FALSE;
    // Code
    hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if(hBitmap)
    {
        bResult = TRUE;
        GetObject(hBitmap, sizeof(bmp), &bmp);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Create The Texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
        glBindTexture(GL_TEXTURE_2D, 0);
        DeleteObject(hBitmap);

    }
    return bResult;
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
    if(texture_smiley)
    {
        glDeleteTextures(1, &texture_smiley);
    }
    if(gpFile)
    {
        fprintf(gpFile,"Log File Is Successfully Closed.\n");
        fclose(gpFile);
        gpFile=NULL;
    }
}