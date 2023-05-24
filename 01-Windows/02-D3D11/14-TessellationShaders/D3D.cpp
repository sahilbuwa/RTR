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

// Global Variable Declarations
HWND ghwnd = NULL;
BOOL gbFullScreen = FALSE;
FILE *gpFile = NULL;
const char gszLogFileName[] = "Log.txt";
BOOL gbActiveWindow = FALSE;

// D3D11 Related Global Variables
IDXGISwapChain* gpIDXGISwapChain = NULL;
ID3D11Device* gpID3D11Device = NULL;
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL;	
float clearColor[4];

ID3D11VertexShader* gpID3D11VertexShader = NULL;
ID3D11PixelShader* gpID3D11PixelShader = NULL; // Fragment Shader
ID3D11HullShader* gpID3D11HullShader = NULL; // Tessellation control
ID3D11DomainShader* gpID3D11DomainShader = NULL; // Tessellation evaluation
ID3D11InputLayout* gpID3D11InputLayout = NULL;
ID3D11Buffer* gpID3D11Buffer_Position_Buffer = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_HullShader = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_DomainShader = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_PixelShader = NULL;
ID3D11RasterizerState* gpID3D11RasterizerState = NULL; // To remove backface culling
ID3D11DepthStencilView* gpID3D11DepthStencilView = NULL; // For depth


struct CBUFFER_HULL_SHADER
{
	XMVECTOR Hull_Constant_Function_Params;
};

struct CBUFFER_DOMAIN_SHADER
{
	XMMATRIX WorldViewProjectionMatrix;
};

struct CBUFFER_PIXEL_SHADER
{
	XMVECTOR LineColor;
};

unsigned int guiNumberOfSegments = 1;
XMMATRIX perspectiveProjectionMatrix;


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
				case VK_UP:
					guiNumberOfSegments++;
					if (guiNumberOfSegments >= 30)
						guiNumberOfSegments = 30; // Reset
					break;
				case VK_DOWN:
					guiNumberOfSegments--;
					if (guiNumberOfSegments <= 1)
						guiNumberOfSegments = 1; // Reset
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
		"struct vertex_output" \
		"{" \
		"float4 position:POSITION;" \
		"};" \
		"vertex_output main(float2 pos:POSITION)" \
		"{" \
		"vertex_output output;" \
		"output.position = float4(pos, 0.0, 1.0);" \
		"return output;" \
		"}";

	// Compile vertex Shader
	hr = D3DCompile(vertexShaderSourceCode, lstrlenA(vertexShaderSourceCode) + 1, "VS", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &pID3DBlob_VertexShaderCode, &pID3DBlob_Error);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		if (pID3DBlob_Error)
		{
			fprintf(gpFile, "D3DCompile() Failed For Vertex Shader : %s\n", (char*)pID3DBlob_Error->GetBufferPointer());
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
		}
		else
		{
			fprintf(gpFile, "D3DCompile() Failed For Vertex Shader : Unknown\n");
		}
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "D3DCompile() Succeded For Vertex Shader.\n");
		fclose(gpFile);
	}
	
	// Create Vertex Shader
	hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VertexShaderCode->GetBufferPointer(), 
											pID3DBlob_VertexShaderCode->GetBufferSize(),
											NULL,
											&gpID3D11VertexShader);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "CreateVertexShader() Failed.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "CreateVertexShader() Succeded.\n");
		fclose(gpFile);
	}

	// Set this vertex shader in vertex shader stage in vertex shader stage of pipeline
	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);

	if (pID3DBlob_Error)
	{
		pID3DBlob_Error->Release();
		pID3DBlob_Error = NULL;
	}

	// Hull Shader
	const char* hullShaderSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4 hull_constant_function_param;" \
		"}" \
		"struct vertex_output" \
		"{" \
		"float4 position:POSITION;" \
		"};" \
		"struct hullConstantOutput" \
		"{" \
		"float edges[2]:SV_TESSFACTOR;" \
		"};" \
		"hullConstantOutput hullConstantFunction(void)" \
		"{" \
		"hullConstantOutput output;" \
		"float numberOfStrips = hull_constant_function_param[0];" \
		"float numberOfSegments = hull_constant_function_param[1];" \
		"output.edges[0] = numberOfStrips;" \
		"output.edges[1] = numberOfSegments;" \
		"return output;" \
		"}" \
		"struct hullOutput" \
		"{" \
		"float4 position:POSITION;" \
		"};" \
		"[domain(\"isoline\")]" \
		"[partitioning(\"integer\")]" \
		"[outputtopology(\"line\")]" \
		"[outputcontrolpoints(4)]" \
		"[patchconstantfunc(\"hullConstantFunction\")]" \
		"hullOutput main(InputPatch<vertex_output,4> inputPatch, uint i:SV_OUTPUTCONTROLPOINTID)" \
		"{" \
		"hullOutput output;" \
		"output.position = inputPatch[i].position;" \
		"return output;" \
		"}";

	ID3DBlob* pID3DBlob_HullShaderCode = NULL;
	pID3DBlob_Error = NULL;

	// Compile Hull Shader
	hr = D3DCompile(hullShaderSourceCode, lstrlenA(hullShaderSourceCode) + 1, "HS", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "hs_5_0", 0, 0, &pID3DBlob_HullShaderCode, &pID3DBlob_Error);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		if (pID3DBlob_Error)
		{
			fprintf(gpFile, "D3DCompile() Failed For Hull Shader : %s\n", (char*)pID3DBlob_Error->GetBufferPointer());
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
		}
		else
		{
			fprintf(gpFile, "D3DCompile() Failed For Hull Shader : Unknown\n");
		}
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "D3DCompile() Succeded For Hull Shader.\n");
		fclose(gpFile);
	}

	// Create Hull Shader
	hr = gpID3D11Device->CreateHullShader(pID3DBlob_HullShaderCode->GetBufferPointer(),
		pID3DBlob_HullShaderCode->GetBufferSize(),
		NULL,
		&gpID3D11HullShader);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "CreateHullShader() Failed.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "CreateHullShader() Succeded.\n");
		fclose(gpFile);
	}

	// Set this hull shader in hull shader stage in hull shader stage of pipeline
	gpID3D11DeviceContext->HSSetShader(gpID3D11HullShader, NULL, 0);

	if (pID3DBlob_HullShaderCode)
	{
		pID3DBlob_HullShaderCode->Release();
		pID3DBlob_HullShaderCode = NULL;
	}

	if (pID3DBlob_Error)
	{
		pID3DBlob_Error->Release();
		pID3DBlob_Error = NULL;
	}

	// Domain Shader
	const char* domainShaderSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldViewProjectionMatrix;" \
		"}" \
		"struct hullConstantOutput" \
		"{" \
		"float edges[2]:SV_TESSFACTOR;" \
		"};" \
		"struct hullOutput" \
		"{" \
		"float4 position:POSITION;" \
		"};" \
		"struct domainOutput" \
		"{" \
		"float4 position:SV_POSITION;" \
		"};" \
		"[domain(\"isoline\")]" \
		"domainOutput main(hullConstantOutput input, OutputPatch<hullOutput,4> outputPatch, float2 tessCoord:SV_DOMAINLOCATION)" \
		"{" \
		"domainOutput output;" \
		"float3 p0 = outputPatch[0].position.xyz;" \
		"float3 p1 = outputPatch[1].position.xyz;" \
		"float3 p2 = outputPatch[2].position.xyz;" \
		"float3 p3 = outputPatch[3].position.xyz;" \
		"float3 p = p0*(1-tessCoord.x)*(1-tessCoord.x)*(1-tessCoord.x) + " \
		"			p1*3*tessCoord.x*(1-tessCoord.x)*(1-tessCoord.x) +" \
		"			p2*3*tessCoord.x*tessCoord.x*(1-tessCoord.x) +" \
		"			p3*tessCoord.x*tessCoord.x*tessCoord.x;" \
		"output.position = mul(worldViewProjectionMatrix, float4(p, 1.0));" \
		"return output;" \
		"}";

	ID3DBlob* pID3DBlob_DomainShaderCode = NULL;
	pID3DBlob_Error = NULL;

	// Compile Domain Shader
	hr = D3DCompile(domainShaderSourceCode,lstrlenA(domainShaderSourceCode) + 1,"DS",NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,"main","ds_5_0",0,0,&pID3DBlob_DomainShaderCode,&pID3DBlob_Error);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		if (pID3DBlob_Error)
		{
			fprintf(gpFile, "D3DCompile() Failed For Domain Shader : %s\n", (char*)pID3DBlob_Error->GetBufferPointer());
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
		}
		else
		{
			fprintf(gpFile, "D3DCompile() Failed For Domain Shader : Unknown\n");
		}
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "D3DCompile() Succeded For Domain Shader.\n");
		fclose(gpFile);
	}

	// Create Domain Shader
	hr = gpID3D11Device->CreateDomainShader(pID3DBlob_DomainShaderCode->GetBufferPointer(),
		pID3DBlob_DomainShaderCode->GetBufferSize(),
		NULL,
		&gpID3D11DomainShader);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateDomainShader() Failed.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateDomainShader() Succeded.\n");
		fclose(gpFile);
	}

	// Set this Domain shader in pixel shader stage in pixel shader stage of pipeline
	gpID3D11DeviceContext->DSSetShader(gpID3D11DomainShader, NULL, 0);

	if (pID3DBlob_DomainShaderCode)
	{
		pID3DBlob_DomainShaderCode->Release();
		pID3DBlob_DomainShaderCode = NULL;
	}

	if (pID3DBlob_Error)
	{
		pID3DBlob_Error->Release();
		pID3DBlob_Error = NULL;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	// Pixel Shader
	const char* pixelShaderSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4 lineColor;" \
		"}" \
		"float4 main(void):SV_TARGET" \
		"{" \
		"float4 color;" \
		"color = lineColor;" \
		"return color;" \
		"}";

	pID3DBlob_Error = NULL;

	// Compile Pixel Shader
	hr = D3DCompile(pixelShaderSourceCode,lstrlenA(pixelShaderSourceCode) + 1,"PS",NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,"main","ps_5_0",0,0,&pID3DBlob_PixelShaderCode,&pID3DBlob_Error);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		if (pID3DBlob_Error)
		{
			fprintf(gpFile, "D3DCompile() Failed For Pixel Shader : %s\n", (char*)pID3DBlob_Error->GetBufferPointer());
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
		}
		else
		{
			fprintf(gpFile, "D3DCompile() Failed For Pixel Shader : Unknown\n");
		}
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "D3DCompile() Succeded For Pixel Shader.\n");
		fclose(gpFile);
	}

	// Create Pixel Shader
	hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderCode->GetBufferPointer(),pID3DBlob_PixelShaderCode->GetBufferSize(),NULL,&gpID3D11PixelShader);
	
	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreatePixelShader() Failed.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreatePixelShader() Succeded.\n");
		fclose(gpFile);
	}

	// Set this pixel shader in pixel shader stage in pixel shader stage of pipeline
	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);

	// Create Input Layout
	// Initialize input layout structure
	D3D11_INPUT_ELEMENT_DESC d3d11InputElementDescriptor[1];
	ZeroMemory((void*)d3d11InputElementDescriptor, sizeof(D3D11_INPUT_ELEMENT_DESC) * _ARRAYSIZE(d3d11InputElementDescriptor));
	d3d11InputElementDescriptor[0].SemanticName = "POSITION";
	d3d11InputElementDescriptor[0].SemanticIndex = 0;
	d3d11InputElementDescriptor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;	//RGB are symbolic for 3 floats.
	d3d11InputElementDescriptor[0].AlignedByteOffset = 0;
	d3d11InputElementDescriptor[0].InputSlot = 0;
	d3d11InputElementDescriptor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDescriptor[0].InstanceDataStepRate = 0;

	// Create Input layout
	hr = gpID3D11Device->CreateInputLayout(d3d11InputElementDescriptor, _ARRAYSIZE(d3d11InputElementDescriptor), pID3DBlob_VertexShaderCode->GetBufferPointer(), pID3DBlob_VertexShaderCode->GetBufferSize(), &gpID3D11InputLayout);
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
	const float vertices[] =
	{
		-1.0f, -1.0f,
		-0.5f, 1.0f,
		0.5f, -1.0f,
		1.0f, 1.0f
	};

	// Create vertex buffer for above position vertices
	// Position
	// A.Initialize buffer descriptor(glGenBuffer)
	D3D11_BUFFER_DESC d3d11BufferDescriptor;
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;	// GL_STATIC_DRAW similar
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(vertices);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B.Initialize subresource data structure to put data into the buffer(glBufferData)
	D3D11_SUBRESOURCE_DATA d3d11SubresourceData;
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = vertices;

	// C.Create actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubresourceData, &gpID3D11Buffer_Position_Buffer);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() failed for position buffer.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() Successful for position buffer.\n");
		fclose(gpFile);
	}

	// Constant Buffer
	// A.Initialize constant buffer descriptor
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;	// GL_STATIC_DRAW similar
	d3d11BufferDescriptor.ByteWidth = sizeof(CBUFFER_HULL_SHADER);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// B.Create actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, NULL, &gpID3D11Buffer_ConstantBuffer_HullShader);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() failed for constant buffer for hull shader.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "initialize:ID3D11Device::CreateBuffer() Successful for constant buffer for hull shader.\n");
		fclose(gpFile);
	}

	// Set Constant Buffer in Hull shader stage of pipeline
	gpID3D11DeviceContext->HSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer_HullShader);

	// Constant Buffer for Domain Shader
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(CBUFFER_DOMAIN_SHADER);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create Buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, NULL, &gpID3D11Buffer_ConstantBuffer_DomainShader);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateBuffer Failed For Constant Buffer Domain Shader.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateBuffer Succeded For Constant Buffer Domain Shader.\n");
		fclose(gpFile);
	}

	// Set Constant Buffer in Domain shader stage of pipeline
	gpID3D11DeviceContext->DSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer_DomainShader);

	// Constant Buffer for Pixel Shader
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(CBUFFER_PIXEL_SHADER);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create Buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, NULL, &gpID3D11Buffer_ConstantBuffer_PixelShader);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateBuffer Failed For Constant Buffer Pixel Shader.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateBuffer Succeded For Constant Buffer Pixel Shader.\n");
		fclose(gpFile);
	}

	// Set Constant Buffer in Pixel shader stage of pipeline
	gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer_PixelShader);

	// Enabling rasterizer state
	D3D11_RASTERIZER_DESC d3d11RasterizerDescriptor;
	ZeroMemory((void*)&d3d11RasterizerDescriptor, sizeof(D3D11_RASTERIZER_DESC));

	d3d11RasterizerDescriptor.CullMode = D3D11_CULL_NONE;
	d3d11RasterizerDescriptor.FillMode = D3D11_FILL_SOLID;
	d3d11RasterizerDescriptor.FrontCounterClockwise = false;
	d3d11RasterizerDescriptor.MultisampleEnable = false;
	d3d11RasterizerDescriptor.ScissorEnable = false;
	d3d11RasterizerDescriptor.DepthClipEnable = true;
	d3d11RasterizerDescriptor.AntialiasedLineEnable = false;
	d3d11RasterizerDescriptor.DepthBias = 0;
	d3d11RasterizerDescriptor.DepthBiasClamp = 0.0f;
	d3d11RasterizerDescriptor.SlopeScaledDepthBias = false;

	// Create Rasterizer State Accordingly
	hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterizerDescriptor, &gpID3D11RasterizerState);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateRasterizerState() Failed For Rasterizer state.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateRasterizerState() Succeded For Rasterizer state.\n");
		fclose(gpFile);
	}

	// S3= Set the state into rasterizer state of pipeline
	gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);


	// Initialize clearColor array
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 0.0f;
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
		fprintf(gpFile, "resize:CreateRenderTargetView() failed.\n");
		fclose(gpFile);
	}
	// Release dummy texture interface
	pID3D11Texture2D_backBuffer->Release();
	pID3D11Texture2D_backBuffer = NULL;

	// Depth
	// Release existing DSV(DepthStencilView) 
	if(gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
	}
	// Initialize texture2D descriptor
	D3D11_TEXTURE2D_DESC d3d11Texture2DDescriptor;
	ZeroMemory((void*)&d3d11Texture2DDescriptor, sizeof(D3D11_TEXTURE2D_DESC));

	d3d11Texture2DDescriptor.Width = (UINT)width;
	d3d11Texture2DDescriptor.Height = (UINT)height;
	d3d11Texture2DDescriptor.ArraySize = 1;
	d3d11Texture2DDescriptor.MipLevels = 1;
	d3d11Texture2DDescriptor.SampleDesc.Count = 1;
	d3d11Texture2DDescriptor.SampleDesc.Quality = 0;
	d3d11Texture2DDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11Texture2DDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11Texture2DDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3d11Texture2DDescriptor.CPUAccessFlags = 0;	// Default rights
	d3d11Texture2DDescriptor.MiscFlags = 0;	// No Misc Flags - Is the depth for DOF or transform depth

	// Declare a 2D texture which is going to be converted into depth buffer
	ID3D11Texture2D *pID3D11Texture2D_DepthBuffer = NULL;
	hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2DDescriptor, NULL, &pID3D11Texture2D_DepthBuffer);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "resize:CreateTexture2D() failed.\n");
		fclose(gpFile);
	}

	// Initialize DSV descriptor
	D3D11_DEPTH_STENCIL_VIEW_DESC d3d11DepthStencilViewDescriptor;
	ZeroMemory((void*)&d3d11DepthStencilViewDescriptor, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3d11DepthStencilViewDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11DepthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;	// Multi-Sampled

	// Using this descriptor create a DSV
	hr = gpID3D11Device->CreateDepthStencilView(pID3D11Texture2D_DepthBuffer, &d3d11DepthStencilViewDescriptor, &gpID3D11DepthStencilView);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "resize:CreateDepthStencilView() failed.\n");
		fclose(gpFile);
		pID3D11Texture2D_DepthBuffer->Release();
		pID3D11Texture2D_DepthBuffer = NULL;
	}

	// Release the local depth buffer
	pID3D11Texture2D_DepthBuffer->Release();
	pID3D11Texture2D_DepthBuffer = NULL;

	// 5.Set this new render target view in pipeline
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpID3D11DepthStencilView);
	
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
	gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	// Pyramid
	// Set position buffer into IA stage of pipeline
	UINT stride = sizeof(float) * 2;
	UINT offset = 0;
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_Position_Buffer, &stride, &offset);
	
	// Set primitive topology in input assembly state in pipeline
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	// Transformations
	// A. Initialize matrices
	XMMATRIX translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	XMMATRIX worldMatrix = translationMatrix;	// Order is important
	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	// Put them into constatnt buffer for Domain Shader
	CBUFFER_DOMAIN_SHADER constantBuffer_DomainShader;
	ZeroMemory((void*)&constantBuffer_DomainShader, sizeof(CBUFFER_DOMAIN_SHADER));

	// Push them into the shader
	constantBuffer_DomainShader.WorldViewProjectionMatrix = wvpMatrix;

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_DomainShader, 0, NULL, &constantBuffer_DomainShader, 0, 0);

	// Put them into constatnt buffer for Domain Shader
	CBUFFER_HULL_SHADER constantBuffer_HullShader;
	ZeroMemory((void*)&constantBuffer_HullShader, sizeof(CBUFFER_HULL_SHADER));

	// Push them into the shader
	constantBuffer_HullShader.Hull_Constant_Function_Params = XMVectorSet(1.0f, (FLOAT)guiNumberOfSegments, 0.0f, 1.0f); // strip, segment, 

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_HullShader, 0, NULL, &constantBuffer_HullShader, 0, 0);
	
	TCHAR str[255];
	wsprintf(str, TEXT("Sahil Ajit Buwa || Number Of Segments = %d\n"), guiNumberOfSegments);
	SetWindowText(ghwnd, str);

	// Put them into constatnt buffer for Pixel Shader
	CBUFFER_PIXEL_SHADER constantBuffer_PixelShader;
	ZeroMemory((void*)&constantBuffer_PixelShader, sizeof(CBUFFER_PIXEL_SHADER));

	// Push them into the shader
	constantBuffer_PixelShader.LineColor = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0);

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_PixelShader, 0, NULL, &constantBuffer_PixelShader, 0, 0);
	
	// Draw The Primitive
	gpID3D11DeviceContext->Draw(4, 0);

	// Swap buffers
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
	if(gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
	}
	if(gpID3D11RasterizerState)
	{
		gpID3D11RasterizerState->Release();
		gpID3D11RasterizerState = NULL;
	}
	if (gpID3D11Buffer_ConstantBuffer_PixelShader)
	{
		gpID3D11Buffer_ConstantBuffer_PixelShader->Release();
		gpID3D11Buffer_ConstantBuffer_PixelShader = NULL;
	}
	if (gpID3D11Buffer_ConstantBuffer_DomainShader)
	{
		gpID3D11Buffer_ConstantBuffer_DomainShader->Release();
		gpID3D11Buffer_ConstantBuffer_DomainShader = NULL;
	}
	if (gpID3D11Buffer_ConstantBuffer_HullShader)
	{
		gpID3D11Buffer_ConstantBuffer_HullShader->Release();
		gpID3D11Buffer_ConstantBuffer_HullShader = NULL;
	}
	if (gpID3D11Buffer_Position_Buffer)
	{
		gpID3D11Buffer_Position_Buffer->Release();
		gpID3D11Buffer_Position_Buffer = NULL;
	}
	if(gpID3D11InputLayout)
	{
		gpID3D11InputLayout->Release();
		gpID3D11InputLayout = NULL;
	}
	if (gpID3D11DomainShader)
	{
		gpID3D11DomainShader->Release();
		gpID3D11DomainShader = NULL;
	}
	if (gpID3D11HullShader)
	{
		gpID3D11HullShader->Release();
		gpID3D11HullShader = NULL;
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
