// Header files
#include<windows.h>	//SDK path madhli header file declare karaichi padhhat
#include<stdio.h>	// For FileIO()
#include<stdlib.h>	// For Exit()
#include<math.h>	// For ceil()
#include"D3D.h"		//Aplya path (local) madhli header file declare karaichi padhhat

// DirectX related header files
#include<dxgi.h>	// DirectX Graphic Infrastructure
#include<d3d11.h>	// DirectX sathi

// D3D related libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

// Defines
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variable Declarations
HWND ghwnd = NULL;
BOOL gbFullScreen = FALSE;
FILE *gpFile = NULL;
BOOL gbActiveWindow = FALSE;

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function Declarations
	HRESULT initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);

	// Variable Declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[]= TEXT("MaziWindow");
	BOOL bDone = FALSE;
	HRESULT hr = S_OK;

	// Code
	if(fopen_s(&gpFile, "Log.txt", "w") != 0)// fopen_s(File Open Secured)
	{
		MessageBox(NULL, TEXT("Creation of log file failed. Exiting...\n"),TEXT("FileIO Error"),MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile,"Log File Is Successfully Created. \n");
		fclose(gpFile);
	}

	// Initialization of WNDCLASSEX structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW ;
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
	hwnd = CreateWindow (	szAppName,
							TEXT("Sahil Ajit Buwa"),
							WS_OVERLAPPEDWINDOW,
							(GetSystemMetrics(SM_CXSCREEN)-WIN_WIDTH)/2, 
							(GetSystemMetrics(SM_CYSCREEN)-WIN_HEIGHT)/2,
							WIN_WIDTH,
							WIN_HEIGHT,
							NULL,
							NULL, //Menu Handle , refer 28
							hInstance, 
							NULL // Extra Maahiti dya ki
						);

	ghwnd = hwnd;
	// Initialize
	hr = initialize();
	if(FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "Initialize() failed.\n");
		fclose(gpFile);
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "Initialize() Successful.\n");
		fclose(gpFile);
	}
	// Show Window
	ShowWindow(hwnd, iCmdShow); // SW_SHOWNORMAL
	// SW_MAXIMIZE |SW_MINIMIZE |SW_HIDE (2nd parameter)
	// Window default kashi dakhav te OS che behaviour function cha 4th parameter

	// Foregrounding and focusing the window
	SetForegroundWindow(hwnd); // Both(hwnd,ghwnd) will do here. 
	SetFocus(hwnd);

	// Game Loop
	while(bDone == FALSE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if(gbActiveWindow == TRUE)
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
	void resize(int, int);
	void uninitialize(void);

	// Variable Declarations
	TCHAR str[255];

	// Code
	switch(iMsg)
	{
		case WM_SETFOCUS:
			gbActiveWindow = TRUE;
			break;
		case WM_KILLFOCUS:
			gbActiveWindow = FALSE;
			break;
		case WM_ERASEBKGND:
			break;
		// As this is not retained mode graphics, there is not WM_PAINT to paint.    
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
			resize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			uninitialize();
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
	wp.length = sizeof(WINDOWPLACEMENT);

	if(gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if(dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);

			if(GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW );
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right-mi.rcMonitor.left, mi.rcMonitor.bottom-mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); //WM_NCCalcSize non client area calculate kar ani consider kar
			}
			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED );

		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}

}

HRESULT initialize(void)
{
	// Function Declarations
	HRESULT PrintD3DInfo(void);
	void uninitialize(void);

	// Variable Declarations
	HRESULT hr = S_OK;	// Success_Okay
	// Code
	hr = PrintD3DInfo();
	if(FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "initialize:PrintD3DInfo() failed.\n");
		fclose(gpFile);
	}
	return 0;
}

HRESULT PrintD3DInfo(void)
{
	// Variable Declarations
	HRESULT hr = S_OK;	// Success_Okay
	IDXGIFactory *pIDXGIFactory = NULL;
	IDXGIAdapter *pIDXGIAdapter = NULL;
	DXGI_ADAPTER_DESC dxgiAdapterDescriptor;
	char str[255];

	// Code
	// Create DXGIFactory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "PrintD3DInfo:CreateDXGIFactory() failed.\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "PrintD3DInfo:CreateDXGIFactory() Successful.\n");
		fclose(gpFile);
	}

	// Get IDXGIAdapter
	if(pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		// Get Description of the found adaptor
		ZeroMemory((void*)&dxgiAdapterDescriptor, sizeof(DXGI_ADAPTER_DESC));
		pIDXGIAdapter->GetDesc(&dxgiAdapterDescriptor);
		WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDescriptor.Description, 255, str, 255, NULL, NULL);
		fopen_s(&gpFile, "Log.txt", "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "Graphics Card Name is %s\n", str);
		fprintf(gpFile, "Graphics Card Memory(VRAM) = %d GBs\n", (int)ceil(dxgiAdapterDescriptor.DedicatedVideoMemory / 1024.0 / 1024.0 / 1024.0));
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "PrintD3DInfo:IDXGIFactory->EnumAdapters() failed.\n");
		fclose(gpFile);
	}

	if(pIDXGIAdapter)
	{
		pIDXGIAdapter->Release();
		pIDXGIAdapter = NULL;
	}

	if(pIDXGIFactory)
	{
		pIDXGIFactory->Release();
		pIDXGIFactory = NULL;
	}
	return hr;
}

void resize(int width, int height)
{
	// Code
	if(height == 0)
		height = 1; // To avoid divided by 0 error(illegal statement) in future calls..
}

void display(void)
{
	// Code

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
	if(ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd=NULL;
	}
	if(gpFile)
	{
		fopen_s(&gpFile, "Log.txt", "a+");	// Open the file and append to the existing file.
		fprintf(gpFile,"Log File Is Successfully Closed.\n");
		fclose(gpFile);
		gpFile=NULL;
	}
}
