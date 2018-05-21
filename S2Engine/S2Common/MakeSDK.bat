@echo off
cls

set DESTFOLDER="..\..\SDK\S2Common"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\Debug
mkdir %DESTFOLDER%\bin\Release

mkdir %DESTFOLDER%\include\Common
copy /Y .\src\Common\Color.h        %DESTFOLDER%\include\Common
copy /Y .\src\Common\Strings.h      %DESTFOLDER%\include\Common
copy /Y .\src\Common\TimedBlock.h   %DESTFOLDER%\include\Common
copy /Y .\src\Common\Types.h        %DESTFOLDER%\include\Common
copy /Y .\src\Common\Utils.h        %DESTFOLDER%\include\Common

mkdir %DESTFOLDER%\include\Design
copy /Y .\src\Design\*.h     %DESTFOLDER%\include\Design

mkdir %DESTFOLDER%\include\Geometry
copy /Y .\src\Geometry\*.h     %DESTFOLDER%\include\Geometry

mkdir %DESTFOLDER%\include\Math
copy /Y .\src\Math\*.h     %DESTFOLDER%\include\Math

mkdir %DESTFOLDER%\include\Resources
copy /Y .\src\Resources\R.h             %DESTFOLDER%\include\Resources

mkdir %DESTFOLDER%\include\Threads
copy /Y .\src\Threads\*.h               %DESTFOLDER%\include\Threads

mkdir %DESTFOLDER%\include\Win32
copy /Y .\src\Win32\MemCheck.h          %DESTFOLDER%\include\Win32
copy /Y .\src\Win32\EmbeddedResources.h %DESTFOLDER%\include\Win32
copy /Y .\src\Win32\Win32.h             %DESTFOLDER%\include\Win32


copy /Y .\src\S2CommonAPI.h  %DESTFOLDER%\include\


copy /Y .\bin\Release\*.lib     %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\*.dll     %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\*.pdb     %DESTFOLDER%\bin\Release

copy /Y .\bin\Debug\*.lib     %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\*.dll     %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\*.pdb     %DESTFOLDER%\bin\Debug

pause

rem pause
echo Done!

:EOF