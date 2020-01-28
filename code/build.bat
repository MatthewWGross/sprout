@echo off
set INCLUDE=..\code\external\include;%INCLUDE%
set defines=-DGLEW_STATIC -D_CONSOLE
set commonCompilerFlags=-MD -nologo -Od -Oi -Gm- -GR- -EHa-  -W4 -wd4201 -wd4100 -wd4189 %defines% -Z7 -FC -Fmsprout.map
set commonLinkerFlags= -opt:ref user32.lib Gdi32.lib shell32.lib oleaut32.lib GLEW_1130_RELEASE.lib glfw3_RELEASE.lib opengl32.lib glu32.lib
set cppfiles=..\code\src\application.cpp ..\code\src\main.cpp ..\code\src\shader.cpp ..\code\src\window.cpp

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

REM 32-bit
REM cl %commonCompilerFlags% %cppfiles% /link -subsystem:windows,5.1 %commonLinkerFlags%

REM 64-bit build

cl %commonCompilerFlags%  %cppfiles% /link %commonLinkerFlags%

popd