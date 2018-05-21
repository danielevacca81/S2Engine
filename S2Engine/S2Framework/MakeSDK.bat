@echo off
cls

set DESTFOLDER="..\..\SDK\S2Framework"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\include\
copy /Y .\src\S2FrameworkAPI.h     %DESTFOLDER%\include\

mkdir %DESTFOLDER%\include\application
copy /Y .\src\application\Application.h  %DESTFOLDER%\include\application

mkdir %DESTFOLDER%\include\input
copy /Y .\src\input\InputManager.h   %DESTFOLDER%\include\input
copy /Y .\src\input\MouseStatus.h    %DESTFOLDER%\include\input
copy /Y .\src\input\KeyboardStatus.h %DESTFOLDER%\include\input

rem mkdir %DESTFOLDER%\include\System

mkdir %DESTFOLDER%\include\threads
copy /Y .\src\threads\Event.h       %DESTFOLDER%\include\threads
copy /Y .\src\threads\Mutex.h       %DESTFOLDER%\include\threads
copy /Y .\src\threads\MutexLocker.h %DESTFOLDER%\include\threads
copy /Y .\src\threads\Thread.h      %DESTFOLDER%\include\threads


mkdir %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\S2Framework.lib %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\S2Framework.dll %DESTFOLDER%\bin\Debug
copy /Y .\bin\Debug\S2Framework.pdb %DESTFOLDER%\bin\Debug

mkdir %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\S2Framework.lib %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\S2Framework.dll %DESTFOLDER%\bin\Release
copy /Y .\bin\Release\S2Framework.pdb %DESTFOLDER%\bin\Release

echo Done!

:EOF