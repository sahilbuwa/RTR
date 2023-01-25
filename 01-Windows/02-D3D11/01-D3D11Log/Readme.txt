single application containing
01-window with msg loop with Icon with Fullscreen with FileIO with ActiveWindow with game loop with WM_CHAR madhe escape la exit(exit on escape) with centered window.

initialize(),resize(width,height) , 
display(),update(),uninitialize() cha skeleton add kela.

80 line
88- updatewindow() kadhla ani 2 win32 api add kele
110,113 - display ani update call kele.
WM_ERASEBKGND - erase background ....
As in WM_Paint it automatically erases the background as ps struct has a erase member(fErase) TRUE.
But as this is skeleton application of OpenGL thats why no WM_PAINT and no one to erase the background.
And we dont want this message to go to defWindowProc so no break is used on 144.

case WM_SIZE add kele...ani resize call kela...
case WM_CLOSE add keli..ani DestroyWindow() call kela..
case WM_DESTROY madhe uninitialize() call kela...unintialize madhe FilePointer Close cha code taakla...

Taken from OpenGL and used as a template for DirectX.