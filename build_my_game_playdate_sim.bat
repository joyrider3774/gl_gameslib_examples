@echo off
setlocal enabledelayedexpansion

REM Array of game projects to build
set "GAME=my_game"

REM Root build directory
set "BUILD_DIR=build/my_game_pd_simulator"

REM Get the script's directory (project root)
set "PROJECT_ROOT=%~dp0"
set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

echo Building game project...
echo Project root: %PROJECT_ROOT%
echo.

set "GAME_NAME=My_Game"
    
echo =========================================
echo Building: %%G
echo Game name: !GAME_NAME!
echo Build dir: !BUILD_DIR!
echo =========================================
    
REM Configure
cmake -G "Visual Studio 17 2022" ^
	  -DPLATFORM=playdate ^
	  -DUSE_VENDORED_SDL=1 ^
      -DUSE_VENDORED_SDL=1 ^
	  -DGAME_SOURCE_DIR="%PROJECT_ROOT%/%GAME%" ^
      -DPLAYDATE_GAME_NAME="!GAME_NAME!" ^
      -S "%PROJECT_ROOT%" ^
      -B "!BUILD_DIR!"
    
if errorlevel 1 (
    echo ERROR: CMake configuration failed for %GAME%
    exit /b 1
)
    
REM Build
cmake --build "!BUILD_DIR!"
    
if errorlevel 1 (
    echo ERROR: Build failed for %GAME%
    exit /b 1
)
    
echo.
echo Successfully built %GAME%
echo.