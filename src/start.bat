@echo off
set fileOne=main
set fileTwo=MiniShell

echo Compiling %fileOne%.cpp , %fileTwo%.cpp...

g++ %fileOne%.cpp %fileTwo%.cpp  -o run.exe

if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)

echo Running run.exe...
run.exe
pause