@echo off
setlocal

echo Compiling PixelToaster with GPU support...
echo ======================================

:: Set compiler options
set CL=/EHsc /W3 /DPIXELTOASTER_DYNAMIC /DUNICODE /D_UNICODE
set LINK=/LIBPATH:"%VULKAN_SDK%\Lib" vulkan-1.lib user32.lib gdi32.lib

:: Create build directory if it doesn't exist
if not exist "build" mkdir build

:: Compile the library
echo Compiling library files...
cl %CL% /c PixelToaster.cpp PixelToasterWindows.cpp PixelToasterConversion.cpp PixelToasterGPU.cpp PixelToasterVulkan.cpp /I.

:: Create the library
echo Creating library...
lib PixelToaster.obj PixelToasterWindows.obj PixelToasterConversion.obj PixelToasterGPU.obj PixelToasterVulkan.obj /OUT:build\PixelToaster.lib

:: Compile the GPU example
echo Compiling GPU example...
cl %CL% GPUExample.cpp /I. /link %LINK% /OUT:build\GPUExample.exe PixelToaster.lib

:: Clean up
del *.obj

echo ======================================
echo Build completed successfully!
echo GPUExample.exe is in the build directory.

endlocal
pause
