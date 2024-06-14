cmake_minimum_required(VERSION 3.26)
project(Game)

set(CMAKE_CXX_STANDARD 17)

add_executable(Game
        main.cpp
        Menu/menu.h
        Menu/menu.cpp
        Texture/getBMP.h
        Texture/getBMP.cpp
        Game/game.cpp
        Game/game.h
        Asteriods/Asteriods.cpp
        Asteriods/Asteriods.h
        Planet/Planet.cpp
        Planet/Planet.h
        SpaceCraft/Spacecraft.cpp
        SpaceCraft/Spacecraft.h
        Projectile/Projectile.h
        Projectile/Projectile.cpp
)
target_link_libraries(Game -lglew32 -lopengl32 -lfreeglut -lglu32 -lwinmm)