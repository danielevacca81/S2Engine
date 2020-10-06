@echo off
SET SHELL_DIR=%~dp0
SET MODULE_NAME=s2Renderer
SET BASEDIR_SDK=%SHELL_DIR%..\..\SDK
SET BASEDIR_MODULE_SOURCES=%SHELL_DIR%

echo MODULE_NAME: %MODULE_NAME%
echo BASEDIR_SDK: %BASEDIR_SDK%
echo BASEDIR_MODULE_SOURCES: %BASEDIR_MODULE_SOURCES%

IF NOT EXIST %BASEDIR_SDK% (
mkdir %BASEDIR_SDK% || (echo %BASEDIR_SDK% failed! && PAUSE && exit)
)

IF EXIST %BASEDIR_SDK%\%MODULE_NAME% (
rmdir /s /q %BASEDIR_SDK%\%MODULE_NAME% || ( goto :ERR )
)

REM wait for really directory deleting from HD
rem timeout 1 > NUL
IF NOT EXIST %BASEDIR_SDK%\%MODULE_NAME% (
mkdir %BASEDIR_SDK%\%MODULE_NAME% || (echo %BASEDIR_SDK%\%MODULE_NAME% failed! && PAUSE && exit)
) ELSE (
echo The directory %BASEDIR_SDK%\%MODULE_NAME% exist!
rem && PAUSE && exit
)

cd %BASEDIR_SDK%\%MODULE_NAME%

IF NOT EXIST .\bin\Win32\Debug (
mkdir .\bin\Win32\Debug || (echo mkdir .\bin\Win32\Debug failed! && PAUSE && exit)
)
IF NOT EXIST .\bin\Win32\Release (
mkdir .\bin\Win32\Release || (echo mkdir .\bin\Win32\Release failed! && PAUSE && exit)
)
IF NOT EXIST .\bin\x64\Debug (
mkdir .\bin\x64\Debug || (echo mkdir .\bin\x64\Debug failed! && PAUSE && exit)
)
IF NOT EXIST .\bin\x64\Release (
mkdir .\bin\x64\Release || (echo mkdir .\bin\x64\Release failed! && PAUSE && exit)
)

copy /Y %BASEDIR_MODULE_SOURCES%\src\*.h .\

copy /Y %BASEDIR_MODULE_SOURCES%\bin\Win32\Debug\*.lib .\bin\Win32\Debug
copy /Y %BASEDIR_MODULE_SOURCES%\bin\Win32\Debug\*.dll .\bin\Win32\Debug
copy /Y %BASEDIR_MODULE_SOURCES%\bin\Win32\Debug\*.pdb .\bin\Win32\Debug
								
copy /Y %BASEDIR_MODULE_SOURCES%\bin\Win32\Release\*.lib .\bin\Win32\Release
copy /Y %BASEDIR_MODULE_SOURCES%\bin\Win32\Release\*.dll .\bin\Win32\Release
copy /Y %BASEDIR_MODULE_SOURCES%\bin\Win32\Release\*.pdb .\bin\Win32\Release
								
copy /Y %BASEDIR_MODULE_SOURCES%\bin\x64\Debug\*.lib .\bin\x64\Debug
copy /Y %BASEDIR_MODULE_SOURCES%\bin\x64\Debug\*.dll .\bin\x64\Debug
copy /Y %BASEDIR_MODULE_SOURCES%\bin\x64\Debug\*.pdb .\bin\x64\Debug
								
copy /Y %BASEDIR_MODULE_SOURCES%\bin\x64\Release\*.lib .\bin\x64\Release
copy /Y %BASEDIR_MODULE_SOURCES%\bin\x64\Release\*.dll .\bin\x64\Release
copy /Y %BASEDIR_MODULE_SOURCES%\bin\x64\Release\*.pdb .\bin\x64\Release

cd ..\..

echo [%MODULE_NAME%] Done!
