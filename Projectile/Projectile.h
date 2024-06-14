#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <GL/freeglut.h>

class Projectile {
public:
    Projectile(float startX, float startY, float startZ, float directionX, float directionZ, float rad);
    void draw();
    void update(float dt);

    bool isActive() const { return active; }
    void deactivate() { active = false; }
    float getX() const { return posX; }
    float getY() const { return posY; }
    float getZ() const { return posZ; }

    int getStrength() const;

    void setStrength(int strength);
    void setRadius(float rad) {radius = rad;}
    void setColor(const float col[3]);
    void increaseStrength(int val);

private:
    float posX, posY, posZ; // Position of the projectile
    float dirX, dirZ;       // Direction of movement
    bool active;            // Is the projectile active?
    int strength = 1;
    float radius = 0.2;
    float color[3] = {1.0f, 1.0f, 0.0f}; // Default color (light yellow)

    static const float speed; // Speed of the projectile

    GLuint projectileDisplayList;

    void createProjectileDisplayList();

    void updateColorAndRadius();
};

#endif // PROJECTILE_H