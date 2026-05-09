@echo off

setlocal

set "INCLUDE=C:\Program Files\PellesC\Include;C:\Program Files\PellesC\Include\Win;"
set "LIB=C:\Program Files\PellesC\Lib;C:\Program Files\PellesC\Lib\Win64;"
set "PATH=C:\Program Files\PellesC\bin;%PATH%"

del main_pocc.obj 2>nul
del main_pocc.exe 2>nul


pocc main.c /utf-8 /Ze /Zx /std=C11 /Fomain_pocc.obj
polink main_pocc.obj user32.lib kernel32.lib advapi32.lib /OUT:main_pocc.exe
main_pocc.exe
endlocal
pause