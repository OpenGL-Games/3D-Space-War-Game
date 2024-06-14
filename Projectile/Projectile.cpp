#include "Projectile.h"
#include <cmath>
#include <iostream>

using namespace std;

const float Projectile::speed = 0.5f;

Projectile::Projectile(float startX, float startY, float startZ, float directionX, float directionZ)
        : posX(startX), posY(startY), posZ(startZ), dirX(directionX), dirZ(directionZ), active(true) {
    createProjectileDisplayList();
}

void Projectile::createProjectileDisplayList() {
    glEnable(GL_DEPTH_TEST);
    projectileDisplayList = glGenLists(1);
    glNewList(projectileDisplayList, GL_COMPILE);
    glutSolidSphere(0.5, 8, 8);
    glEndList();
}

void Projectile::draw() {
    if (active) {
        glColor3f(1.0, 0.0, 0.0);
        glPushMatrix();
        glTranslatef(posX, posY, posZ);
        glCallList(projectileDisplayList);
        glPopMatrix();
    }
}

void Projectile::update(float dt) {
    if (active) {
        posX += dirX * speed * dt;
        posZ += dirZ * speed * dt;
        // Check for boundaries or other conditions to deactivate the projectile
    }
}