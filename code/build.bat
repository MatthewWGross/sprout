@echo off
set INCLUDE=..\code\external\include;%INCLUDE%
set LIB=..\code\external\lib;%LIB%
set defines=-DGLEW_STATIC -D_CONSOLE
set commonCompilerFlags=-MD -nologo -Od -Oi -Gm- -GR- -EHa-  -W4 -wd4201 -wd4100 -wd4189 %defines% -Z7 -FC -Fmsprout.map
set commonLinkerFlags= -opt:ref user32.lib Gdi32.lib shell32.lib oleaut32.lib GLEW_1130_DEBUG.lib glfw3_DEBUG.lib opengl32.lib glu32.lib
set cppfiles=..\code\src\winplatform.cpp ..\code\src\main.cpp ..\code\src\shader.cpp ..\code\src\objloader.cpp

IF EXIST ..\build rm -rf ..\build
mkdir ..\build
pushd ..\build

REM 32-bit
REM cl %commonCompilerFlags% %cppfiles% /link -subsystem:windows,5.1 %commonLinkerFlags%

REM 64-bit build

cl %commonCompilerFlags%  %cppfiles% /link %commonLinkerFlags%

popd