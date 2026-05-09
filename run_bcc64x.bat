@echo off

bcc64x -O3 -std=c11 main.c -o main_bcc64x.exe
main_bcc64x.exe

pause