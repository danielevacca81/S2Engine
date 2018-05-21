@echo off
cls

set DESTFOLDER="..\..\SDK\S2CAD"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\Debug
mkdir %DESTFOLDER%\bin\Release

mkdir %DESTFOLDER%\include\CADTools
copy /Y .\src\CADTools\*.h        %DESTFOLDER%\include\CADTools

mkdir %DESTFOLDER%\include\Selection
copy /Y .\src\Selection\*.h     %DESTFOLDER%\include\Selection

mkdir %DESTFOLDER%\include\VObjects
copy /Y .\src\VObjects\*.h     %DESTFOLDER%\include\VObjects

copy /Y .\src\S2CAD_API.h  %DESTFOLDER%\include\


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