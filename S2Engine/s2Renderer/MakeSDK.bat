@echo off
cls

set DESTFOLDER="..\..\SDK\s2Renderer"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\x64\Debug
mkdir %DESTFOLDER%\bin\x64\Release

mkdir %DESTFOLDER%\include
mkdir %DESTFOLDER%\include\renderer

copy /Y .\src\s2OpenGL_API.h  %DESTFOLDER%\include\
copy /Y .\src\*.h             %DESTFOLDER%\include\renderer\

copy /Y .\bin\x64\Debug\s2Renderer.dll   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\s2Renderer.lib   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\s2Renderer.pdb   %DESTFOLDER%\bin\x64\Debug

copy /Y .\bin\x64\Release\s2Renderer.dll %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\s2Renderer.lib %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\s2Renderer.pdb %DESTFOLDER%\bin\x64\Release
													
echo Done!

:EOF