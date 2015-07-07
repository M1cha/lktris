CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
 
PROJECT(sdlemu)
 
ADD_EXECUTABLE(sdlemu main.c ../lk/app/game/game.c ../lk/app/game/fb.c ../lk/app/game/stone.c ../lk/app/game/font.c ../lk/app/game/states/ingame.c ../lk/app/game/states/menu.c ../lk/app/game/states/gameover.c)
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -O3 -g -fno-builtin -finline -W -Wall -Wno-multichar -Wno-unused-parameter -Wno-unused-function -fstack-protector-all -ffunction-sections -fdata-sections")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -gc-sections")


INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL REQUIRED sdl)
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIRS} ../lk/app/game/include)
TARGET_LINK_LIBRARIES(sdlemu ${SDL_LIBRARIES})

