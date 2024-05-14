cmake_minimum_required(VERSION 3.27)
project(Game)

set(CMAKE_CXX_STANDARD 17)

add_executable(Game
        main.cpp
        Menu/menu.h
        Menu/menu.cpp
        Texture/getBMP.h
        Texture/getBMP.cpp
)
target_link_libraries(Game -lglew32 -lopengl32 -lfreeglut -lglu32 -lwinmm)