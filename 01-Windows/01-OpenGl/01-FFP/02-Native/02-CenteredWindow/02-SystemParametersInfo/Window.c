// Header files
#include<windows.h>
#define XWIDTH 800
#define YWIDTH 600
#define taskbarHeight 40
// Global Variable Declarations
RECT rect;

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    // Variable Declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[]= TEXT("MaziWindow");

    // Code
    // Initialization of WNDCLASSEX structure
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW ;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // BLACK_BRUSH
    wndclass.hIcon = LoadIcon(NULL , IDI_APPLICATION); //ID of icon
    wndclass.hCursor = LoadCursor(NULL , IDC_ARROW); // ID of cursor
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // Register The WNDCLASSEX
    RegisterClassEx(&wndclass);
    SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
    
    // Create The Window
    hwnd = CreateWindow (   szAppName,
                            TEXT("Sahil Ajit Buwa"),
                            WS_OVERLAPPEDWINDOW,
                            ((rect.right-rect.left)/2)-(XWIDTH/2), 
                            ((rect.bottom-rect.top+taskbarHeight)/2)-(YWIDTH/2),
                            XWIDTH,
                            YWIDTH,
                            NULL,
                            NULL, //Menu Handle , refer 28
                            hInstance, 
                            NULL // Extra Maahiti dya ki
                        );

    // Show Window
    ShowWindow(hwnd, iCmdShow  ); // SW_SHOWNORMAL
    // SW_MAXIMIZE |SW_MINIMIZE |SW_HIDE (2nd parameter)
    // Window default kashi dakhav te OS che behaviour function cha 4th parameter


    // Update The Window
    UpdateWindow(hwnd);
    // mazya window la dakhavlyavr mi dilelya brush ne rangav background

    // Message Loop
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam; 
}

// Callback Function

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    TCHAR str[255];
    // Code
    switch(iMsg)
    {
        case WM_LBUTTONDOWN:
            wsprintf(str,TEXT("Resolution : %d*%d \n"),rect.right,rect.bottom);
		    MessageBox(hwnd,str,TEXT("Result"),MB_OK);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
