//
// Created by Mahmoud on 6/14/2024.
//

#ifndef GAME_PICKABLE_H
#define GAME_PICKABLE_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <ctime>

class Pickable {
public:
    float x, y, z;
    bool active;
    int type; // 0 for health, 1 for weapon strength

    Pickable(float x, float y, float z, int type);
    void draw();
    void update();
    bool isActive();
};


#endif //GAME_PICKABLE_H
