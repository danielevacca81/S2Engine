@echo off

if "%1"=="" goto ERR1
if "%2"=="" goto ERR2
SET SHELL_DIR=%~dp0
SET MODULE_NAME=%1
SET CONFIG_NAME=%2
SET BASE_BUILD_DIR="%SHELL_DIR%..\..\s2Engine"
SET BASE_INCL_DIR_DEST=%BASE_BUILD_DIR%\include
SET BASE_BIN_DIR_DEST=%BASE_BUILD_DIR%\bin
SET BASEDIR_MODULE_SOURCES=%SHELL_DIR%

echo MODULE_NAME: %MODULE_NAME%
echo BASE_INCL_DIR_DEST: %BASE_INCL_DIR_DEST%
echo BASEDIR_MODULE_SOURCES: %BASEDIR_MODULE_SOURCES%

IF NOT EXIST %BASE_INCL_DIR_DEST% (
mkdir %BASE_INCL_DIR_DEST% || (echo %BASE_INCL_DIR_DEST% failed! && PAUSE && exit)
) 

IF EXIST %BASE_INCL_DIR_DEST%\%MODULE_NAME% (
rmdir /s /q %BASE_INCL_DIR_DEST%\%MODULE_NAME% || ( goto :ERR )
)

REM wait for really directory deleting from HD
rem timeout 2
IF NOT EXIST %BASE_INCL_DIR_DEST%\%MODULE_NAME% (
mkdir %BASE_INCL_DIR_DEST%\%MODULE_NAME% || (echo %BASE_INCL_DIR_DEST%\%MODULE_NAME% failed! && PAUSE && exit)
) ELSE (
echo The directory %BASE_INCL_DIR_DEST%\%MODULE_NAME% already exists! && PAUSE && exit
)

IF NOT EXIST %BASE_BIN_DIR_DEST%\x64\%CONFIG_NAME% (
mkdir %BASE_BIN_DIR_DEST%\x64\%CONFIG_NAME% || (echo mkdir %BASE_BIN_DIR_DEST%\x64\%CONFIG_NAME% failed! && PAUSE && exit)
)

cd %BASE_INCL_DIR_DEST%\%MODULE_NAME%
copy /Y   %BASEDIR_MODULE_SOURCES%\src\Application.h .
copy /Y   %BASEDIR_MODULE_SOURCES%\src\Application_API.h .
copy /Y   %BASEDIR_MODULE_SOURCES%\src\CommandLineParser.h .
copy /Y   %BASEDIR_MODULE_SOURCES%\src\Window.h .
copy /Y   %BASEDIR_MODULE_SOURCES%\src\WindowParameters.h .


cd %BASE_BIN_DIR_DEST%
copy /Y %BASEDIR_MODULE_SOURCES%\bin\x64\%CONFIG_NAME%\*.lib .\x64\%CONFIG_NAME%
copy /Y %BASEDIR_MODULE_SOURCES%\bin\x64\%CONFIG_NAME%\*.dll .\x64\%CONFIG_NAME%
copy /Y %BASEDIR_MODULE_SOURCES%\bin\x64\%CONFIG_NAME%\*.pdb .\x64\%CONFIG_NAME%

echo Done!
goto EOF

:ERR1
echo Missing module name
goto EOF

:ERR2
echo Missing configuration
goto EOF

:ERR

:EOF
cd ..\..
