@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
pushd "%~dp0.."
python tests\prepare_performance_tests.py
if errorlevel 1 goto failed
cl /nologo /EHsc /W4 /wd4127 /O2 /I include tests\performance_regression.cpp /Fotests\build\performance_regression.obj /Fetests\build\performance_regression.exe
if errorlevel 1 goto failed
tests\build\performance_regression.exe
if errorlevel 1 goto failed
popd
exit /b 0
:failed
popd
exit /b 1
