@echo off
cls

set DESTFOLDER="..\..\SDK\s2Qt"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\x64\Debug
mkdir %DESTFOLDER%\bin\x64\Release

mkdir %DESTFOLDER%\include
mkdir %DESTFOLDER%\include\qt

copy /Y .\src\*.h                  %DESTFOLDER%\include\qt

copy /Y .\bin\x64\Debug\s2Qt.dll   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\s2Qt.lib   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\s2Qt.pdb   %DESTFOLDER%\bin\x64\Debug

copy /Y .\bin\x64\Release\s2Qt.dll %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\s2Qt.lib %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\s2Qt.pdb %DESTFOLDER%\bin\x64\Release
													
echo Done!

:EOF