@echo off
cls

set DESTFOLDER="..\..\SDK\S2Qt"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\Debug
mkdir %DESTFOLDER%\bin\Release

mkdir %DESTFOLDER%\include
mkdir %DESTFOLDER%\include\S2Qt

copy /Y .\src\S2QtAPI.h        %DESTFOLDER%\include\
copy /Y .\src\QtFwk\*.h        %DESTFOLDER%\include\S2Qt

copy /Y .\bin\Debug\S2Qt.dll   %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\S2Qt.lib   %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\S2Qt.pdb   %DESTFOLDER%\bin\Debug

copy /Y .\bin\Release\S2Qt.dll %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\S2Qt.lib %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\S2Qt.pdb %DESTFOLDER%\bin\Release
													
echo Done!

:EOF