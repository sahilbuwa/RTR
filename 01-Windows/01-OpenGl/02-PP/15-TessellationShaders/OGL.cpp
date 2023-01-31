// Standard Header files
#include<windows.h> //SDK path madhli header file declare karaichi padhhat
#include"OGL.h"  // Aplya path (local) madhli header file declare karaichi padhhat
#include<stdio.h> // For FileIO()
#include<stdlib.h> // For Exit()
 
// OpenGL header files
#include<GL/glew.h> // This must be above gl.h inclusion. 
#include<GL/gl.h>
#include"vmath.h" // Maths (RedBook)
using namespace vmath;

// OpenGL Libraries
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib") 

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
GLuint shaderProgramObject;

enum
{
	SAB_ATTRIBUTE_POSITION = 0,
	SAB_ATTRIBUTE_COLOR,
	SAB_ATTRIBUTE_NORMAL,
	SAB_ATTRIBUTE_TEXTURE0
};

GLuint vao;
GLuint vbo;
GLuint mvpMatrixUniform;

mat4 perspectiveProjectionMatrix;

// Tesselation related variables
GLuint numberOfSegmentsUniform;
GLuint numberOfStripsUniform;
GLuint lineColorUniform;
unsigned int uiNumberOfSegments;

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
				case VK_UP:
					uiNumberOfSegments++;
					if(uiNumberOfSegments >= 30)
						uiNumberOfSegments = 30;
					break;
				case VK_DOWN:
					uiNumberOfSegments--;
					if(uiNumberOfSegments <= 1)
						uiNumberOfSegments = 1;
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
	const GLchar *vertexShaderSourceCode = 
	"#version 460 core" \
	"\n" \
	"in vec2 a_position;" \
	"void main(void)" \
	"{" \
	"gl_Position = vec4(a_position, 0.0, 1.0);" \
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
			}
		}
	}

	// Tessellation Control Shader
	// Shader Source Code
	const GLchar *tessellationControlShaderSourceCode = 
	"#version 460 core" \
	"\n" \
	"layout(vertices = 4) out;" \
	"uniform int u_numberOfSegments;" \
	"uniform int u_numberOfStrips;" \
	"void main(void)" \
	"{" \
	"gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;" \
	"gl_TessLevelOuter[0] = float(u_numberOfStrips);" \
	"gl_TessLevelOuter[1] = float(u_numberOfSegments);" \
	"}";
	// Tessellation C Shader cha Object tayar kela
	GLuint tessellationControlShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);
	// Object la Source code dila
	glShaderSource(tessellationControlShaderObject, 1, (const GLchar**)&tessellationControlShaderSourceCode, NULL);
	// GPU chya inline compiler la code compile karaila dila
	glCompileShader(tessellationControlShaderObject);
	// Error checking
	status = 0;
	infoLogLength = 0;
	log = NULL;

	glGetShaderiv(tessellationControlShaderObject, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		glGetShaderiv(tessellationControlShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(tessellationControlShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "Tessellation Control Shader Compilation Log : %s\n", log);
				free(log);
				uninitialize();
			}
		}
	}

	// Tessellation Evaluation Shader
	// Shader Source Code
	const GLchar *tessellationEvaluationShaderSourceCode = 
	"#version 460 core" \
	"\n" \
	"layout(isolines) in;" \
	"uniform mat4 u_mvpMatrix;" \
	"void main(void)" \
	"{" \
	"vec3 p0 = gl_in[0].gl_Position.xyz;" \
	"vec3 p1 = gl_in[1].gl_Position.xyz;" \
	"vec3 p2 = gl_in[2].gl_Position.xyz;" \
	"vec3 p3 = gl_in[3].gl_Position.xyz;" \
	"float u = gl_TessCoord.x;" \
	"vec3 p = p0 * (1-u) * (1-u) * (1-u) +" \
			 "p1 * 3 * u * (1-u) * (1-u) +" \
			 "p2 * 3 * u * u * (1-u) +" \
			 "p3 * u * u * u;" \
	"gl_Position = u_mvpMatrix * vec4(p, 1.0);" \
	"}";
	// Tessellation E Shader cha Object tayar kela
	GLuint tessellationEvaluationShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);
	// Object la Source code dila
	glShaderSource(tessellationEvaluationShaderObject, 1, (const GLchar**)&tessellationEvaluationShaderSourceCode, NULL);
	// GPU chya inline compiler la code compile karaila dila
	glCompileShader(tessellationEvaluationShaderObject);
	// Error checking
	status = 0;
	infoLogLength = 0;
	log = NULL;

	glGetShaderiv(tessellationEvaluationShaderObject, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		glGetShaderiv(tessellationEvaluationShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(tessellationEvaluationShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "Tessellation Evaluation Shader Compilation Log : %s\n", log);
				free(log);
				uninitialize();
			}
		}
	}

	// Fragment Shader
	const GLchar *fragmentShaderSourceCode = 
	"#version 460 core" \
	"\n" \
	"uniform vec4 u_lineColor;" \
	"out vec4 FragColor;" \
	"void main(void)" \
	"{" \
	"FragColor = u_lineColor;" \
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
			}
		}
	}

	// Shader Program Object
	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, tessellationControlShaderObject);
	glAttachShader(shaderProgramObject, tessellationEvaluationShaderObject);
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
			}
		}
	}
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
	numberOfSegmentsUniform = glGetUniformLocation(shaderProgramObject, "u_numberOfSegments");
	numberOfStripsUniform = glGetUniformLocation(shaderProgramObject, "u_numberOfStrips");
	lineColorUniform = glGetUniformLocation(shaderProgramObject, "u_lineColor");

	// Declaration of vertex data arrays
	const GLfloat vertices[] = 
	{
		-1.0f, -1.0f,
		-0.5f, 1.0f,
		0.5f, -1.0f,
		1.0f, 1.0f
	};

	// Vao and Vbo related code
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Clear the screen using black color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Depth Related Changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Our line has at least 1 geometry
	uiNumberOfSegments = 1;
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
	// Use the Shader Program Object
	glUseProgram(shaderProgramObject);
	
	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	translationMatrix = translate(0.0f, 0.0f, -4.0f); 
	modelViewMatrix = translationMatrix;  
	
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	glUniform1i(numberOfSegmentsUniform, uiNumberOfSegments);
	// Caption change of window
	TCHAR str[255];
	wsprintf(str, TEXT("Sahil Ajit Buwa - OGL Window || Number of segments = %d"), uiNumberOfSegments);
	SetWindowText(ghwnd, str);

	glUniform1i(numberOfStripsUniform, 1); // 1 strip is there.
	glUniform4fv(lineColorUniform, 1, vec4(1.0f, 1.0f, 0.0f, 1.0f));

	glBindVertexArray(vao);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	// Here there should be draw code (12 lakh)
	glDrawArrays(GL_PATCHES, 0, 4);

	glBindVertexArray(0);
	// Unuse the shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	// Code

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
	if(vbo)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	// Deletion and uninitialization of vao
	if(vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
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
