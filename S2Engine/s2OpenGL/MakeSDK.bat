@echo off
cls

set DESTFOLDER="..\..\SDK\s2OpenGL"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\x64\Debug
mkdir %DESTFOLDER%\bin\x64\Release

mkdir %DESTFOLDER%\include
mkdir %DESTFOLDER%\include\opengl

copy /Y .\src\s2OpenGL_API.h  %DESTFOLDER%\include\
copy /Y .\src\*.h             %DESTFOLDER%\include\opengl\

copy /Y .\bin\x64\Debug\s2OpenGL.dll   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\s2OpenGL.lib   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\s2OpenGL.pdb   %DESTFOLDER%\bin\x64\Debug

copy /Y .\bin\x64\Release\s2OpenGL.dll %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\s2OpenGL.lib %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\s2OpenGL.pdb %DESTFOLDER%\bin\x64\Release
													
echo Done!

:EOF