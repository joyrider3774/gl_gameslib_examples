# gl_gameslib_examples
Examples for using gl_gameslib which is simple games library targeting multiple devices or platforms. 
The gl_gameslib repository can be found here:
https://github.com/joyrider3774/gl_gameslib

## Supporting targets from same codebase
- SDL3
- Playdate

## Included Examples

### Formula_1
A simple game and watch formula 1 style game, makes uses mainly of gl_image's, uses its own text drawing

### Rects
Shows displaying rects using graphics functions

### Images
Shows displaying and loading images using graphics functions

### Fonts
Shows displaying and loading fonts using fonts functions, based on sfont

## Your own game
there is a `my_game` folder included which contains the basic needs to start your own project using gl_gameslib.
You can checkout this repository and start making your own game using it

## Building
\*.bat and \*.sh files are included to be able to build your own my_game game or all the examples.

The \*.bat files should be started from within a visual studio 2022 developpers prompt with the playdate SDK environment variable set.
When building for the device you will also need an arm toolchain, and cmake you need to have as well, basically the default setup to build C apps for playdate.

The \*.sh files are included to build on linux or from msys on windows, sometimes it's easier to build the sdl3 versions using it.
