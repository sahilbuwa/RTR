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
#define MAX_STACK_SIZE 32

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
	SAB_ATTRIBUTE_TEXURE0
};

GLuint vao_sphere;
GLuint vbo_sphere_position;
GLuint vbo_sphere_normal;
GLuint vbo_sphere_element;

GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint projectionMatrixUniform;

GLuint sphereColorUniform;

// Sphere arrays
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
GLuint numElements;
GLuint numVertices;

mat4 perspectiveProjectionMatrix;

mat4 matrixStack[MAX_STACK_SIZE];
int top = -1;

int shoulder = 0;
int elbow = 0;
int wrist = 0;

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
				case 's':
					shoulder = (shoulder + 6) % 360;
					break;
				case 'S':
					shoulder = (shoulder - 6) % 360;
					break;
				case 'e':
					elbow = (elbow + 3) % 360;
					break;
				case 'E':
					elbow = (elbow - 3) % 360;
					break;
				case 'w':
					wrist = (wrist + 3) % 360;
					break;
				case 'W':
					wrist = (wrist - 3) % 360;
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
	void InitializeMatrixStack(void);

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
	const GLchar *vertexShaderSourceCode = "#version 460 core" \
	"\n" \
	"in vec4 a_position;" \
	"in vec4 a_normal;" \
	"uniform mat4 u_modelMatrix;" \
	"uniform mat4 u_viewMatrix;" \
	"uniform mat4 u_projectionMatrix;" \
	"void main(void)" \
	"{" \
	"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
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
	const GLchar *fragmentShaderSourceCode = "#version 460 core" \
	"\n" \
	"uniform vec3 u_color;" \
	"out vec4 FragColor;" \
	"void main(void)" \
	"{" \
	"	FragColor = vec4(u_color, 1.0);" \
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
	glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_NORMAL, "a_normal");
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
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
	sphereColorUniform = glGetUniformLocation(shaderProgramObject, "u_color");
	
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

	perspectiveProjectionMatrix = mat4::identity();
	// Matrix Stack Initialize to Identity
	InitializeMatrixStack();
	
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
	void PushMatrix(mat4 matrix);
	mat4 PopMatrix(void);
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Use the Shader Program Object
	glUseProgram(shaderProgramObject);
	
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 translationMatrix = mat4::identity(); 
	mat4 rotationMatrixZ1 = mat4::identity();
	mat4 rotationMatrixZ2 = mat4::identity();
	mat4 rotationMatrixZ3 = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// View Transformation
	viewMatrix = lookat(vec3(0.0f, 0.0f, 10.0f), 
		   				vec3(0.0f, 0.0f, 0.0f), 
		   				vec3(0.0f, 1.0f, 0.0f));
	
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	
	// Save the camera/view Matrix (Push)
	PushMatrix(viewMatrix);
	
	rotationMatrixZ1 = rotate((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);

	translationMatrix = translate(1.0f, 0.0f, 0.0f);

	scaleMatrix = scale(2.0f, 0.5f, 1.0f);
	modelMatrix =  rotationMatrixZ1 * translationMatrix;

	PushMatrix(modelMatrix);

	modelMatrix *= scaleMatrix;
	
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniform3f(sphereColorUniform, 0.5f, 0.35f, 0.05f);
	// Draw Sphere Shoulder
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	modelMatrix = PopMatrix();
	modelMatrix *= translationMatrix;
	// Rotate around sun
	rotationMatrixZ2 = rotate((GLfloat)elbow, 0.0f, 0.0f, 1.0f);

	modelMatrix *= rotationMatrixZ2 * translationMatrix;
	
	PushMatrix(modelMatrix);

	modelMatrix *= scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	modelMatrix = PopMatrix();
	translationMatrix = translate(1.0f, 0.0f, 0.0f);
	modelMatrix *= translationMatrix;
	rotationMatrixZ3 = rotate((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

	scaleMatrix = scale(1.0f, 0.5f, 1.0f);
	translationMatrix = translate(0.5f, 0.0f, 0.0f);

	modelMatrix *= rotationMatrixZ3 * translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	
	glBindVertexArray(vao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	PopMatrix();

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

// =================================================================================================================== //
// STACK IMPLEMENTATION //
// =================================================================================================================== //

void InitializeMatrixStack(void)
{
	for (int i = 0; i < MAX_STACK_SIZE; i++)
	{
		matrixStack[i] = mat4::identity();
	}
}

void PushMatrix(mat4 matrix)
{
	if(top > MAX_STACK_SIZE)
	{
		fprintf(gpFile, "Stack is full.\n");
		return;
	}
	top++;
	matrixStack[top] = matrix;

}

mat4 PopMatrix(void)
{
	if(top == 0)
	{
		fprintf(gpFile,"Stack is now empty.\n");
		return mat4::identity();
	}
	mat4 matrix = matrixStack[top];
	top--;
	return matrix;
	// Or
	// return matrixStack[top--];
}
