:: set this batch as the compiler for your project with
:: the following settings:
::
::  Compiler = path to this batch
::  Parameters = %ProjectDir% %Input%
::  Default directory: %Dir%

@echo off
set JAM_TOOLSET=MINGW
set TOP=%1
jam %2 %3 %4 %5
