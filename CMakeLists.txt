cmake_minimum_required(VERSION 3.14)

set(PLATFORM "sdl3" CACHE STRING "Platform: sdl3 or playdate")

if(PLATFORM STREQUAL "playdate")
    set(BUILD_FOR_PLAYDATE ON CACHE BOOL "" FORCE)
    set(BUILD_FOR_SDL3 OFF CACHE BOOL "" FORCE)
    set(CMAKE_C_STANDARD 11)

    set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

    if (NOT ${ENVSDK} STREQUAL "")
        file(TO_CMAKE_PATH ${ENVSDK} SDK)
    else()
        execute_process(
            COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
            COMMAND head -n 1
            COMMAND cut -c9-
            OUTPUT_VARIABLE SDK
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()

    if (NOT EXISTS ${SDK})
        message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
        return()
    endif()

    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
    set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
endif()

# Game Name Customization
if(NOT DEFINED PLAYDATE_GAME_NAME)
    set(PLAYDATE_GAME_NAME Game)
endif()

set(PLAYDATE_GAME_DEVICE "${PLAYDATE_GAME_NAME}_DEVICE")

project(${PLAYDATE_GAME_NAME} C ASM)

if(NOT DEFINED GAME_SOURCE_DIR)
    set(GAME_SOURCE_DIR "${PROJECT_SOURCE_DIR}/Examples/formula_1")
endif()

file(TO_CMAKE_PATH "${PROJECT_SOURCE_DIR}" PROJECT_SOURCE_DIR_NORMALIZED)
file(TO_CMAKE_PATH "${GAME_SOURCE_DIR}" GAME_SOURCE_DIR_NORMALIZED)

file(GLOB SRC CONFIGURE_DEPENDS
    "${GAME_SOURCE_DIR_NORMALIZED}/src/*.c"
    "${GAME_SOURCE_DIR_NORMALIZED}/src/*.h"
)

if(PLATFORM STREQUAL "playdate")
    set(BUILD_FOR_PLAYDATE ON CACHE BOOL "" FORCE)
    add_definitions(-DPLATFORM_PLAYDATE)
    add_definitions(-DTARGET_EXTENSION)

    add_subdirectory("${PROJECT_SOURCE_DIR}/Vendored/gl_gameslib")
    
    if (TOOLCHAIN STREQUAL "armgcc")
        add_executable(${PLAYDATE_GAME_DEVICE} ${CMAKE_CURRENT_SOURCE_DIR}/buildsupport/playdate/setup.c ${SRC})
        target_link_libraries(${PLAYDATE_GAME_DEVICE} gl_gamelib)
    else()
        add_library(${PLAYDATE_GAME_NAME} SHARED ${SRC} ${CONFIGURE_DEPENDS})
        target_link_libraries(${PLAYDATE_GAME_NAME} gl_gamelib)
    endif()

    include(${CMAKE_CURRENT_SOURCE_DIR}/buildsupport/playdate/playdate_game.cmake)
    
elseif(PLATFORM STREQUAL "sdl3")
    # Create an option to switch between a system sdl library and a vendored sdl library
    option(USE_VENDORED_SDL "Use vendored libraries" OFF)

    # set the output directory for built objects.
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GAME_SOURCE_DIR_NORMALIZED}/Source")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GAME_SOURCE_DIR_NORMALIZED}/Source")

    # prevent installing to system directories. 
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")

    if(USE_VENDORED_SDL)
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
        
        # SDL_3
        set(SDL_STATIC_PIC ON CACHE BOOL "" FORCE)
        set(SDL_SHARED OFF)	
        add_subdirectory("${PROJECT_SOURCE_DIR}/Vendored/SDL3" EXCLUDE_FROM_ALL)
        
        # SDL_mixer (used for playing audio)
        set(SDLMIXER_MIDI_NATIVE OFF) 
        set(SDLMIXER_MIDI OFF)
        set(SDLMIXER_GME OFF)
        set(SDLMIXER_WAVPACK OFF)
        set(SDLMIXER_MOD ON)
        set(SDLMIXER_MOD_XMP ON)
        set(SDLMIXER_MOD_XMP_SHARED OFF)
        set(SDLMIXER_MOD_XMP_LITE OFF)
        set(SDLMIXER_MOD_MODPLUG OFF)
        set(SDLMIXER_MOD_MODPLUG_SHARED OFF)
        set(SDLMIXER_OPUS OFF)
        set(SDLMIXER_SNDFILE OFF)
        set(SDLMIXER_FLAC OFF)
        set(SDLMIXER_MP3 OFF)
        set(SDLMIXER_VORBIS OFF)
        set(SDLMIXER_BUILD_SHARED_LIBS OFF)
        set(SDLMIXER_VENDORED ON)
        add_subdirectory("${PROJECT_SOURCE_DIR}/Vendored/SDL_mixer" EXCLUDE_FROM_ALL)
        
        # SDL_image (used for loading various image formats)
        set(SDLIMAGE_VENDORED ON)
        set(SDLIMAGE_AVIF OFF)
        set(SDLIMAGE_BMP OFF)
        set(SDLIMAGE_GIF OFF)
        set(SDLIMAGE_JPG OFF)
        set(SDLIMAGE_JXL OFF)
        set(SDLIMAGE_LBM OFF)
        set(SDLIMAGE_PCX OFF)
        set(SDLIMAGE_PNG ON)
        set(SDLIMAGE_PNM OFF)
        set(SDLIMAGE_QOI OFF)
        set(SDLIMAGE_SVG OFF)
        set(SDLIMAGE_TGA OFF)
        set(SDLIMAGE_TIF OFF)
        set(SDLIMAGE_WEBP OFF)
        set(SDLIMAGE_XCF OFF)
        set(SDLIMAGE_XPM OFF)
        set(SDLIMAGE_XV OFF)
        add_subdirectory("${PROJECT_SOURCE_DIR}/Vendored/SDL_image" EXCLUDE_FROM_ALL)
    else()
        set(SDL_SHARED ON)
        find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
        find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image)
        find_package(SDL3_mixer REQUIRED CONFIG REQUIRED COMPONENTS SDL3_mixer)
    endif()

    set(BUILD_FOR_PLAYDATE OFF CACHE BOOL "" FORCE)
    set(BUILD_FOR_SDL3 ON CACHE BOOL "" FORCE)
    
    # Add library subdirectory
    add_subdirectory("${PROJECT_SOURCE_DIR}/Vendored/gl_gameslib")
       
    if (WIN32)
        set(CMAKE_EXE_LINKER_FLAGS "-static -mwindows")
    endif()
    
    add_executable(${PLAYDATE_GAME_NAME})
    target_sources(${PLAYDATE_GAME_NAME} PRIVATE ${SRC} ${CONFIGURE_DEPENDS})
    add_definitions(-DPLATFORM_SDL3)

    # on Visual Studio, set our app as the default project
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${EXECUTABLE_NAME}")

    if(USE_VENDORED_SDL)
        target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE 
            SDL3_mixer::SDL3_mixer-static
            SDL3_image::SDL3_image-static
            SDL3::SDL3-static
            gl_gamelib
        )

        # SDL_Image bug: https://github.com/libsdl-org/SDL_image/issues/506
        if (APPLE AND NOT BUILD_SHARED_LIBS)
            find_library(IO_LIB ImageIO REQUIRED)
            find_library(CS_LIB CoreServices REQUIRED)
            find_library(CT_LIB CoreText REQUIRED)
            find_library(CG_LIB CoreGraphics REQUIRED)
            find_library(CF_LIB CoreFoundation REQUIRED)
            target_link_libraries(${PLAYDATE_GAME_NAME} PUBLIC ${CF_LIB} ${CT_LIB} ${IO_LIB} ${CS_LIB} ${CG_LIB})
        endif()
    else()
        target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE 
            SDL3_mixer::SDL3_mixer
            SDL3_image::SDL3_image
            SDL3::SDL3
            gl_gamelib
        )
    endif()
endif()