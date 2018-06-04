@echo off
cls

set DESTFOLDER="..\..\SDK\Renderer"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\x64\Debug
mkdir %DESTFOLDER%\bin\x64\Release

mkdir %DESTFOLDER%\include
mkdir %DESTFOLDER%\include\opengl
mkdir %DESTFOLDER%\include\renderer
mkdir %DESTFOLDER%\include\view

copy /Y .\src\Renderer_API.h  %DESTFOLDER%\include\
copy /Y .\src\opengl\*.h       %DESTFOLDER%\include\opengl
copy /Y .\src\renderer\*.h     %DESTFOLDER%\include\renderer
rem copy /Y .\src\Utils\*.h        %DESTFOLDER%\include\Utils
rem copy /Y .\src\View\*.h         %DESTFOLDER%\include\View
rem copy /Y .\src\VObjects\*.h     %DESTFOLDER%\include\VObjects

copy /Y .\bin\x64\Debug\Renderer.dll   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\Renderer.lib   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\Renderer.pdb   %DESTFOLDER%\bin\x64\Debug

copy /Y .\bin\x64\Release\Renderer.dll %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\Renderer.lib %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\Renderer.pdb %DESTFOLDER%\bin\x64\Release
													
echo Done!

:EOF