@echo off

if "%~1"=="" (
    echo Usage: build.bat ^<input.cpp^> [output.exe]
    exit /b 1
)

set INPUT=%~1

if "%~2"=="" (
    set OUTPUT=implant.exe
) else (
    set OUTPUT=%~2
)

cl.exe ^
    /nologo ^
    /Ox ^
    /MT ^
    /W0 ^
    /GS- ^
    /DNDEBUG ^
    /Tc"%INPUT%" ^
    /link ^
    /OUT:"%OUTPUT%" ^
    /SUBSYSTEM:CONSOLE ^
    /MACHINE:x64