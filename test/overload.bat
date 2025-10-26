@echo off
setlocal enabledelayedexpansion

set HOST=localhost
set PORT=8080

REM Create a valid HTTP request file
(
  echo GET / HTTP/1.1
  echo Host: %HOST%
  echo User-Agent: TestClient
  echo Accept: */*
  echo Connection: keep-alive
  echo.
) > request.txt

:loop
  echo Sending request to %HOST%:%PORT%...
  ncat %HOST% %PORT% < request.txt
goto loop
