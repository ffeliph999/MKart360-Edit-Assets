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
cl /nologo /EHsc /W4 /wd4127 /wd4018 /O2 /I include tests\MK64_NETPLAY_4_TO_8_STRESS_TEST.cpp /Fetests\build\MK64_NETPLAY_4_TO_8_STRESS_TEST.exe /Fotests\build\MK64_NETPLAY_4_TO_8_STRESS_TEST.obj
if errorlevel 1 goto failed
tests\build\MK64_NETPLAY_4_TO_8_STRESS_TEST.exe
if errorlevel 1 goto failed
popd
exit /b 0
:failed
popd
exit /b 1
