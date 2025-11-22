@echo off
if not exist bin mkdir bin

echo [BUILD] KhaliError Compiler banacchi...
g++ src/main.cpp src/Lexer.cpp src/Generator.cpp -o bin/khali_error.exe

if %errorlevel% neq 0 (
    echo [ERROR] Jhamela hoyeche boss! Code check koro.
    pause
    exit /b
)

echo [SUCCESS] Compiler ready!
echo.
echo [TEST] Running example...
bin\khali_error.exe examples/new_test.ke
pause