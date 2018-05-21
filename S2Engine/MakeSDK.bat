@echo off
cd S2Globals
call MakeSDK.bat
pause
cls
cd..

cd S2Framework
call MakeSDK.bat
pause
cls
cd..

cd S2Qt
call MakeSDK.bat
pause
cls
cd..

cd S2Renderer
call MakeSDK.bat
pause
cls
cd..

echo Done!
pause