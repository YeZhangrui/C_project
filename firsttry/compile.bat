@echo off
chcp 65001 > nul
"D:\x86_64-16.1.0-release-win32-seh-ucrt-rt_v14-rev1\mingw64\bin\g++.exe" -c "c:\Users\aaa\Desktop\C_project\change_calculator.c" -o "c:\Users\aaa\Desktop\C_project\change_calculator.o" 2> "c:\Users\aaa\Desktop\C_project\compile_err.txt"
type "c:\Users\aaa\Desktop\C_project\compile_err.txt"
