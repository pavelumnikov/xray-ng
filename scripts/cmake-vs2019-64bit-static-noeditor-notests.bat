for %%i in ("%~dp0..") do set "ROOTDIR=%%~fi"
setlocal enablextensions
if errorlevel 1 echo Unable to enable extensions
set CMAKE_PATH=%ROOTDIR%\extras\cmake
set BUILD_ROOT=%ROOTDIR%\projects\%~n0
if "%PHYSX_DIR%"=="" ECHO PHYSX_DIR is NOT defined
if "%VK_SDK_PATH%"=="" ECHO VK_SDK_PATH is NOT defined
del /s /q %BUILD_ROOT%
mkdir %BUILD_ROOT%
pushd %BUILD_ROOT%
%CMAKE_PATH%\bin\cmake.exe -G "Visual Studio 16 2019" -DPHYSX_DIR="%PHYSX_DIR%\install\vc15win64" -DVK_SDK_PATH="%VK_SDK_PATH%" --check-system-vars %ROOTDIR%
popd
pause
endlocal