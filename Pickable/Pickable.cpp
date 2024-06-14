#include "Pickable.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

Pickable::Pickable(float x, float y, float z, int type) : x(x), y(y), z(z), type(type), active(true) {}

void drawHeart() {
    glPushMatrix(); // Push current matrix onto the stack
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f); // Rotate by 45 degrees around the z-axis

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.8f); // Top of the heart
    for (int i = -90; i <= 90; i++) {
        float t = i / 180.0f * M_PI;
        float x = 0.05f * 16.0f * pow(sin(t), 3);
        float y = -0.05f * (13.0f * cos(t) - 5.0f * cos(2 * t) - 2.0f * cos(3 * t) - cos(4 * t));
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix(); // Restore previous matrix from the stack
}



void drawWeapon() {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.8f);
    for (int i = -90; i <= 90; i++) {
        float t = i / 180.0f * M_PI;
        float x = 0.05f * 16.0f * pow(sin(t), 3);
        float y = 0.05f * (13.0f * cos(t) - 5.0f * cos(2 * t) - 2.0f * cos(3 * t) - cos(4 * t));
        glVertex2f(x, y);
    }
    glVertex2f(0.1f, 2.0f); // Top of the heart
    glEnd();
}



void Pickable::draw() {
    if (!active) return;

    glPushMatrix();
    glTranslatef(x, y, z);

    if (type == 0) {
        glColor3f(0.0f, 1.0f, 0.0f); // Health is green
        drawHeart();
    } else {
        glColor3f(1.0f, 0.0f, 0.0f); // Weapon strength is red
        drawWeapon();
    }

    glPopMatrix();
}

void Pickable::update() {
    if (active) {
        y -= 0.1f; // Move downwards
        if (y < -10.0f) {
            active = false; // Deactivate if it goes out of screen
        }
    }
}

bool Pickable::isActive() {
    return active;
}
