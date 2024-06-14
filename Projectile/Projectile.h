#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <GL/freeglut.h>

class Projectile {
public:
    Projectile(float startX, float startY, float startZ, float directionX, float directionZ);
    void draw();
    void update(float dt);

    bool isActive() const { return active; }
    float getX() const { return posX; }
    float getY() const { return posY; }
    float getZ() const { return posZ; }


private:
    float posX, posY, posZ; // Position of the projectile
    float dirX, dirZ;       // Direction of movement
    bool active;            // Is the projectile active?

    static const float speed; // Speed of the projectile

    GLuint projectileDisplayList;

    void createProjectileDisplayList();
};

#endif // PROJECTILE_H