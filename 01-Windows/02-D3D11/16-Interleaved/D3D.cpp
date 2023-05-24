// Header files
#include<windows.h>	//SDK path madhli header file declare karaichi padhhat
#include<stdio.h>	// For FileIO()
#include<stdlib.h>	// For Exit()
#include<math.h>	// For ceil()
#include"D3D.h"		//Aplya path (local) madhli header file declare karaichi padhhat
#include"XNAMath/xnamath.h"
#include"WICTextureLoader.h"	

// DirectX related header files
#include<dxgi.h>	// DirectX Graphic Infrastructure
#include<d3d11.h>	// DirectX sathi
#include<d3dcompiler.h>	// for D3DCompile()

// D3D related libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

// Namespace defined for directx
// using namespace DirectX;

// Defines
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

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
ID3D11RasterizerState *gpID3D11RasterizerState = NULL;
ID3D11DepthStencilView *gpID3D11DepthStencilView = NULL;
float clearColor[4];

ID3D11VertexShader *gpID3D11VertexShader = NULL;
ID3D11PixelShader *gpID3D11PixelShader = NULL;
ID3D11InputLayout *gpID3D11InputLayout = NULL;
ID3D11Buffer* gpID3D11Buffer_PCNT_Cube_Buffer = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11ShaderResourceView *gpID3D11ShaderResourceView_Texture_Marble = NULL;
ID3D11SamplerState *gpID3D11SamplerState_Texture = NULL;

// mvpMatrixUniform
struct CBUFFER 
{
	XMMATRIX WorldMatrix;
	XMMATRIX ViewMatrix;
	XMMATRIX ProjectionMatrix;

	XMVECTOR La;
	XMVECTOR Ld;
	XMVECTOR Ls;
	XMVECTOR LightPosition;

	XMVECTOR Ka;
	XMVECTOR Kd;
	XMVECTOR Ks;
	float MaterialShininess;

	unsigned int LightingEnabled;
};

XMMATRIX perspectiveProjectionMatrix;

float LightAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
float LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
float LightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
float LightPosition[] = {0.0f, 0.0f, -100.0f, 1.0f};

float MaterialAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
float MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
float MaterialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
float MaterialShininess = 50.0f;

BOOL bLight = FALSE;
float angleCube = 0.0f;

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

	// Initialize COM
	hr = CoInitialize(NULL);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "WinMain:CoInitialize() failed.\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "WinMain:CoInitialize() Successful.\n");
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
	HRESULT LoadD3DTexture(const wchar_t*, ID3D11ShaderResourceView**);

	// Variable Declarations
	HRESULT hr = S_OK;	// Success_Okay
	D3D_DRIVER_TYPE d3dDriverType;
	D3D_DRIVER_TYPE d3dDriverTypes[] = {D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE};
	D3D_FEATURE_LEVEL d3dFeatureLevel_Required = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL d3dFeatureLevel_Acquired = D3D_FEATURE_LEVEL_10_0;
	UINT numDrivers = 0;
	UINT createDeviceFlags = 0;
	UINT numFeatureLevels = 1;

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
		"float4x4 worldMatrix;" \
		"float4x4 viewMatrix;" \
		"float4x4 projectionMatrix;" \
		"float4 la;" \
		"float4 ld;" \
		"float4 ls;" \
		"float4 lightPosition;" \
		"float4 ka;" \
		"float4 kd;" \
		"float4 ks;" \
		"float materialShininess;" \
		"uint lightingEnabled;" \
		"}" \
		"struct vertex" \
		"{" \
		"float4 position:SV_POSITION;" \
		"float4 color:COLOR;" \
		"float3 transformedNormals:NORMAL0;" \
		"float3 lightDirection:NORMAL1;" \
		"float3 viewerVector:NORMAL2;" \
		"float2 texcoord:TEXCOORD;" \
		"};" \
		"vertex main(float4 position:POSITION, float4 color:COLOR, float4 normal:NORMAL, float2 texcoord:TEXCOORD)" \
		"{" \
		"vertex output;" \
		"if(lightingEnabled == 1)" \
		"{" \
		"float4 eyeCoordinates = mul(worldMatrix, position);" \
		"eyeCoordinates = mul(viewMatrix, eyeCoordinates);" \
		"float3x3 normalMatrix = (float3x3)worldMatrix;" \
		"output.transformedNormals = mul(normalMatrix, (float3)normal);" \
		"output.lightDirection = (float3)lightPosition - (float3)eyeCoordinates;" \
		"output.viewerVector = -eyeCoordinates.xyz;" \
		"}" \
		"float4 pos = mul(worldMatrix, position);" \
		"pos = mul(viewMatrix, pos);" \
		"pos = mul(projectionMatrix, pos);" \
		"output.color = color;" \
		"output.position = pos;" \
		"output.texcoord = texcoord;" \
		"return output;" \
		"}";

	ID3DBlob* pID3DBlob_VertexShaderCode = NULL;
	ID3DBlob* pID3DBlob_Error = NULL;

	// Compile vertex Shader
	hr = D3DCompile(vertexShaderSourceCode,lstrlenA(vertexShaderSourceCode) + 1,"VS",NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,"main","vs_5_0",0,0,&pID3DBlob_VertexShaderCode,&pID3DBlob_Error);

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

	// Pixel Shader
	const char* pixelShaderSourceCode =
		"Texture2D myTexture2D;" \
		"SamplerState mySamplerState;" \
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldMatrix;" \
		"float4x4 viewMatrix;" \
		"float4x4 projectionMatrix;" \
		"float4 la;" \
		"float4 ld;" \
		"float4 ls;" \
		"float4 lightPosition;" \
		"float4 ka;" \
		"float4 kd;" \
		"float4 ks;" \
		"float materialShininess;" \
		"uint lightingEnabled;" \
		"}" \
		"struct vertex" \
		"{" \
		"float4 position:SV_POSITION;" \
		"float4 color:COLOR;" \
		"float3 transformedNormals:NORMAL0;" \
		"float3 lightDirection:NORMAL1;" \
		"float3 viewerVector:NORMAL2;" \
		"float2 texcoord:TEXCOORD;" \
		"};" \
		"float4 main(vertex input):SV_TARGET" \
		"{" \
		"float4 color;" \
		"float3 phong_ads_light = float3(0.0, 0.0, 0.0);" \
		"if(lightingEnabled == 1)" \
		"{" \
		"float3 normalized_transformed_Normals = normalize(input.transformedNormals);" \
		"float3 normalized_lightDirection = normalize(input.lightDirection);" \
		"float3 normalized_viewerVector = normalize(input.viewerVector);" \
		"float3 ambient = la * ka;" \
		"float3 diffuse = ld * kd * max(dot(normalized_lightDirection, normalized_transformed_Normals), 0.0);" \
		"float3 reflectionVector = reflect(-normalized_lightDirection, normalized_transformed_Normals);" \
		"float3 specular = ls * ks * pow(max(dot(reflectionVector, normalized_viewerVector), 0.0), materialShininess);" \
		"phong_ads_light = ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"phong_ads_light = float3(1.0, 1.0, 1.0);" \
		"}" \
		"color = (input.color) * (float4(phong_ads_light, 1.0)) * (myTexture2D.Sample(mySamplerState, input.texcoord));" \
		"return color;" \
		"}";

	ID3DBlob* pID3DBlob_PixelShaderCode = NULL;
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
	hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderCode->GetBufferPointer(),
											pID3DBlob_PixelShaderCode->GetBufferSize(),
											NULL,
											&gpID3D11PixelShader);
	
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

	// Input layout
	D3D11_INPUT_ELEMENT_DESC d3d11InputElementDescriptor[4];
	ZeroMemory((void*)d3d11InputElementDescriptor, sizeof(D3D11_INPUT_ELEMENT_DESC) * _ARRAYSIZE(d3d11InputElementDescriptor));
	
	d3d11InputElementDescriptor[0].SemanticName = "POSITION";
	d3d11InputElementDescriptor[0].SemanticIndex = 0;
	d3d11InputElementDescriptor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3d11InputElementDescriptor[0].AlignedByteOffset = 0;
	d3d11InputElementDescriptor[0].InputSlot = 0;
	d3d11InputElementDescriptor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDescriptor[0].InstanceDataStepRate = 0;

	d3d11InputElementDescriptor[1].SemanticName = "COLOR";
	d3d11InputElementDescriptor[1].SemanticIndex = 0;
	d3d11InputElementDescriptor[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3d11InputElementDescriptor[1].AlignedByteOffset = 0;
	d3d11InputElementDescriptor[1].InputSlot = 1;
	d3d11InputElementDescriptor[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDescriptor[1].InstanceDataStepRate = 0;

	d3d11InputElementDescriptor[2].SemanticName = "NORMAL";
	d3d11InputElementDescriptor[2].SemanticIndex = 0;
	d3d11InputElementDescriptor[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3d11InputElementDescriptor[2].AlignedByteOffset = 0;
	d3d11InputElementDescriptor[2].InputSlot = 2;
	d3d11InputElementDescriptor[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDescriptor[2].InstanceDataStepRate = 0;

	d3d11InputElementDescriptor[3].SemanticName = "TEXCOORD";
	d3d11InputElementDescriptor[3].SemanticIndex = 0;
	d3d11InputElementDescriptor[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	d3d11InputElementDescriptor[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;  // OpenGL = glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	d3d11InputElementDescriptor[3].InputSlot = 3;
	d3d11InputElementDescriptor[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDescriptor[3].InstanceDataStepRate = 0;

	// Create Input layout
	// Initialize input layout structure
	hr = gpID3D11Device->CreateInputLayout(d3d11InputElementDescriptor, _ARRAYSIZE(d3d11InputElementDescriptor), pID3DBlob_VertexShaderCode->GetBufferPointer(), pID3DBlob_VertexShaderCode->GetBufferSize(), &gpID3D11InputLayout);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateInputLayout() Failed.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateInputLayout() Succeded.\n");
		fclose(gpFile);
	}

	// Set this layout in input assembly stage of pipeline
	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

	pID3DBlob_VertexShaderCode->Release();
	pID3DBlob_VertexShaderCode = NULL;
	pID3DBlob_PixelShaderCode->Release();
	pID3DBlob_PixelShaderCode = NULL;

	// Geometry
	// Declaration of vertex data arrays
	// Cube
	const float cube_pcnt[] =
	{
		// position : x,y,z    color : r,g,b          normal : x,y,z         Tex : u,v
	// SIDE 1 ( TOP )
	-1.0f, +1.0f, +1.0f,   +1.0f, +0.0f, +0.0f,   +0.0f, +1.0f, +0.0f,   +0.0f, +0.0f,
	+1.0f, +1.0f, +1.0f,   +1.0f, +0.0f, +0.0f,   +0.0f, +1.0f, +0.0f,   +0.0f, +1.0f,
	-1.0f, +1.0f, -1.0f,   +1.0f, +0.0f, +0.0f,   +0.0f, +1.0f, +0.0f,   +1.0f, +0.0f,

	-1.0f, +1.0f, -1.0f,   +1.0f, +0.0f, +0.0f,   +0.0f, +1.0f, +0.0f,   +1.0f, +0.0f,
	+1.0f, +1.0f, +1.0f,   +1.0f, +0.0f, +0.0f,   +0.0f, +1.0f, +0.0f,   +0.0f, +1.0f,
	+1.0f, +1.0f, -1.0f,   +1.0f, +0.0f, +0.0f,   +0.0f, +1.0f, +0.0f,   +1.0f, +1.0f,

	// SIDE 2 ( BOTTOM )
	+1.0f, -1.0f, -1.0f,   +0.0f, +1.0f, +0.0f,   +0.0f, -1.0f, +0.0f,   +0.0f, +0.0f,
	+1.0f, -1.0f, +1.0f,   +0.0f, +1.0f, +0.0f,   +0.0f, -1.0f, +0.0f,   +0.0f, +1.0f,
	-1.0f, -1.0f, -1.0f,   +0.0f, +1.0f, +0.0f,   +0.0f, -1.0f, +0.0f,   +1.0f, +0.0f,

	-1.0f, -1.0f, -1.0f,   +0.0f, +1.0f, +0.0f,   +0.0f, -1.0f, +0.0f,   +1.0f, +0.0f,
	+1.0f, -1.0f, +1.0f,   +0.0f, +1.0f, +0.0f,   +0.0f, -1.0f, +0.0f,   +0.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,   +0.0f, +1.0f, +0.0f,   +0.0f, -1.0f, +0.0f,   +1.0f, +1.0f,

	// SIDE 3 ( FRONT )
		-1.0f, +1.0f, -1.0f,   +0.0f, +0.0f, +1.0f,   +0.0f, +0.0f, -1.0f,   +0.0f, +0.0f,
	+1.0f, +1.0f, -1.0f,   +0.0f, +0.0f, +1.0f,   +0.0f, +0.0f, -1.0f,   +0.0f, +1.0f,
	-1.0f, -1.0f, -1.0f,   +0.0f, +0.0f, +1.0f,   +0.0f, +0.0f, -1.0f,   +1.0f, +0.0f,

	-1.0f, -1.0f, -1.0f,   +0.0f, +0.0f, +1.0f,   +0.0f, +0.0f, -1.0f,   +1.0f, +0.0f,
	+1.0f, +1.0f, -1.0f,   +0.0f, +0.0f, +1.0f,   +0.0f, +0.0f, -1.0f,   +0.0f, +1.0f,
	+1.0f, -1.0f, -1.0f,   +0.0f, +0.0f, +1.0f,   +0.0f, +0.0f, -1.0f,   +1.0f, +1.0f,

	// SIDE 4 ( BACK )
		+1.0f, -1.0f, +1.0f,   +0.0f, +1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   +0.0f, +0.0f,
	+1.0f, +1.0f, +1.0f,   +0.0f, +1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   +0.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,   +0.0f, +1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   +1.0f, +0.0f,

	-1.0f, -1.0f, +1.0f,   +0.0f, +1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   +1.0f, +0.0f,
	+1.0f, +1.0f, +1.0f,   +0.0f, +1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   +0.0f, +1.0f,
	-1.0f, +1.0f, +1.0f,   +0.0f, +1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   +1.0f, +1.0f,

	// SIDE 5 ( LEFT )
	-1.0f, +1.0f, +1.0f,   +1.0f, +0.0f, +1.0f,   -1.0f, +0.0f, +0.0f,   +0.0f, +0.0f,
	-1.0f, +1.0f, -1.0f,   +1.0f, +0.0f, +1.0f,   -1.0f, +0.0f, +0.0f,   +0.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,   +1.0f, +0.0f, +1.0f,   -1.0f, +0.0f, +0.0f,   +1.0f, +0.0f,

	-1.0f, -1.0f, +1.0f,   +1.0f, +0.0f, +1.0f,   -1.0f, +0.0f, +0.0f,   +1.0f, +0.0f,
	-1.0f, +1.0f, -1.0f,   +1.0f, +0.0f, +1.0f,   -1.0f, +0.0f, +0.0f,   +0.0f, +1.0f,
	-1.0f, -1.0f, -1.0f,   +1.0f, +0.0f, +1.0f,   -1.0f, +0.0f, +0.0f,   +1.0f, +1.0f,

	// SIDE 6 ( RIGHT )
	+1.0f, -1.0f, -1.0f,   +1.0f, +1.0f, +0.0f,   +1.0f, +0.0f, +0.0f,   +0.0f, +0.0f,
	+1.0f, +1.0f, -1.0f,   +1.0f, +1.0f, +0.0f,   +1.0f, +0.0f, +0.0f,   +0.0f, +1.0f,
	+1.0f, -1.0f, +1.0f,   +1.0f, +1.0f, +0.0f,   +1.0f, +0.0f, +0.0f,   +1.0f, +0.0f,

	+1.0f, -1.0f, +1.0f,   +1.0f, +1.0f, +0.0f,   +1.0f, +0.0f, +0.0f,   +1.0f, +0.0f,
	+1.0f, +1.0f, -1.0f,   +1.0f, +1.0f, +0.0f,   +1.0f, +0.0f, +0.0f,   +0.0f, +1.0f,
	+1.0f, +1.0f, +1.0f,   +1.0f, +1.0f, +0.0f,   +1.0f, +0.0f, +0.0f,   +1.0f, +1.0f
	};

	// Create vertex buffer for above position vertices for Cube 
	// S1 = Initialize buffer descriptor structure
	D3D11_BUFFER_DESC d3d11BufferDescriptor;
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(cube_pcnt);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// S2 = Initialize subresource data structure to put data into the buffer
	D3D11_SUBRESOURCE_DATA d3d11SubresourceData;
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	d3d11SubresourceData.pSysMem = cube_pcnt;

	// S3 = Create the actual buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubresourceData, &gpID3D11Buffer_PCNT_Cube_Buffer);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateBuffer Failed For PCNT for Cube.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateBuffer Succeded For PCNT for Cube.\n");
		fclose(gpFile);
	}
	
	// Constant Buffer
	// S1 = Initialize buffer descriptor structure
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(CBUFFER);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// S2 = Create Buffer
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, NULL, &gpID3D11Buffer_ConstantBuffer);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateBuffer Failed For Constant Buffer.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateBuffer Succeded For Constant Buffer.\n");
		fclose(gpFile);
	}

	// S3 = Set Constant Buffer in vertex shader stage of pipeline
	gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);
	// Set Constant Buffer in pixel shader stage of pipeline [ Per pixel lighting ]
	gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

	// Enabling rasterizer state
	// S1 = Initialize rasterizer descriptor
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

	// S2 = Create Rasterizer State Accordingly
	hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterizerDescriptor, &gpID3D11RasterizerState);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateRasterizerState() Failed For Constant Buffer.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateRasterizerState() Succeded For Constant Buffer.\n");
		fclose(gpFile);
	}

	// S3= Set the state into rasterizer state of pipeline
	gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);

	// Create Texture Marble
	hr = LoadD3DTexture(L"marble.bmp", &gpID3D11ShaderResourceView_Texture_Marble); // L = literal string

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "LoadD3DTexture() for Marble in initialize() Failed.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "LoadD3DTexture() for Marble in initialize() Succeded.\n");
		fclose(gpFile);
	}

	// Create texture sampler state
	D3D11_SAMPLER_DESC d3d11SamplerDescriptor;
	ZeroMemory((void*)&d3d11SamplerDescriptor, sizeof(D3D11_SAMPLER_DESC));

	d3d11SamplerDescriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3d11SamplerDescriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3d11SamplerDescriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3d11SamplerDescriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hr = gpID3D11Device->CreateSamplerState(&d3d11SamplerDescriptor, &gpID3D11SamplerState_Texture);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateSamplerState() Failed.\n");
		fclose(gpFile);

		return hr;
	}
	else
	{
		fopen_s(&gpFile, gszLogFileName, "a+");
		fprintf(gpFile, "ID3D11Device::CreateSamplerState() Succeded.\n");
		fclose(gpFile);
	}

	// Initialize clearColor array
	clearColor[0] = 1.0f;
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

// Texture Loading Function
HRESULT LoadD3DTexture(const wchar_t* textureFileName, ID3D11ShaderResourceView** ppID3D11ShaderResourceView)
{
	// Variable Declarations
	HRESULT hr = S_OK;
	// Code
	hr = DirectX::CreateWICTextureFromFile(gpID3D11Device, gpID3D11DeviceContext, textureFileName, NULL, ppID3D11ShaderResourceView);
	
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
	
	// Cube
	// Set position buffer into input assembly stage of pipeline
	UINT stride = sizeof(float) * 11;
	UINT offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 3, &gpID3D11Buffer_PCNT_Cube_Buffer, &stride, &offset);

	// Set color buffer into input assembly stage of pipeline
	stride = sizeof(float) * 11; 
	offset = sizeof(float) * 3;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 3, &gpID3D11Buffer_PCNT_Cube_Buffer, &stride, &offset);

	// Set normal buffer into input assembly stage of pipeline
	stride = sizeof(float) * 11;
	offset = sizeof(float) * 6;

	gpID3D11DeviceContext->IASetVertexBuffers(2, 3, &gpID3D11Buffer_PCNT_Cube_Buffer, &stride, &offset);

	// Set normal buffer into input assembly stage of pipeline
	stride = sizeof(float) * 11;
	offset = sizeof(float) * 9;

	gpID3D11DeviceContext->IASetVertexBuffers(3, 3, &gpID3D11Buffer_PCNT_Cube_Buffer, &stride, &offset);

	// Set primitive topology in input assembly state in pipeline
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Transformations
	// A. Initialize matrices
	XMMATRIX translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 5.0f);
	XMMATRIX rotationMatrixX = XMMatrixRotationX(XMConvertToRadians(angleCube));
	XMMATRIX rotationMatrixY = XMMatrixRotationY(XMConvertToRadians(angleCube));
	XMMATRIX rotationMatrixZ = XMMatrixRotationZ(XMConvertToRadians(angleCube));
	XMMATRIX scaleMatrix = XMMatrixScaling(0.75f, 0.75f, 0.75f);
	XMMATRIX rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
	XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;	// Order is important
	XMMATRIX viewMatrix = XMMatrixIdentity();

	// B. Put them into constant buffer
	CBUFFER ConstantBuffer;
	ZeroMemory((void*)&ConstantBuffer, sizeof(CBUFFER));
	ConstantBuffer.WorldMatrix = worldMatrix;
	ConstantBuffer.ViewMatrix = viewMatrix;
	ConstantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;


	// Lighting related code
	if (bLight == TRUE)
	{
		ConstantBuffer.LightingEnabled = 1;

		ConstantBuffer.La = XMVectorSet(LightAmbient[0], LightAmbient[1], LightAmbient[2], LightAmbient[3]);
		ConstantBuffer.Ld = XMVectorSet(LightDiffuse[0], LightDiffuse[1], LightDiffuse[2], LightDiffuse[3]);
		ConstantBuffer.Ls = XMVectorSet(LightSpecular[0], LightSpecular[1], LightSpecular[2], LightSpecular[3]);
		ConstantBuffer.LightPosition = XMVectorSet(LightPosition[0], LightPosition[1], LightPosition[2], LightPosition[3]);

		ConstantBuffer.Ka = XMVectorSet(MaterialAmbient[0], MaterialAmbient[1], MaterialAmbient[2], MaterialAmbient[3]);
		ConstantBuffer.Kd = XMVectorSet(MaterialDiffuse[0], MaterialDiffuse[1], MaterialDiffuse[2], MaterialDiffuse[3]);
		ConstantBuffer.Ks = XMVectorSet(MaterialSpecular[0], MaterialSpecular[1], MaterialSpecular[2], MaterialSpecular[3]);
		ConstantBuffer.MaterialShininess = MaterialShininess;
	}
	else
	{
		ConstantBuffer.LightingEnabled = 0;
	}

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &ConstantBuffer, 0, 0);

	// Set shader resource view in pixel shader
	gpID3D11DeviceContext->PSSetShaderResources(0, 1, &gpID3D11ShaderResourceView_Texture_Marble);

	// Set sampler state in pixel shader
	gpID3D11DeviceContext->PSSetSamplers(0, 1, &gpID3D11SamplerState_Texture);

	// Draw The Primitive
	gpID3D11DeviceContext->Draw(6, 0);
	gpID3D11DeviceContext->Draw(6, 6);
	gpID3D11DeviceContext->Draw(6, 12);
	gpID3D11DeviceContext->Draw(6, 18);
	gpID3D11DeviceContext->Draw(6, 24);
	gpID3D11DeviceContext->Draw(6, 30);

	// Swap Buffers by presenting them 
	gpIDXGISwapChain->Present(0, 0);
}

void update(void)
{
	// Code
	angleCube += 0.05f;
	if(angleCube >= 360.0f)
		angleCube -= 360.0f;
}

void uninitialize(void)
{
	// Function Declarations
	void ToggleFullscreen(void);
	// Code
	if(gpID3D11SamplerState_Texture)
	{
		gpID3D11SamplerState_Texture->Release();
		gpID3D11SamplerState_Texture = NULL;
	}
	if(gpID3D11ShaderResourceView_Texture_Marble)
	{
		gpID3D11ShaderResourceView_Texture_Marble->Release();
		gpID3D11ShaderResourceView_Texture_Marble = NULL;
	}
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
	if(gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}
	if (gpID3D11Buffer_PCNT_Cube_Buffer)
	{
		gpID3D11Buffer_PCNT_Cube_Buffer->Release();
		gpID3D11Buffer_PCNT_Cube_Buffer = NULL;
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
	// Uninitialize COM
	CoUninitialize();
}
