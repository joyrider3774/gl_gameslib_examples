@echo off
setlocal enabledelayedexpansion

REM Array of game projects to build
set "GAMES=Examples/formula_1 Examples/rects Examples/fonts Examples/images"

REM Root build directory
set "BUILD_ROOT=build/examples_pd_simulator"

REM Get the script's directory (project root)
set "PROJECT_ROOT=%~dp0"
set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

echo Building multiple game projects...
echo Project root: %PROJECT_ROOT%
echo.

REM Build each game
for %%G in (%GAMES%) do (
    REM Extract game name from path
    for %%F in ("%%G") do set "GAME_NAME=%%~nxF"
    
    REM Create build directory for this game
    set "BUILD_DIR=%BUILD_ROOT%\!GAME_NAME!"
    
    echo =========================================
    echo Building: %%G
    echo Game name: !GAME_NAME!
    echo Build dir: !BUILD_DIR!
    echo =========================================
    
    REM Configure
    cmake -G "Visual Studio 17 2022" ^
	      -DPLATFORM=playdate ^
	      -DGAME_SOURCE_DIR="%PROJECT_ROOT%/%%G" ^
          -DPLAYDATE_GAME_NAME="!GAME_NAME!" ^
          -S "%PROJECT_ROOT%" ^
          -B "!BUILD_DIR!"
    
    if errorlevel 1 (
        echo ERROR: CMake configuration failed for %%G
        exit /b 1
    )
    
    REM Build
    cmake --build "!BUILD_DIR!"
    
    if errorlevel 1 (
        echo ERROR: Build failed for %%G
        exit /b 1
    )
    
    echo.
    echo Successfully built %%G
    echo.
)

echo =========================================
echo All projects built successfully!
echo =========================================