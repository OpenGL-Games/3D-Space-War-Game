#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <GL/freeglut.h>

class Projectile {
public:
    Projectile(float startX, float startY, float startZ, float directionX, float directionZ);
    void draw();
    void update(float dt);

    bool isActive() const { return active; }
    void deactivate() { active = false; }
    float getX() const { return posX; }
    float getY() const { return posY; }
    float getZ() const { return posZ; }

    int getStrength() const;

    void setStrength(int strength);
    void increaseStrength(int val);

private:
    float posX, posY, posZ; // Position of the projectile
    float dirX, dirZ;       // Direction of movement
    bool active;            // Is the projectile active?
    int strength = 1;

    static const float speed; // Speed of the projectile

    GLuint projectileDisplayList;

    void createProjectileDisplayList();

    void setColor(float *color);

    void updateColorAndRadius();
};

#endif // PROJECTILE_H