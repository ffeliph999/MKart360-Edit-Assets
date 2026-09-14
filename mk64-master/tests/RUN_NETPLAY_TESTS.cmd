@echo off
setlocal
pushd "%~dp0.."
if defined VS100COMNTOOLS (
  call "%VS100COMNTOOLS%..\..\VC\vcvarsall.bat" x86
) else (
  call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
)
if errorlevel 1 goto failed
if not exist tests\build mkdir tests\build
cl /nologo /EHsc /W4 /wd4127 /O2 /I include tests\netplay_regression.cpp /Fetests\build\netplay_regression.exe /Fotests\build\netplay_regression.obj
if errorlevel 1 goto failed
tests\build\netplay_regression.exe
if errorlevel 1 goto failed
popd
exit /b 0
:failed
popd
exit /b 1
