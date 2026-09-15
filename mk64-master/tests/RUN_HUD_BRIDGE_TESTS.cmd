@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
pushd "%~dp0.."
python tests\prepare_hud_bridge_tests.py
if errorlevel 1 goto failed
cl /nologo /EHsc /W4 /O2 /I include tests\hud_bridge_regression.cpp /Fotests\build\hud_bridge_regression.obj /Fetests\build\hud_bridge_regression.exe
if errorlevel 1 goto failed
tests\build\hud_bridge_regression.exe
if errorlevel 1 goto failed
popd
exit /b 0
:failed
popd
exit /b 1
