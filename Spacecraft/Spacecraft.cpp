#include "Spacecraft.h"
#include <iostream>

Spacecraft::Spacecraft() {
    xVal = 0;
    zVal = 0;
    angle = 0;
    health = 100; // Initial health
    spacecraftModel = glGenLists(1);
    glNewList(spacecraftModel, GL_COMPILE);
    glPushMatrix();
    glRotatef(180.0, 0.0, 1.0, 0.0); // Point the spacecraft down the z-axis initially
    glColor3f(1.0, 1.0, 1.0);
    glutWireCone(5.0, 10.0, 10, 10);
    glPopMatrix();
    glEndList();
}

void Spacecraft::draw() {
    glPushMatrix();
    glTranslatef(xVal, 0.0, zVal);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glCallList(spacecraftModel);
    glPopMatrix();
}

void Spacecraft::update(float delta_time) {
    // Perform any animation or update logic here
}

void Spacecraft::move(float dx, float dz) {
    xVal += dx;
    zVal += dz;
}

void Spacecraft::rotate(float angle_change) {
    angle += angle_change;
    if (angle > 360.0)
        angle -= 360.0;
    if (angle < 0.0)
        angle += 360.0;
}

void Spacecraft::shoot() {
    // Implement projectile shooting logic here
}

void Spacecraft::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        std::cout << "Spacecraft destroyed!" << std::endl;
        // Implement spacecraft destruction logic here
    }
}

void Spacecraft::increaseHealth(int healthIncrease) {
    health += healthIncrease;
    if (health > 100)
        health = 100;
}