@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
pushd "%~dp0.."
if not exist tests\build mkdir tests\build
python tests\prepare_hud_tests.py
if errorlevel 1 goto failed
cl /nologo /TC /O2 /Gy /DXBOX360_PORT=1 /DNON_MATCHING=1 /DAVOID_UB=1 /DF3DEX_GBI=1 /DF3D_OLD=1 /DVERSION_US=1 /I . /I include /I src /I src\racing /I src\ending /I src\audio /I src\data /I courses tests\race8_regression.c /Fotests\build\race8_regression.obj /Fetests\build\race8_regression.exe /link /OPT:REF
if errorlevel 1 goto failed
tests\build\race8_regression.exe
if errorlevel 1 goto failed
popd
exit /b 0
:failed
popd
exit /b 1
