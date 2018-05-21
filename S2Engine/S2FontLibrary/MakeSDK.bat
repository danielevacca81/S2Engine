@echo off
cls

set DESTFOLDER="..\..\SDK\S2FontLibrary"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\Debug
mkdir %DESTFOLDER%\bin\Release
mkdir %DESTFOLDER%\include\Fonts

copy /Y .\src\S2FontLibraryAPI.h  %DESTFOLDER%\include\
copy /Y .\src\Fonts\FontNew.h     %DESTFOLDER%\include\Fonts

copy /Y .\bin\Debug\S2FontLibrary.dll   %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\S2FontLibrary.lib   %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\S2FontLibrary.pdb   %DESTFOLDER%\bin\Debug

copy /Y .\bin\Release\S2FontLibrary.dll %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\S2FontLibrary.lib %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\S2FontLibrary.pdb %DESTFOLDER%\bin\Release


echo Done!

:EOF