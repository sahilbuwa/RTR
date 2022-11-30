// Standard Header files
#include<windows.h> //SDK path madhli header file declare karaichi padhhat
#include"OGL.h"  // Aplya path (local) madhli header file declare karaichi padhhat
#include<stdio.h> // For FileIO()
#include<stdlib.h> // For Exit()
#include"Sphere.h" // Sphere sathi

// OpenGL header files
#include<GL/glew.h> // This must be above gl.h inclusion. 
#include<GL/gl.h>
#include"vmath.h" // Maths (RedBook)
using namespace vmath;


// OpenGL Libraries
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Sphere.lib")

// Defines
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variable Declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;
FILE *gpFile = NULL;
BOOL gbActiveWindow = FALSE;
// Programmable Pipeline Related Variables
GLuint shaderProgramObject1;
GLuint shaderProgramObject2;
GLchar choosenProgramObject;
enum
{
    SAB_ATTRIBUTE_POSITION = 0,
    SAB_ATTRIBUTE_COLOR,
    SAB_ATTRIBUTE_NORMAL,
    SAB_ATTRIBUTE_TEXURE0
};

GLuint vao_sphere;
GLuint vbo_sphere_position;
GLuint vbo_sphere_normal;
GLuint vbo_sphere_element;
GLuint modelMatrixUniformV;
GLuint viewMatrixUniformV;
GLuint projectionMatrixUniformV;

GLuint modelMatrixUniformF;
GLuint viewMatrixUniformF;
GLuint projectionMatrixUniformF;

// Sphere arrays
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
GLuint numElements;
GLuint numVertices;

mat4 perspectiveProjectionMatrix;

// Lights Variables
BOOL bLight = FALSE;
GLuint ldUniformV[3];
GLuint laUniformV[3];
GLuint lsUniformV[3];
GLuint lightPositionUniformV[3];

GLuint kaUniformV;
GLuint kdUniformV;
GLuint ksUniformV;
GLuint materialShininessUniformV;

GLuint lightingEnabledUniformV;

GLuint ldUniformF[3];
GLuint laUniformF[3];
GLuint lsUniformF[3];
GLuint lightPositionUniformF[3];

GLuint kaUniformF;
GLuint kdUniformF;
GLuint ksUniformF;
GLuint materialShininessUniformF;

GLuint lightingEnabledUniformF;

struct Light
{
    vec4 lightAmbient;
    vec4 lightDiffuse;
    vec4 lightSpecular;
    vec4 lightPosition;
};
Light lights[3];

GLfloat materialAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialShininess = 50.0f;

GLfloat redLightAngle = 0.0f,
        greenLightAngle = 0.0f,
        blueLightAngle = 0.0f;

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
                            TEXT("Sahil Ajit Buwa - OGL Window"),
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
    else if(iRetval==-5)
    {
        fprintf(gpFile,"glewInit() failed.\n");
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
        // As this is not retained mode graphics, there is no WM_PAINT to paint.    
        case WM_CHAR:
            switch(wParam)
            {
                
                case 'L':
                case 'l':
                    if (bLight == FALSE)
                    {
                        bLight = TRUE;
                    }
                    else
                    {
                        bLight = FALSE;
                    }
                    break;
                case 'v':
                case 'V':
                    choosenProgramObject = 'v';
                    break;
                case 'F':
                case 'f':
                    choosenProgramObject = 'f';
                    break;
                case 'q':
                case 'Q':
                    DestroyWindow(hwnd);
                    break;
                default:
                    break;
            }
            break;
        case WM_KEYDOWN:
            switch(wParam)
            {
                case 27:
                    ToggleFullScreen();
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
    void resize(int, int);
	void printGLInfo(void);
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
    ghrc = wglCreateContext(ghdc); // Windows System Integration - Attache naav , June naav - Windows Graphics Library
    if(ghrc==NULL)
        return -3;

    // Make the rendering context as current context
    if(wglMakeCurrent(ghdc,ghrc)==FALSE)
        return -4;

    // Here Starts OpenGL code
    // GLEW initialization
    if(glewInit() != GLEW_OK)
        return -5;
    // Print OpenGL Info
	printGLInfo();

    // Vertex Shader
    // Shader Source Code
    const GLchar *vertexShaderSourceCode1 = 
    "#version 460 core" \
    "\n" \
    "in vec4 a_position;\n" \
    "in vec3 a_normal;\n" \
    "uniform mat4 u_modelMatrix;\n" \
    "uniform mat4 u_viewMatrix;\n" \
    "uniform mat4 u_projectionMatrix;\n" \
    "uniform vec3 u_la[3];\n" \
    "uniform vec3 u_ld[3];" \
    "uniform vec3 u_ls[3];" \
    "uniform vec4 u_lightPosition[3];" \
    "uniform vec3 u_ka;" \
    "uniform vec3 u_kd;" \
    "uniform vec3 u_ks;" \
    "uniform float u_materialShininess;" \
    "uniform int u_lightingEnabled;" \
    "out vec3 fong_ads_light;" \
    "void main(void)" \
    "{" \
    "if(u_lightingEnabled == 1)" \
    "{" \
    "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" \
    "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" \
    "vec3 transformedNormals = normalize(normalMatrix * a_normal);\n" \
    "vec3 viewerVector = normalize(-eyeCoordinates.xyz);\n" \
    "vec3 ambient[3];\n" \
    "vec3 lightDirection[3];\n" \
    "vec3 diffuse[3];\n" \
    "vec3 specular[3];\n" \
    "vec3 reflectionVector[3];"
    "for(int i=0;i<3;i++)\n" \
    "{\n" \
    "ambient[i] = u_la[i] * u_ka;\n" \
    "lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);\n" \
    "diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i], transformedNormals), 0.0);\n" \
    "reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);\n" \
    "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininess);\n" \
    "fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];\n" \
    "}\n" \
    "}\n" \
    "else\n" \
    "{\n" \
    "fong_ads_light = vec3(1.0, 1.0, 1.0);\n" \
    "}\n" \
    "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" \
    "}";
    // Vertex Shader cha Object tayar kela
    GLuint vertexShaderObject1 = glCreateShader(GL_VERTEX_SHADER);
    // Object la Source code dila
    glShaderSource(vertexShaderObject1, 1, (const GLchar**)&vertexShaderSourceCode1, NULL);
    // GPU chya inline compiler la code compile karaila dila
    glCompileShader(vertexShaderObject1);
    // Error checking
    GLint status;
    GLint infoLogLength;
    char *log = NULL;

    glGetShaderiv(vertexShaderObject1, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject1, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObject1, infoLogLength, &written, log);
                fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", log);
                free(log);
                uninitialize();
            }
        }
    }

    // Fragment Shader
    const GLchar *fragmentShaderSourceCode1 = 
    "#version 460 core" \
    "\n" \
    "in vec3 fong_ads_light;" \
    "out vec4 FragColor;" \
    "void main(void)" \
    "{" \
    "FragColor = vec4(fong_ads_light,1.0);" \
    "}";

    GLuint fragmentShaderObject1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderObject1, 1, (const GLchar **)&fragmentShaderSourceCode1, NULL);
    glCompileShader(fragmentShaderObject1);
    // Error Checking
    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(fragmentShaderObject1, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject1, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(fragmentShaderObject1, infoLogLength, &written, log);
                fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", log);
                free(log);
                uninitialize();
            }
        }
    }

    // Shader Program Object
    shaderProgramObject1 = glCreateProgram();
    glAttachShader(shaderProgramObject1, vertexShaderObject1);
    glAttachShader(shaderProgramObject1, fragmentShaderObject1);
    glBindAttribLocation(shaderProgramObject1, SAB_ATTRIBUTE_POSITION, "a_position"); // Andhaar
    glBindAttribLocation(shaderProgramObject1, SAB_ATTRIBUTE_NORMAL, "a_normal");
    glLinkProgram(shaderProgramObject1);
    // Error Checking
    status = 0;
    infoLogLength = 0;
    log = NULL;
    glGetProgramiv(shaderProgramObject1, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject1, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject1, infoLogLength, &written, log);
                fprintf(gpFile, "Shader Program Link Log : %s\n", log);
                free(log);
                uninitialize();
            }
        }
    }
     // Shader 1 uniforms
    modelMatrixUniformV = glGetUniformLocation(shaderProgramObject1, "u_modelMatrix");
    viewMatrixUniformV = glGetUniformLocation(shaderProgramObject1, "u_viewMatrix");
    projectionMatrixUniformV = glGetUniformLocation(shaderProgramObject1, "u_projectionMatrix");
    
    laUniformV[0] = glGetUniformLocation(shaderProgramObject1, "u_la[0]");
    ldUniformV[0] = glGetUniformLocation(shaderProgramObject1, "u_ld[0]");
    lsUniformV[0] = glGetUniformLocation(shaderProgramObject1, "u_ls[0]");
    lightPositionUniformV[0] = glGetUniformLocation(shaderProgramObject1, "u_lightPosition[0]");

    laUniformV[1] = glGetUniformLocation(shaderProgramObject1, "u_la[1]");
    ldUniformV[1] = glGetUniformLocation(shaderProgramObject1, "u_ld[1]");
    lsUniformV[1] = glGetUniformLocation(shaderProgramObject1, "u_ls[1]");
    lightPositionUniformV[1] = glGetUniformLocation(shaderProgramObject1, "u_lightPosition[1]");

    laUniformV[2] = glGetUniformLocation(shaderProgramObject1, "u_la[2]");
    ldUniformV[2] = glGetUniformLocation(shaderProgramObject1, "u_ld[2]");
    lsUniformV[2] = glGetUniformLocation(shaderProgramObject1, "u_ls[2]");
    lightPositionUniformV[2] = glGetUniformLocation(shaderProgramObject1, "u_lightPosition[2]");

    kaUniformV = glGetUniformLocation(shaderProgramObject1, "u_ka");
    kdUniformV = glGetUniformLocation(shaderProgramObject1, "u_kd");
    ksUniformV = glGetUniformLocation(shaderProgramObject1, "u_ks");
    materialShininessUniformV = glGetUniformLocation(shaderProgramObject1, "u_materialShininess");

    lightingEnabledUniformV = glGetUniformLocation(shaderProgramObject1, "u_lightingEnabled");


    // Shader 2 for per fragment
    // Vertex Shader
    // Shader Source Code
    const GLchar *vertexShaderSourceCode2 = 
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
    GLuint vertexShaderObject2 = glCreateShader(GL_VERTEX_SHADER);
    // Object la Source code dila
    glShaderSource(vertexShaderObject2, 1, (const GLchar**)&vertexShaderSourceCode2, NULL);
    // GPU chya inline compiler la code compile karaila dila
    glCompileShader(vertexShaderObject2);
    // Error checking
    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(vertexShaderObject2, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject2, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObject2, infoLogLength, &written, log);
                fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", log);
                free(log);
                uninitialize();
            }
        }
    }

    // Fragment Shader
    const GLchar *fragmentShaderSourceCode2 = 
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
    "vec3 reflectionVector[3];\n"
    "vec3 normalized_transformedNormals = normalize(transformedNormals);\n" \
    "vec3 normalized_viewerVector = normalize(viewerVector);\n"
    "for(int i=0;i<3;i++)\n"
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

    GLuint fragmentShaderObject2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderObject2, 1, (const GLchar **)&fragmentShaderSourceCode2, NULL);
    glCompileShader(fragmentShaderObject2);
    // Error Checking
    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(fragmentShaderObject2, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject2, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(fragmentShaderObject2, infoLogLength, &written, log);
                fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", log);
                free(log);
                uninitialize();
            }
        }
    }

    // Shader Program Object
    shaderProgramObject2 = glCreateProgram();
    glAttachShader(shaderProgramObject2, vertexShaderObject2);
    glAttachShader(shaderProgramObject2, fragmentShaderObject2);
    glBindAttribLocation(shaderProgramObject2, SAB_ATTRIBUTE_POSITION, "a_position"); // Andhaar
    glBindAttribLocation(shaderProgramObject2, SAB_ATTRIBUTE_NORMAL, "a_normal");
    glLinkProgram(shaderProgramObject2);
    // Error Checking
    status = 0;
    infoLogLength = 0;
    log = NULL;
    glGetProgramiv(shaderProgramObject2, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject2, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if(log != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject2, infoLogLength, &written, log);
                fprintf(gpFile, "Shader Program Link Log : %s\n", log);
                free(log);
                uninitialize();
            }
        }
    }
    // Shader 2 uniforms
    modelMatrixUniformF = glGetUniformLocation(shaderProgramObject2, "u_modelMatrix");
    viewMatrixUniformF = glGetUniformLocation(shaderProgramObject2, "u_viewMatrix");
    projectionMatrixUniformF = glGetUniformLocation(shaderProgramObject2, "u_projectionMatrix");

    laUniformF[0] = glGetUniformLocation(shaderProgramObject2, "u_la[0]");
    ldUniformF[0] = glGetUniformLocation(shaderProgramObject2, "u_ld[0]");
    lsUniformF[0] = glGetUniformLocation(shaderProgramObject2, "u_ls[0]");
    lightPositionUniformF[0] = glGetUniformLocation(shaderProgramObject2, "u_lightPosition[0]");

    laUniformF[1] = glGetUniformLocation(shaderProgramObject2, "u_la[1]");
    ldUniformF[1] = glGetUniformLocation(shaderProgramObject2, "u_ld[1]");
    lsUniformF[1] = glGetUniformLocation(shaderProgramObject2, "u_ls[1]");
    lightPositionUniformF[1] = glGetUniformLocation(shaderProgramObject2, "u_lightPosition[1]");

    laUniformF[2] = glGetUniformLocation(shaderProgramObject2, "u_la[2]");
    ldUniformF[2] = glGetUniformLocation(shaderProgramObject2, "u_ld[2]");
    lsUniformF[2] = glGetUniformLocation(shaderProgramObject2, "u_ls[2]");
    lightPositionUniformF[2] = glGetUniformLocation(shaderProgramObject2, "u_lightPosition[2]");

    kaUniformF = glGetUniformLocation(shaderProgramObject2, "u_ka");
    kdUniformF = glGetUniformLocation(shaderProgramObject2, "u_kd");
    ksUniformF = glGetUniformLocation(shaderProgramObject2, "u_ks");
    materialShininessUniformF = glGetUniformLocation(shaderProgramObject2, "u_materialShininess");

    lightingEnabledUniformF = glGetUniformLocation(shaderProgramObject2, "u_lightingEnabled");
    // Default Initialization of Toggled state
    choosenProgramObject = 'v';
    // Declaration of vertex data arrays
    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    numVertices = getNumberOfSphereVertices();
    numElements = getNumberOfSphereElements();

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
    glClearColor(0.0f,0.0f,0.0f,1.0f);

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

void display(void)
{
    // Code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Transformations
    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();
    mat4 translationMatrix = translate(0.0f, 0.0f, -1.5f); 
    modelMatrix = translationMatrix;

    if(choosenProgramObject == 'v')
    {
        // Use the Shader Program Object
        glUseProgram(shaderProgramObject1);

        glUniformMatrix4fv(modelMatrixUniformV, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniformV, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectionMatrixUniformV, 1, GL_FALSE, perspectiveProjectionMatrix);
        
        if(bLight == TRUE)
        {
            glUniform1i(lightingEnabledUniformV, 1);
            glUniform3fv(kaUniformV, 1, materialAmbient);
            glUniform3fv(kdUniformV, 1, materialDiffuse);
            glUniform3fv(ksUniformV, 1, materialSpecular);
            glUniform1f(materialShininessUniformV, materialShininess);

            for(int i = 0; i < 3; i++)
            {
                glUniform3fv(laUniformV[i], 1, lights[i].lightAmbient);
                glUniform3fv(ldUniformV[i], 1, lights[i].lightDiffuse);
                glUniform3fv(lsUniformV[i], 1, lights[i].lightSpecular);
                glUniform4fv(lightPositionUniformV[i], 1, lights[i].lightPosition);
            }
        }
        else
        {
            glUniform1i(lightingEnabledUniformV, 0);
        }
    }
    else
    {
        // Use the Shader Program Object
        glUseProgram(shaderProgramObject2);

        glUniformMatrix4fv(modelMatrixUniformF, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniformF, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectionMatrixUniformF, 1, GL_FALSE, perspectiveProjectionMatrix);
        
        if(bLight == TRUE)
        {
            glUniform1i(lightingEnabledUniformF, 1);
            glUniform3fv(kaUniformF, 1, materialAmbient);
            glUniform3fv(kdUniformF, 1, materialDiffuse);
            glUniform3fv(ksUniformF, 1, materialSpecular);
            glUniform1f(materialShininessUniformF, materialShininess);

            for(int i = 0; i < 3; i++)
            {
                glUniform3fv(laUniformF[i], 1, lights[i].lightAmbient);
                glUniform3fv(ldUniformF[i], 1, lights[i].lightDiffuse);
                glUniform3fv(lsUniformF[i], 1, lights[i].lightSpecular);
                glUniform4fv(lightPositionUniformF[i], 1, lights[i].lightPosition);
            }
        }
        else
        {
            glUniform1i(lightingEnabledUniformF, 0);
        }
    }
     // *** bind vao ***
    glBindVertexArray(vao_sphere);

    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

    // *** unbind vao ***
    glBindVertexArray(0);
    // Unuse the shader program object
    glUseProgram(0);

    SwapBuffers(ghdc);
}

void update(void)
{
    // Code
    redLightAngle += 0.1f;
    if(redLightAngle >= 360.0f)
        redLightAngle -= 360.0f;
    lights[0].lightPosition = vec4(0.0f, 10.0f * cosf(redLightAngle * M_PI / 180.0f), 10.0f * sinf(redLightAngle * M_PI / 180.0f), 1.0f);

    greenLightAngle += 0.1f;
    if(greenLightAngle >= 360.0f)
        greenLightAngle -= 360.0f;
    lights[1].lightPosition = vec4(10.0f * cosf(greenLightAngle * M_PI / 180.0f), 0.0f, 10.0f * sinf(greenLightAngle * M_PI / 180.0f), 1.0f);

    blueLightAngle += 0.1f;
    if(blueLightAngle >= 360.0f)
        blueLightAngle -= 360.0f;
    lights[2].lightPosition = vec4(10.0f * cosf(blueLightAngle * M_PI / 180.0f), 10.0f * sinf(blueLightAngle * M_PI / 180.0f), 0.0f, 1.0f);

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
    if(shaderProgramObject1)
    {
        glUseProgram(shaderProgramObject1);
        GLsizei numAttachedShaders;
        glGetProgramiv(shaderProgramObject1, GL_ATTACHED_SHADERS, &numAttachedShaders);
        GLuint *shaderObjects;
        shaderObjects = (GLuint *)malloc(numAttachedShaders*sizeof(GLuint));
        glGetAttachedShaders(shaderProgramObject1, numAttachedShaders, &numAttachedShaders, shaderObjects);
        for(GLsizei i = 0; i < numAttachedShaders; i++)
        {
            glDetachShader(shaderProgramObject1, shaderObjects[i]);
            glDeleteShader(shaderObjects[i]);
            shaderObjects[i] = 0;
        }
        free(shaderObjects);
        shaderObjects = NULL;
        glUseProgram(0);
        glDeleteProgram(shaderProgramObject1);
        shaderProgramObject1 = 0;
    }
    if(shaderProgramObject2)
    {
        glUseProgram(shaderProgramObject2);
        GLsizei numAttachedShaders;
        glGetProgramiv(shaderProgramObject2, GL_ATTACHED_SHADERS, &numAttachedShaders);
        GLuint *shaderObjects;
        shaderObjects = (GLuint *)malloc(numAttachedShaders*sizeof(GLuint));
        glGetAttachedShaders(shaderProgramObject2, numAttachedShaders, &numAttachedShaders, shaderObjects);
        for(GLsizei i = 0; i < numAttachedShaders; i++)
        {
            glDetachShader(shaderProgramObject2, shaderObjects[i]);
            glDeleteShader(shaderObjects[i]);
            shaderObjects[i] = 0;
        }
        free(shaderObjects);
        shaderObjects = NULL;
        glUseProgram(0);
        glDeleteProgram(shaderProgramObject2);
        shaderProgramObject2 = 0;
    }
    if(choosenProgramObject)
    {
        choosenProgramObject = 0;
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
