cls
del OGL.exe
del OGL.obj
del OGL.res
cl.exe -EHsc -c OGL.cpp
rc.exe OGL.rc 
link.exe OGL.obj OGL.res user32.lib gdi32.lib -SUBSYSTEM:WINDOWS

