@echo off
cls

set DESTFOLDER="..\..\SDK\S2Renderer"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\Debug
mkdir %DESTFOLDER%\bin\Release

mkdir %DESTFOLDER%\include
mkdir %DESTFOLDER%\include\Geometry
mkdir %DESTFOLDER%\include\Graphics
mkdir %DESTFOLDER%\include\Math
mkdir %DESTFOLDER%\include\OpenGL
mkdir %DESTFOLDER%\include\Renderer
mkdir %DESTFOLDER%\include\Scene
mkdir %DESTFOLDER%\include\Utils
mkdir %DESTFOLDER%\include\View
mkdir %DESTFOLDER%\include\VObjects

copy /Y .\src\S2RendererAPI.h  %DESTFOLDER%\include\
rem copy /Y .\src\Geometry\*.h     %DESTFOLDER%\include\Geometry
copy /Y .\src\Graphics\*.h          %DESTFOLDER%\include\Graphics
copy /Y .\src\OpenGL\*.h       %DESTFOLDER%\include\OpenGL
copy /Y .\src\Renderer\*.h     %DESTFOLDER%\include\Renderer
rem copy /Y .\src\Scene\*.h        %DESTFOLDER%\include\Scene
copy /Y .\src\Utils\*.h        %DESTFOLDER%\include\Utils
copy /Y .\src\View\*.h         %DESTFOLDER%\include\View
rem copy /Y .\src\VObjects\*.h     %DESTFOLDER%\include\VObjects

copy /Y .\bin\Debug\S2Renderer.dll   %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\S2Renderer.lib   %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\S2Renderer.pdb   %DESTFOLDER%\bin\Debug

copy /Y .\bin\Release\S2Renderer.dll %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\S2Renderer.lib %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\S2Renderer.pdb %DESTFOLDER%\bin\Release
													
echo Done!

:EOF