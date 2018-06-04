@echo off
cls

set DESTFOLDER="..\..\SDK\Core"

rmdir /s /q %DESTFOLDER%
mkdir %DESTFOLDER%

mkdir %DESTFOLDER%\bin\x64\Debug
mkdir %DESTFOLDER%\bin\x64\Release

mkdir %DESTFOLDER%\include\
mkdir %DESTFOLDER%\include\commandline\
mkdir %DESTFOLDER%\include\containers\
mkdir %DESTFOLDER%\include\events\
mkdir %DESTFOLDER%\include\exceptions\
mkdir %DESTFOLDER%\include\filesystem\
mkdir %DESTFOLDER%\include\geometry\
mkdir %DESTFOLDER%\include\graphics\
mkdir %DESTFOLDER%\include\math\
mkdir %DESTFOLDER%\include\patterns\
mkdir %DESTFOLDER%\include\resources\
mkdir %DESTFOLDER%\include\utils\
mkdir %DESTFOLDER%\include\win32\

copy /Y .\src\commandline\*.h  %DESTFOLDER%\include\commandline\
copy /Y .\src\containers\*.h   %DESTFOLDER%\include\containers\
copy /Y .\src\containers\*.hpp %DESTFOLDER%\include\containers\
copy /Y .\src\events\*.h       %DESTFOLDER%\include\events\
copy /Y .\src\exceptions\*.h   %DESTFOLDER%\include\exceptions\
copy /Y .\src\filesystem\*.h   %DESTFOLDER%\include\filesystem\
copy /Y .\src\geometry\*.h     %DESTFOLDER%\include\geometry\
copy /Y .\src\graphics\*.h     %DESTFOLDER%\include\graphics\
copy /Y .\src\graphics\*.hpp   %DESTFOLDER%\include\graphics\
copy /Y .\src\math\*.h         %DESTFOLDER%\include\math\
copy /Y .\src\patterns\*.h     %DESTFOLDER%\include\patterns\
copy /Y .\src\resources\*.h    %DESTFOLDER%\include\resources\
copy /Y .\src\utils\*.h        %DESTFOLDER%\include\utils\
copy /Y .\src\win32\*.h        %DESTFOLDER%\include\win32\
copy /Y .\src\Core_API.h       %DESTFOLDER%\include\


copy /Y .\bin\x64\Release\*.lib     %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\*.dll     %DESTFOLDER%\bin\x64\Release
copy /Y .\bin\x64\Release\*.pdb     %DESTFOLDER%\bin\x64\Release

copy /Y .\bin\x64\Debug\*.lib     %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\*.dll     %DESTFOLDER%\bin\x64\Debug
copy /Y .\bin\x64\Debug\*.pdb     %DESTFOLDER%\bin\x64\Debug

rem pause
echo Done!

:EOF