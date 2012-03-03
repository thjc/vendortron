@ECHO OFF
zpu-elf-gcc -c -g -Wall -Os ezpu.c
zpu-elf-ar rcs libezpu.a ezpu.o
del ezpu.o
rem zpu-elf-strip libezpu.a