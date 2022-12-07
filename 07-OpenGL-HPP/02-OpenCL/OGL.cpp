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

// OpenCL headers
#include<CL/opencl.h>

// OpenGL Libraries
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"opencl.lib")

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
GLuint vbo_gpu;
GLuint mvpMatrixUniform;

mat4 perspectiveProjectionMatrix;

// Sine wave related variables
const unsigned int meshWidth = 1024;
const unsigned int meshHeight = 1024;

#define MYARRAYSIZE meshWidth * meshHeight * 4

float pos[meshWidth][meshHeight][4];

float animationTime = 0.0f;
// OpenCL variables
cl_int oclResult;
cl_mem graphicsResource = NULL;
cl_context oclContext;
cl_command_queue oclCommandQueue;
cl_program oclProgram;
cl_kernel oclKernel;
BOOL onGPU = FALSE;


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
	ShowWindow(hwnd, iCmdShow); // SW_SHOWNORMAL
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
				case 'C':
				case 'c':
					onGPU = FALSE;
					break;
				case 'G':
				case 'g':
					onGPU = TRUE;
					break;
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
	void resize(int, int);
	void printGLInfo(void);
	void uninitialize(void);
	// Variable Declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex=0;
	// OpenCL local variables
	cl_platform_id oclPlatformID;
	unsigned int dev_count;
	cl_device_id *oclDeviceIDs = NULL;
	cl_device_id oclDeviceID;
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

	// OpenCL Initialization
	// Get platform ID
	oclResult = clGetPlatformIDs(1, &oclPlatformID, NULL);
	if(oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetPlatformIDs() failed.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	// Get GPU device ID
	// 1. Get total GPU device count
	oclResult = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 0, NULL, &dev_count);
	if(oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetDeviceIDs() failed to get device count.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	else if(dev_count == 0)
	{
		fprintf(gpFile, "GPU device count is 0.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	// 2. Create memory for the array of device IDs for dev_count.
	oclDeviceIDs = (cl_device_id *)malloc(sizeof(cl_device_id) * dev_count);
	// 3. Fill the device IDs array
	oclResult = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, dev_count, oclDeviceIDs, NULL);
	if(oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetDeviceIDs() failed to get deviceIDs.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	// 4. Take the 0th device from that array as selected device
	oclDeviceID = oclDeviceIDs[0];
	// 5. Free the memory
	free(oclDeviceIDs);
	// Create OpenCL context for the selected gpu
	// 1.Create context properties array
	cl_context_properties oclContextProperties[] = 
	{	
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)oclPlatformID,
		0
	};
	// 2.Create the actual OpenCL context
	oclContext = clCreateContext(oclContextProperties, 1, &oclDeviceID, NULL, NULL, &oclResult);
	if(oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateContext() failed.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	// Create command queue
	oclCommandQueue = clCreateCommandQueueWithProperties(oclContext, oclDeviceID, 0, &oclResult);
	if(oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateCommandQueueWithProperties() failed.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	// Create OpenCL program from OpenCL kernel source code
	// 1. Write OpenCL kernel source code
	const char* oclKernelSourceCode = 
	"__kernel void sineWaveKernel(__global float4 *position, unsigned int width, unsigned int height, float time)\n" \
	"{" \
		"unsigned int i = get_global_id(0);" \
		"unsigned int j = get_global_id(1);" \
		"float u = float(i) / float(width);" \
		"float v = float(j) / float(height);" \
		"u = u * 2.0f - 1.0f;" \
		"v = v * 2.0f - 1.0f;" \
		"float frequency = 4.0f;" \
		"float w = sinf(u * frequency + time) * cosf(v * frequency + time) * 0.5f;" \
		"position[j * width + i] = float4(u, w, v, 1.0f);" \
	"}";

	// Vertex Shader
	// Shader Source Code
	const GLchar *vertexShaderSourceCode = 
	"#version 460 core" \
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
			}
		}
	}

	// Fragment Shader
	const GLchar *fragmentShaderSourceCode = 
	"#version 460 core" \
	"\n" \
	"out vec4 FragColor;" \
	"void main(void)" \
	"{" \
	"FragColor = vec4(1.0, 0.5, 0.0, 1.0);" \
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
	// Declaration of vertex data arrays
	for (unsigned int i = 0; i < meshWidth; i++)
	{
		for (unsigned int j = 0 ; j < meshHeight; j++)
		{
			for (unsigned int k = 0; k < 4; k++)
			{
				pos[i][j][k] = 0.0f;
			}
		}
	}
	// Vao and Vbo related code
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, MYARRAYSIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void display(void)
{
	// Function Declarations
	void sineWave(unsigned int, unsigned int, float);
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Use the Shader Program Object
	glUseProgram(shaderProgramObject);
	
	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	translationMatrix = translate(0.0f, 0.0f, 0.0f); 
	modelViewMatrix = translationMatrix;  
	
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	
	glBindVertexArray(vao);

	sineWave(meshWidth, meshHeight, animationTime);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, MYARRAYSIZE * sizeof(float), pos, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glDrawArrays(GL_POINTS, 0, meshWidth * meshHeight);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Unuse the shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void sineWave(unsigned int width, unsigned int height, float time)
{
	// Code
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			for (unsigned int k = 0; k < 4; k++)
			{
				float u = float(i) / float(width);
				float v = float(j) / float(height);

				u = u * 2.0f - 1.0f;
				v = v * 2.0f - 1.0f;
				float frequency = 4.0f;
				float w = sinf(u * frequency + time) * cosf(v * frequency + time) * 0.5f;

				if(k == 0)
					pos[i][j][k] = u;
				if(k == 1)
					pos[i][j][k] = w;
				if(k == 2)
					pos[i][j][k] = v;
				if(k == 3)
					pos[i][j][k] = 1.0f;
			}
		}
	}
}

void update(void)
{
	// Code
	animationTime = animationTime + 0.01f;
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
	if(vbo_gpu)
	{
		glDeleteBuffers(1, &vbo_gpu);
		vbo_gpu = 0;
	}
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
