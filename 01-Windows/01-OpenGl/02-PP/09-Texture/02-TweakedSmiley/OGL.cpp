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
GLuint vbo_position;
GLuint vbo_texcoord;

GLuint mvpMatrixUniform;
GLuint textureSamplerUniform;
GLuint keypressUniform;

GLuint texture_smiley;
int keyPressed = -1;

mat4 perspectiveProjectionMatrix;

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
	else if(iRetval==-6)
	{
		fprintf(gpFile,"Smiley Texture failed.\n");
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
				// update();

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
				case 49:
					keyPressed = 1;
					break;
				case 50:
					keyPressed = 2;
					break;
				case 51:
					keyPressed = 3;
					break;
				case 52:
					keyPressed = 4;
					break;
				default:
					keyPressed = 0;
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
	BOOL LoadGLTexture(GLuint* ,TCHAR[]);
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
	
	if(LoadGLTexture(&texture_smiley, MAKEINTRESOURCE(IDBITMAP_SMILEY))==FALSE)
	{
		fprintf(gpFile,"LoadGLTexture for stone Failed.\n");
		uninitialize();
		return -6;
	}
	// Print OpenGL Info
	printGLInfo();

	// Vertex Shader
	// Shader Source Code
	const GLchar *vertexShaderSourceCode = 
	"#version 460 core" \
	"\n" \
	"in vec4 a_position;" \
	"in vec2 a_texcoord;" \
	"uniform mat4 u_mvpMatrix;" \
	"out vec2 a_texcoord_out;" \
	"void main(void)" \
	"{" \
	"gl_Position = u_mvpMatrix * a_position;" \
	"a_texcoord_out = a_texcoord;" \
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
	"in vec2 a_texcoord_out;" \
	"uniform sampler2D u_textureSampler;" \
	"uniform int u_keyPressed;" \
	"out vec4 FragColor;" \
	"void main(void)" \
	"{" \
	"if(u_keyPressed == 1)" \
	"{" \
	"FragColor = texture(u_textureSampler, a_texcoord_out);" \
	"}" \
	"else" \
	"{" \
	"FragColor = vec4(1.0, 1.0, 1.0, 1.0);" \
	"}" \
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
	glBindAttribLocation(shaderProgramObject, SAB_ATTRIBUTE_TEXTURE0, "a_texcoord"); // Andhaar
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
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");
	keypressUniform = glGetUniformLocation(shaderProgramObject, "u_keyPressed");
	
	// Declaration of vertex data arrays
	const GLfloat position[] =
	{
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	// pyramid
	// Vao related code
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// vbo for position
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for texture
	glGenBuffers(1, &vbo_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(SAB_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(SAB_ATTRIBUTE_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Clear the screen using black color
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	// Depth Related Changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Enabling the texture
	glEnable(GL_TEXTURE_2D);

	perspectiveProjectionMatrix = mat4::identity();
	// Warmup Resize Call
	resize(WIN_WIDTH,WIN_HEIGHT);
	return 0;
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
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Create The Texture
		// gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		DeleteObject(hBitmap);
	}
	return bResult;
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
	translationMatrix = translate(0.0f, 0.0f, -6.0f);
	modelViewMatrix = translationMatrix;   // Order is very important. (Matrix multiplication is not commutative.)

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_smiley);
	glUniform1i(textureSamplerUniform, 0);

	glBindVertexArray(vao);

	GLfloat texcoord[8];
	if(keyPressed == 1)
	{
		texcoord[0] = 0.5f;
		texcoord[1] = 0.5f;
		texcoord[2] = 0.0f;
		texcoord[3] = 0.5f;
		texcoord[4] = 0.0f;
		texcoord[5] = 0.0f;
		texcoord[6] = 0.5f;
		texcoord[7] = 0.0f;
		glUniform1i(keypressUniform, 1);
	}
	else if(keyPressed == 2)
	{
		texcoord[0] = 1.0f;
		texcoord[1] = 1.0f;
		texcoord[2] = 0.0f;
		texcoord[3] = 1.0f;
		texcoord[4] = 0.0f;
		texcoord[5] = 0.0f;
		texcoord[6] = 1.0f;
		texcoord[7] = 0.0f;
		glUniform1i(keypressUniform, 1);
	}
	else if(keyPressed == 3)
	{
		texcoord[0] = 2.0f;
		texcoord[1] = 2.0f;
		texcoord[2] = 0.0f;
		texcoord[3] = 2.0f;
		texcoord[4] = 0.0f;
		texcoord[5] = 0.0f;
		texcoord[6] = 2.0f;
		texcoord[7] = 0.0f;
		glUniform1i(keypressUniform, 1);
	}
	else if(keyPressed == 4)
	{
		texcoord[0] = 0.5f;
		texcoord[1] = 0.5f;
		texcoord[2] = 0.5f;
		texcoord[3] = 0.5f;
		texcoord[4] = 0.5f;
		texcoord[5] = 0.5f;
		texcoord[6] = 0.5f;
		texcoord[7] = 0.5f;
		glUniform1i(keypressUniform, 1);
	}
	else
	{
		glUniform1i(keypressUniform, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Here there should be draw code (12 lakh)
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
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
	if(texture_smiley)
	{
		glDeleteTextures(1, &texture_smiley);
		texture_smiley = 0;
	}
	// Deletion and uninitialization of vbo_texcoords
	if(vbo_texcoord)
	{
		glDeleteBuffers(1, &vbo_texcoord);
		vbo_texcoord = 0;
	}
	// Deletion and uninitialization of vbo_position
	if(vbo_position)
	{
		glDeleteBuffers(1, &vbo_position);
		vbo_position = 0;
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
