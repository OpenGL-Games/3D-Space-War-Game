#include "Projectile.h"
#include <cmath>
#include <iostream>
#include <GL/glut.h>

using namespace std;

const float Projectile::speed = 0.5f;


Projectile::Projectile(float startX, float startY, float startZ, float directionX, float directionZ, float rad)
        : posX(startX), posY(startY), posZ(startZ), dirX(directionX), dirZ(directionZ), active(true), strength(1), radius(rad) {
    createProjectileDisplayList();
}

void Projectile::createProjectileDisplayList() {
    glEnable(GL_DEPTH_TEST);
    projectileDisplayList = glGenLists(1);
    glNewList(projectileDisplayList, GL_COMPILE);
    glutSolidSphere(radius, 20, 20); // Use radius for the sphere
    glEndList();
}

void Projectile::draw() {
    if (active) {
        glColor3f(color[0], color[1], color[2]);
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

int Projectile::getStrength() const {
    return strength;
}

void Projectile::increaseStrength(int val) {
    strength += val;
    updateColorAndRadius();
    createProjectileDisplayList(); // Update the display list with new radius
}

void Projectile::setStrength(int strength) {
    Projectile::strength = strength;
    updateColorAndRadius();
    createProjectileDisplayList(); // Update the display list with new radius
}

void Projectile::updateColorAndRadius() {
    // Calculate radius based on strength
    radius += 0.01;
    if(radius >= 2.0) radius = 2.0;

    // Calculate color based on strength
    color[1] -= 0.01; // Green component decreases as strength increases
    if(color[1] <= 0) {
        color[1] = 0;
        color[2] += 0.01;
        if(color[2] >= 1) color[2] = 1;
    }
}

void Projectile::setColor(const float col[3]) {
    for (int i = 0; i < 3; ++i) {
        color[i] = col[i];
    }
}



