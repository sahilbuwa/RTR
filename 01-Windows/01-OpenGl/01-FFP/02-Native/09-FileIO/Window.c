// Header files
#include<windows.h>
#include<stdio.h> // For FileIO()
#include<stdlib.h> // For Exit()

// Global Variable Declarations
FILE *gpFile=NULL;

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

    // Create The Window
    hwnd = CreateWindow (   szAppName,
                            TEXT("Sahil Ajit Buwa"),
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, 
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            NULL, //HWND_DESKTOP pn chalto
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
    // Code
    switch(iMsg)
    {
        case WM_DESTROY:
            if(gpFile)
            {
                fprintf(gpFile,"Log File Is Successfully Closed.\n");
                fclose(gpFile);
                gpFile=NULL;
            }
            PostQuitMessage(0);
            break;
        default:
            break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
