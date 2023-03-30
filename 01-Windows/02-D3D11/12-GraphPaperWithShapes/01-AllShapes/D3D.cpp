// Header files
#include<windows.h>	//SDK path madhli header file declare karaichi padhhat
#include<stdio.h>	// For FileIO()
#include<stdlib.h>	// For Exit()
#include<math.h>	// For ceil()
#include"D3D.h"		//Aplya path (local) madhli header file declare karaichi padhhat
#include"XNAMath/xnamath.h"

// DirectX related header files
#include<dxgi.h>	// DirectX Graphic Infrastructure
#include<d3d11.h>	// DirectX sathi
#include<d3dcompiler.h>	// for D3DCompile()

// D3D related libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Defines
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define M_PI 3.141592
#define M_PI_2 1.570796

// Global Variable Declarations
HWND ghwnd = NULL;
BOOL gbFullScreen = FALSE;
FILE *gpFile = NULL;
const char gszLogFileName[] = "Log.txt";
BOOL gbActiveWindow = FALSE;

// D3D11 related global variables
IDXGISwapChain *gpIDXGISwapChain = NULL;
ID3D11Device *gpID3D11Device = NULL;
ID3D11DeviceContext *gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView *gpID3D11RenderTargetView = NULL;
float clearColor[4];

ID3D11VertexShader *gpID3D11VertexShader = NULL;
ID3D11PixelShader *gpID3D11PixelShader = NULL;
ID3D11InputLayout *gpID3D11InputLayout = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer_Square = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer_Circle = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer_Triangle = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer_Point = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer_Graph = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer_Axes = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer = NULL;

// mvpMatrixUniform
struct CBUFFER 
{
	XMMATRIX WorldViewProjectionMatrix;
	XMVECTOR Color;
};

XMMATRIX perspectiveProjectionMatrix;

float circleVertices[1080];
float graphLineVertices[480];

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
	if(fopen_s(&gpFile, gszLogFileName, "w") != 0)// fopen_s(File Open Secured)
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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// BLACK_BRUSH
	wndclass.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(MYICON));	//ID of icon , MYICON int aslyane tyala string madhe convert karnyasathi MAKEINTRESOURCE macro waparaava laagto
	wndclass.hCursor = LoadCursor(NULL , IDC_ARROW);	// ID of cursor
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE(MYICON));	//Chotu Icon sathi 

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
							NULL,	//Menu Handle , refer 28
							hInstance, 
							NULL	// Extra Maahiti dya ki
						);

	ghwnd = hwnd;
	// Initialize
	hr = initialize();
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "Initialize() failed.\n");
		fclose(gpFile);
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "Initialize() Successful.\n");
		fclose(gpFile);
	}
	// Show Window
	ShowWindow(hwnd, iCmdShow);	// SW_SHOWNORMAL
	// SW_MAXIMIZE |SW_MINIMIZE |SW_HIDE (2nd parameter)
	// Window default kashi dakhav te OS che behaviour function cha 4th parameter

	// Foregrounding and focusing the window
	SetForegroundWindow(hwnd);	// Both(hwnd,ghwnd) will do here. 
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
	HRESULT resize(int, int);
	void uninitialize(void);

	// Variable Declarations
	HRESULT hr = S_OK;

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
			if(gpID3D11DeviceContext)
			{
				hr = resize(LOWORD(lParam), HIWORD(lParam));
				if(FAILED(hr))
				{
					fopen_s(&gpFile, gszLogFileName, "a+");	// Open the file and append to the existing file.
					fprintf(gpFile, "WndProc:resize() failed.\n");
					fclose(gpFile);
					return hr;
				}
			}
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
	HRESULT resize(int, int);

	// Variable Declarations
	HRESULT hr = S_OK;	// Success_Okay
	D3D_DRIVER_TYPE d3dDriverType;
	D3D_DRIVER_TYPE d3dDriverTypes[] = {D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE};
	D3D_FEATURE_LEVEL d3dFeatureLevel_Required = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL d3dFeatureLevel_Acquired = D3D_FEATURE_LEVEL_10_0;
	UINT numDrivers = 0;
	UINT createDeviceFlags = 0;
	UINT numFeatureLevels = 1;
	ID3DBlob *pID3DBlob_VertexShaderCode = NULL;
	ID3DBlob *pID3DBlob_Error = NULL;
	ID3DBlob *pID3DBlob_PixelShaderCode = NULL;

	// Code
	hr = PrintD3DInfo();
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "initialize:PrintD3DInfo() failed.\n");
		fclose(gpFile);
	}

	// Initialize SwapChain Descriptor structure
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDescriptor;
	ZeroMemory((void*)&dxgiSwapChainDescriptor, sizeof(DXGI_SWAP_CHAIN_DESC));
	dxgiSwapChainDescriptor.BufferCount = 1;
	dxgiSwapChainDescriptor.BufferDesc.Width = WIN_WIDTH;
	dxgiSwapChainDescriptor.BufferDesc.Height = WIN_HEIGHT;
	dxgiSwapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// Unsigned Normalized
	dxgiSwapChainDescriptor.BufferDesc.RefreshRate.Numerator = 165;
	dxgiSwapChainDescriptor.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDescriptor.OutputWindow = ghwnd;
	dxgiSwapChainDescriptor.SampleDesc.Count = 1;
	dxgiSwapChainDescriptor.SampleDesc.Quality = 0;	// 0 means default
	dxgiSwapChainDescriptor.Windowed = TRUE;

	// Call D3D11CreateDeviceAndSwapChain() for required driver
	numDrivers = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);
	for(UINT i = 0; i < numDrivers; i++)
	{
		d3dDriverType = d3dDriverTypes[i];

		hr = D3D11CreateDeviceAndSwapChain(	NULL,
											d3dDriverType,
											NULL,	// Jeva khatri ahe software driver milaichi teva tya software chya DLL la LoadLibrary madhun milavlela HMODULE ikde dyava
											createDeviceFlags,
											&d3dFeatureLevel_Required,
											numFeatureLevels,
											D3D11_SDK_VERSION,
											&dxgiSwapChainDescriptor,
											&gpIDXGISwapChain,
											&gpID3D11Device,
											&d3dFeatureLevel_Acquired,
											&gpID3D11DeviceContext);
		
		if(SUCCEEDED(hr))
		{
			break;
		}
	}

	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:D3D11CreateDeviceAndSwapChain() failed.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");

		// Print Obtained Driver Type
		if(d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
		{
			fprintf(gpFile, "D3D11 obtained Hardware Driver.\n");
		}
		else if(d3dDriverType == D3D_DRIVER_TYPE_WARP)
		{
			fprintf(gpFile, "D3D11 obtained WARP Driver.\n");
		}
		else if(d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			fprintf(gpFile, "D3D11 obtained Debug Driver.\n");
		}
		else
		{
			fprintf(gpFile, "D3D11 obtained Unknown Driver.\n");
		}

		// Print obtained D3D feature level
		if(d3dFeatureLevel_Acquired == D3D_FEATURE_LEVEL_11_0)
		{
			fprintf(gpFile, "D3D11 obtained 11.0 feature level.\n");
		}
		else if(d3dFeatureLevel_Acquired == D3D_FEATURE_LEVEL_10_1)
		{
			fprintf(gpFile, "D3D11 obtained 10.1 feature level.\n");
		}
		else if(d3dFeatureLevel_Acquired == D3D_FEATURE_LEVEL_10_0)
		{
			fprintf(gpFile, "D3D11 obtained 10.0 feature level.\n");
		}
		else
		{
			fprintf(gpFile, "D3D11 obtained unknown feature level.\n");
		}

		fclose(gpFile);
	}

	// Vertex Shader
	const char* vertexShaderSourceCode = 
	"cbuffer ConstantBuffer" \
	"{" \
	"float4x4 worldViewProjectionMatrix;" \
	"float3 color;" \
	"}" \
	"float4 main(float4 pos:POSITION):SV_POSITION" \
	"{" \
	"float4 position;" \
	"position = mul(worldViewProjectionMatrix, pos);" \
	"return position;" \
	"}";

	// Compile Vertex shader
	hr = D3DCompile(vertexShaderSourceCode, lstrlenA(vertexShaderSourceCode) + 1, "VS", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &pID3DBlob_VertexShaderCode, &pID3DBlob_Error);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		if(pID3DBlob_Error)
		{
			fprintf(gpFile, "initialize:D3DCompile() failed for vertex shader: %s.\n",(char*)pID3DBlob_Error->GetBufferPointer());
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
		}
		else
		{
			fprintf(gpFile, "initialize:D3DCompile() failed for vertex shader: unknown.\n");
		}
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:D3DCompile() successful for vertex shader.\n");
		fclose(gpFile);
	}

	// Create Vertex Shader
	hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VertexShaderCode->GetBufferPointer(), pID3DBlob_VertexShaderCode->GetBufferSize(), NULL, &gpID3D11VertexShader);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateVertexShader() failed.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateVertexShader() Successful.\n");
		fclose(gpFile);
	}

	// Set this vertex shader in vertex shader stage of pipeline
	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);

	// Pixel Shader
	const char* pixelShaderSourceCode = 
	"cbuffer ConstantBuffer" \
	"{" \
	"float4x4 worldViewProjectionMatrix;" \
	"float3 color;" \
	"}" \
	"float4 main(void):SV_TARGET" \
	"{" \
	"return float4(color, 1.0);" \
	"}";

	pID3DBlob_Error = NULL;
	// Compile Pixel shader
	hr = D3DCompile(pixelShaderSourceCode, lstrlenA(pixelShaderSourceCode) + 1, "PS", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, &pID3DBlob_PixelShaderCode, &pID3DBlob_Error);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		if(pID3DBlob_Error)
		{
			fprintf(gpFile, "initialize:D3DCompile() failed for pixel shader: %s.\n",(char*)pID3DBlob_Error->GetBufferPointer());
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
		}
		else
		{
			fprintf(gpFile, "initialize:D3DCompile() failed for pixel shader: unknown.\n");
		}
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:D3DCompile() successful for pixel shader.\n");
		fclose(gpFile);
	}

	// Create Pixel Shader
	hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderCode->GetBufferPointer(), pID3DBlob_PixelShaderCode->GetBufferSize(), NULL, &gpID3D11PixelShader);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreatePixelShader() failed.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreatePixelShader() Successful.\n");
		fclose(gpFile);
	}

	// Set this pixel shader in pixel shader stage of pipeline
	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);

	// Create Input Layout
	// Initialize input layout structure
	D3D11_INPUT_ELEMENT_DESC d3d11InputElementDescriptor;
	ZeroMemory((void*)&d3d11InputElementDescriptor, sizeof(D3D11_INPUT_ELEMENT_DESC));
	d3d11InputElementDescriptor.SemanticName = "POSITION";
	d3d11InputElementDescriptor.SemanticIndex = 0;
	d3d11InputElementDescriptor.Format = DXGI_FORMAT_R32G32B32_FLOAT;	//RGB are symbolic for 3 floats.
	d3d11InputElementDescriptor.AlignedByteOffset = 0;
	d3d11InputElementDescriptor.InputSlot = 0;
	d3d11InputElementDescriptor.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDescriptor.InstanceDataStepRate = 0;

	// Create Input layout
	hr = gpID3D11Device->CreateInputLayout(&d3d11InputElementDescriptor, 1, pID3DBlob_VertexShaderCode->GetBufferPointer(), pID3DBlob_VertexShaderCode->GetBufferSize(), &gpID3D11InputLayout);
		if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateInputLayout() failed.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateInputLayout() Successful.\n");
		fclose(gpFile);
	}

	// Set this input layout in input assembly state of pipeline
	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);
	// Release vertex and pixel shader blobs
	pID3DBlob_VertexShaderCode->Release();
	pID3DBlob_VertexShaderCode = NULL;
	pID3DBlob_PixelShaderCode->Release();
	pID3DBlob_PixelShaderCode = NULL;

	// Geometry
	const float squareVertices[] =
	{
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};

	const float triangleVertices[] =
	{
		cosf(M_PI_2), sinf(M_PI_2), 0.0f,
		cosf(-(M_PI/6.0f)), sinf(-(M_PI/6.0f)) , 0.0f,
		cosf((7.0f*M_PI)/6.0f), sinf((7.0f*M_PI)/6.0f), 0.0f,
		cosf(M_PI_2), sinf(M_PI_2), 0.0f
	};
	float angle;
	for(int i = 0; i < 360; i++)
	{
		angle = i * M_PI / 180.0f;
		circleVertices[i * 3] = cosf(angle);
		circleVertices[i * 3 + 1] = sinf(angle);
		circleVertices[i * 3 + 2] = 0.0f;
	}
	int index = 0;
	for(float i = -1.25f;i <= 1.25f; i += 0.0625f)
	{
		if(i == 0.0f)
			continue;
		graphLineVertices[index * 3] = 1.25f;
		graphLineVertices[index * 3 + 1] = i; 
		graphLineVertices[index * 3 + 2] = 0.0f;
		index++;
		graphLineVertices[index * 3] = -1.25f;
		graphLineVertices[index * 3 + 1] = i;
		graphLineVertices[index * 3 + 2] = 0.0f; 
		index++;
		graphLineVertices[index * 3] = i;
		graphLineVertices[index * 3 + 1] = 1.25f; 
		graphLineVertices[index * 3 + 2] = 0.0f; 
		index++;
		graphLineVertices[index * 3] = i;
		graphLineVertices[index * 3 + 1] = -1.25f; 
		graphLineVertices[index * 3 + 2] = 0.0f; 
		index++;
	}
	const float axesVertices[] =
	{ 
		0.0f, 1.25f, 0.0f,
		0.0f, -1.25f, 0.0f,
		1.25f, 0.0f, 0.0f,
		-1.25f, 0.0f, 0.0f
	};
	const float pointVertex[] =
	{
		0.0f, 0.0f, 0.0f
	};

	// Create vertex buffer for above position vertices
	// Square
	// A.Initialize buffer descriptor(glGenBuffer)
	D3D11_BUFFER_DESC d3d11BufferDescriptor;
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;	// GL_STATIC_DRAW similar
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(squareVertices);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B.Initialize subresource data structure to put data into the buffer(glBufferData)
	D3D11_SUBRESOURCE_DATA d3d11SubresourceData;
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = squareVertices;

	// C.Create actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubresourceData, &gpID3D11Buffer_PositionBuffer_Square);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() failed for position buffer for square.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() Successful for position buffer for square.\n");
		fclose(gpFile);
	}

	// Circle
	// A.Initialize buffer descriptor(glGenBuffer)
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;	// GL_STATIC_DRAW similar
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(circleVertices);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B.Initialize subresource data structure to put data into the buffer(glBufferData)
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = circleVertices;

	// C.Create actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubresourceData, &gpID3D11Buffer_PositionBuffer_Circle);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() failed for position buffer for circle.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() Successful for position buffer for circle.\n");
		fclose(gpFile);
	}

	// Triangle
	// A.Initialize buffer descriptor(glGenBuffer)
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;	// GL_STATIC_DRAW similar
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(triangleVertices);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B.Initialize subresource data structure to put data into the buffer(glBufferData)
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = triangleVertices;

	// C.Create actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubresourceData, &gpID3D11Buffer_PositionBuffer_Triangle);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() failed for position buffer for triangle.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() Successful for position buffer for triangle.\n");
		fclose(gpFile);
	}

	// Point
	// A.Initialize buffer descriptor(glGenBuffer)
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;	// GL_STATIC_DRAW similar
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(pointVertex);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B.Initialize subresource data structure to put data into the buffer(glBufferData)
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = pointVertex;

	// C.Create actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubresourceData, &gpID3D11Buffer_PositionBuffer_Point);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() failed for position buffer for point.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() Successful for position buffer for point.\n");
		fclose(gpFile);
	}

	// Graphs
	// A.Initialize buffer descriptor(glGenBuffer)
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;	// GL_STATIC_DRAW similar
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(graphLineVertices);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B.Initialize subresource data structure to put data into the buffer(glBufferData)
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = graphLineVertices;

	// C.Create actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubresourceData, &gpID3D11Buffer_PositionBuffer_Graph);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() failed for position buffer for graph.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() Successful for position buffer for graph.\n");
		fclose(gpFile);
	}

	// Axes
	// A.Initialize buffer descriptor(glGenBuffer)
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;	// GL_STATIC_DRAW similar
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(axesVertices);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B.Initialize subresource data structure to put data into the buffer(glBufferData)
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = axesVertices;

	// C.Create actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubresourceData, &gpID3D11Buffer_PositionBuffer_Axes);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() failed for position buffer for Axes.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() Successful for position buffer for Axes.\n");
		fclose(gpFile);
	}

	// Constant Buffer
	// A.Initialize constant buffer descriptor
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;	// GL_STATIC_DRAW similar
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * sizeof(CBUFFER);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// B.Create actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, NULL, &gpID3D11Buffer_ConstantBuffer);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() failed for constant buffer.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() Successful for constant buffer.\n");
		fclose(gpFile);
	}

	// C. Set constant buffer into the pipeline
	gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);
	gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

	// Initialize clearColor array
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 0.0f;	// Blue
	clearColor[3] = 1.0f;

	perspectiveProjectionMatrix = XMMatrixIdentity();

	// Warmup Resize
	hr = resize(WIN_WIDTH, WIN_HEIGHT);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:resize() failed.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:resize() Successful.\n");
		fclose(gpFile);
	}
	return hr;
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
		fopen_s(&gpFile, gszLogFileName, "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "PrintD3DInfo:CreateDXGIFactory() failed.\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");	// Open the file and append to the existing file.
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
		fopen_s(&gpFile, gszLogFileName, "a+");	// Open the file and append to the existing file.
		fprintf(gpFile, "Graphics Card Name is %s\n", str);
		fprintf(gpFile, "Graphics Card Memory(VRAM) = %d GBs\n", (int)ceil(dxgiAdapterDescriptor.DedicatedVideoMemory / 1024.0 / 1024.0 / 1024.0));
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");	// Open the file and append to the existing file.
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

HRESULT resize(int width, int height)
{
	// Variable Declarations
	HRESULT hr = S_OK;
	ID3D11Texture2D *pID3D11Texture2D_backBuffer;
	// Code
	if(height == 0)
		height = 1;	// To avoid divided by 0 error(illegal statement) in future functions.

	// 1.Release existing RTV(RenderTargetView) 
	if(gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}
	
	// 2.Tell SwapChain to resize buffer according to new width and height
	gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// 3.Get new resized buffer from swapchain into a dummy texture
	gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pID3D11Texture2D_backBuffer);

	// 4.Create new render target view using above buffers
	hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_backBuffer, NULL, &gpID3D11RenderTargetView);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "resize() failed.\n");
		fclose(gpFile);
	}
	// Release dummy texture interface
	pID3D11Texture2D_backBuffer->Release();
	pID3D11Texture2D_backBuffer = NULL;

	// 5.Set this new render target view in pipeline
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, NULL);
	
	// Initialize viewport structure
	D3D11_VIEWPORT d3d11Viewport;
	d3d11Viewport.TopLeftX = 0.0f;
	d3d11Viewport.TopLeftY = 0.0f;
	d3d11Viewport.Width = (float)width;
	d3d11Viewport.Height = (float)height;
	d3d11Viewport.MinDepth = 0.0f;
	d3d11Viewport.MaxDepth = 1.0f;

	// Set this viewport to pipeline similar to glViewport()
	gpID3D11DeviceContext->RSSetViewports(1, &d3d11Viewport);

	// Set projection matrix
	perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width/(float)height, 0.1f, 100.0f);

	return hr;
}

void display(void)
{
	// Code
	// Clear Render Target View with clearColor similar to glClearColor()
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, clearColor);
	
	// Graph Lines
	// Set position buffer into IA stage of pipeline
	UINT stride = sizeof(float) * 3;
	UINT offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Graph, &stride, &offset);
	
	// Set primitive topology in input assembly state in pipeline
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Transformations
	// A. Initialize matrices
	XMMATRIX worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 3.25f);
	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	// B. Put them into constant buffer
	CBUFFER ConstantBuffer;
	ZeroMemory((void*)&ConstantBuffer, sizeof(CBUFFER));
	ConstantBuffer.WorldViewProjectionMatrix = wvpMatrix;
	ConstantBuffer.Color = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &ConstantBuffer, 0, 0);

	// Draw the primitive(glDrawArrays(-, 2, 1))
	gpID3D11DeviceContext->Draw(320, 0);

	// Axes
	// Set position buffer into IA stage of pipeline
	stride = sizeof(float) * 3;
	offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Axes, &stride, &offset);
	
	// Set primitive topology in input assembly state in pipeline
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// B. Put them into constant buffer
	ZeroMemory((void*)&ConstantBuffer, sizeof(CBUFFER));
	ConstantBuffer.WorldViewProjectionMatrix = wvpMatrix;
	ConstantBuffer.Color = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &ConstantBuffer, 0, 0);

	// Draw the primitive(glDrawArrays(-, 2, 1))
	gpID3D11DeviceContext->Draw(2, 0);

	ZeroMemory((void*)&ConstantBuffer, sizeof(CBUFFER));
	ConstantBuffer.WorldViewProjectionMatrix = wvpMatrix;
	ConstantBuffer.Color = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &ConstantBuffer, 0, 0);

	// Draw the primitive(glDrawArrays(-, 2, 1))
	gpID3D11DeviceContext->Draw(2, 2);

	// Square
	// Set position buffer into IA stage of pipeline
	stride = sizeof(float) * 3;
	offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Square, &stride, &offset);
	
	// Set primitive topology in input assembly state in pipeline
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// B. Put them into constant buffer
	ZeroMemory((void*)&ConstantBuffer, sizeof(CBUFFER));
	ConstantBuffer.WorldViewProjectionMatrix = wvpMatrix;
	ConstantBuffer.Color = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &ConstantBuffer, 0, 0);

	// Draw the primitive(glDrawArrays(-, 2, 1))
	gpID3D11DeviceContext->Draw(5, 0);

	// Triangle
	// Set position buffer into IA stage of pipeline
	stride = sizeof(float) * 3;
	offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Triangle, &stride, &offset);
	
	// Set primitive topology in input assembly state in pipeline
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// B. Put them into constant buffer
	ZeroMemory((void*)&ConstantBuffer, sizeof(CBUFFER));
	ConstantBuffer.WorldViewProjectionMatrix = wvpMatrix;
	ConstantBuffer.Color = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &ConstantBuffer, 0, 0);

	// Draw the primitive(glDrawArrays(-, 2, 1))
	gpID3D11DeviceContext->Draw(4, 0);

	// Circle
	// Set position buffer into IA stage of pipeline
	stride = sizeof(float) * 3;
	offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Circle, &stride, &offset);
	
	// Set primitive topology in input assembly state in pipeline
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// B. Put them into constant buffer
	ZeroMemory((void*)&ConstantBuffer, sizeof(CBUFFER));
	ConstantBuffer.WorldViewProjectionMatrix = wvpMatrix;
	ConstantBuffer.Color = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &ConstantBuffer, 0, 0);

	// Draw the primitive(glDrawArrays(-, 2, 1))
	gpID3D11DeviceContext->Draw(360, 0);

	// Point
	// Set position buffer into IA stage of pipeline
	stride = sizeof(float) * 3;
	offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Point, &stride, &offset);
	
	// Set primitive topology in input assembly state in pipeline
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// B. Put them into constant buffer
	ZeroMemory((void*)&ConstantBuffer, sizeof(CBUFFER));
	ConstantBuffer.WorldViewProjectionMatrix = wvpMatrix;
	ConstantBuffer.Color = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &ConstantBuffer, 0, 0);

	// Draw the primitive(glDrawArrays(-, 2, 1))
	gpID3D11DeviceContext->Draw(1, 0);

	// Swap Buffers by presenting them 
	gpIDXGISwapChain->Present(0, 0);
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
	if(gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}
	if(gpID3D11Buffer_PositionBuffer_Axes)
	{
		gpID3D11Buffer_PositionBuffer_Axes->Release();
		gpID3D11Buffer_PositionBuffer_Axes = NULL;
	}
	if(gpID3D11Buffer_PositionBuffer_Circle)
	{
		gpID3D11Buffer_PositionBuffer_Circle->Release();
		gpID3D11Buffer_PositionBuffer_Circle = NULL;
	}
	if(gpID3D11Buffer_PositionBuffer_Square)
	{
		gpID3D11Buffer_PositionBuffer_Square->Release();
		gpID3D11Buffer_PositionBuffer_Square = NULL;
	}
	if(gpID3D11Buffer_PositionBuffer_Graph)
	{
		gpID3D11Buffer_PositionBuffer_Graph->Release();
		gpID3D11Buffer_PositionBuffer_Graph = NULL;
	}
	if(gpID3D11Buffer_PositionBuffer_Triangle)
	{
		gpID3D11Buffer_PositionBuffer_Triangle->Release();
		gpID3D11Buffer_PositionBuffer_Triangle = NULL;
	}
	if(gpID3D11Buffer_PositionBuffer_Point)
	{
		gpID3D11Buffer_PositionBuffer_Point->Release();
		gpID3D11Buffer_PositionBuffer_Point = NULL;
	}
	if(gpID3D11InputLayout)
	{
		gpID3D11InputLayout->Release();
		gpID3D11InputLayout = NULL;
	}
	if(gpID3D11PixelShader)
	{
		gpID3D11PixelShader->Release();
		gpID3D11PixelShader = NULL;
	}
	if(gpID3D11VertexShader)
	{
		gpID3D11VertexShader->Release();
		gpID3D11VertexShader = NULL;
	}
	if(gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}
	if(gpIDXGISwapChain)
	{
		gpIDXGISwapChain->Release();
		gpIDXGISwapChain = NULL;
	}
	if(gpID3D11DeviceContext)
	{
		gpID3D11DeviceContext->Release();
		gpID3D11DeviceContext = NULL;
	}
	if(gpID3D11Device)
	{
		gpID3D11Device->Release();
		gpID3D11Device = NULL;
	}
	if(gbFullScreen)
	{
		ToggleFullScreen();
	}
	if(ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd=NULL;
	}
}
