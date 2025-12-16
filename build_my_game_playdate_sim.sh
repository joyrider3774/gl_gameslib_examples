#!/bin/sh

# Array of game projects to build
GAME="my_game"

# Root build directory
BUILD_ROOT="build/my_game_pd_simulator"

# Get the script's directory (project root)
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"

echo "Building game ..."
echo "Project root: ${PROJECT_ROOT}"
echo ""

GAME_NAME=$(basename "${GAME}")

# Create build directory for this game
BUILD_DIR="${BUILD_ROOT}/${GAME_NAME}"

echo "========================================="
echo "Building: ${GAME}"
echo "Game name: ${GAME_NAME}"
echo "Build dir: ${BUILD_DIR}"
echo "========================================="

# Configure
cmake -DPLATFORM=playdate \
	  -DGAME_SOURCE_DIR="${PROJECT_ROOT}/${GAME}" \
	  -DPLAYDATE_GAME_NAME="${GAME_NAME}" \
	  -S "${PROJECT_ROOT}" \
	  -B "${BUILD_DIR}"

if [ $? -ne 0 ]; then
	echo "ERROR: CMake configuration failed for ${GAME}"
	exit 1
fi

# Build
cmake --build "${BUILD_DIR}"

if [ $? -ne 0 ]; then
	echo "ERROR: Build failed for ${GAME}"
	exit 1
fi

echo ""
echo "✓ Successfully built ${GAME}"
echo ""
