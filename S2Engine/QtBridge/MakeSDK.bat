@echo off
cls

set DESTFOLDER="..\..\SDK\QtBridge"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\x64\Debug
mkdir %DESTFOLDER%\bin\x64\Release

mkdir %DESTFOLDER%\include
mkdir %DESTFOLDER%\include\qtbridge

copy /Y .\src\*.h                      %DESTFOLDER%\include\qtbridge

copy /Y .\bin\x64\Debug\QtBridge.dll   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\QtBridge.lib   %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\QtBridge.pdb   %DESTFOLDER%\bin\x64\Debug

copy /Y .\bin\x64\Release\QtBridge.dll %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\QtBridge.lib %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\QtBridge.pdb %DESTFOLDER%\bin\x64\Release
													
echo Done!

:EOF