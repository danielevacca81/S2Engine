@echo off
cls

set DESTFOLDER="..\..\SDK\s2Renderer"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\x64\Debug
mkdir %DESTFOLDER%\bin\x64\Release

mkdir %DESTFOLDER%\include
mkdir %DESTFOLDER%\include\view

copy /Y .\src\s2Renderer_API.h  %DESTFOLDER%\include\
rem copy /Y .\src\renderer\*.h     %DESTFOLDER%\include\renderer
rem copy /Y .\src\Utils\*.h        %DESTFOLDER%\include\Utils
copy /Y .\src\View\*.h         %DESTFOLDER%\include\view
rem copy /Y .\src\VObjects\*.h     %DESTFOLDER%\include\VObjects

copy /Y .\bin\x64\Debug\s2Renderer.dll   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\s2Renderer.lib   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\s2Renderer.pdb   %DESTFOLDER%\bin\x64\Debug

copy /Y .\bin\x64\Release\s2Renderer.dll %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\s2Renderer.lib %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\s2Renderer.pdb %DESTFOLDER%\bin\x64\Release
													
echo Done!

:EOF