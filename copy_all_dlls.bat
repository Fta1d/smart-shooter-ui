@echo off
echo Copying ALL GStreamer DLLs to the executable directory...

set EXE_DIR=E:\smart-shooter-ui\build\Desktop_Qt_6_9_0_MinGW_64_bit-Debug\debug
set GST_DIR=C:\Program Files\gstreamer\1.0\mingw_x86_64\bin
set MSYS_DIR=C:\msys64\mingw64\bin

echo Creating a backup of the existing executable just in case...
copy "%EXE_DIR%\smart-shooter-ui.exe" "%EXE_DIR%\smart-shooter-ui.exe.bak"

echo Copying ALL DLLs from GStreamer bin directory...
copy "%GST_DIR%\*.dll" "%EXE_DIR%\"

echo Copying msgpack library...
copy "%MSYS_DIR%\libmsgpackc-*.dll" "%EXE_DIR%\"

echo Copying GStreamer plugin directory structure...
echo This may take a moment...
xcopy /E /I /Y "%GST_DIR%\..\lib\gstreamer-1.0" "%EXE_DIR%\gstreamer-1.0"

echo Done!
echo.
echo Check if the following critical files exist:
if exist "%EXE_DIR%\libgobject-2.0-0.dll" (
  echo - libgobject-2.0-0.dll: FOUND
) else (
  echo - libgobject-2.0-0.dll: MISSING
)

if exist "%EXE_DIR%\libgstbase-1.0-0.dll" (
  echo - libgstbase-1.0-0.dll: FOUND
) else (
  echo - libgstbase-1.0-0.dll: MISSING
)

if exist "%EXE_DIR%\libgstreamer-1.0-0.dll" (
  echo - libgstreamer-1.0-0.dll: FOUND
) else (
  echo - libgstreamer-1.0-0.dll: MISSING
)

pause