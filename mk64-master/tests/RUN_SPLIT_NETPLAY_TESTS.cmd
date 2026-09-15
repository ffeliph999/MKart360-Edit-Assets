@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
pushd "%~dp0.."
if not exist tests\build mkdir tests\build
cl /nologo /EHsc /W4 /wd4127 /O2 /I include tests\split_netplay_regression.cpp /Fotests\build\split_netplay_regression.obj /Fetests\build\split_netplay_regression.exe
if errorlevel 1 goto failed
tests\build\split_netplay_regression.exe
if errorlevel 1 goto failed
popd
exit /b 0
:failed
popd
exit /b 1
