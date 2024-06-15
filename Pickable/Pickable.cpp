#include "Pickable.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

Pickable::Pickable(float x, float y, float z, int type) : x(x), y(y), z(z), type(type), active(true) {}

void drawCone(float baseRadius, float height, int slices, int stacks) {
    float angleStep = 2.0f * M_PI / slices;
    float heightStep = height / stacks;

    // Draw the sides
    for (int i = 0; i < stacks; ++i) {
        float currentHeight = i * heightStep;
        float nextHeight = (i + 1) * heightStep;
        float currentRadius = baseRadius * (1 - (float)i / stacks);
        float nextRadius = baseRadius * (1 - (float)(i + 1) / stacks);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float angle = j * angleStep;
            float x = cos(angle);
            float y = sin(angle);

            glVertex3f(currentRadius * x, currentHeight, currentRadius * y);
            glVertex3f(nextRadius * x, nextHeight, nextRadius * y);
        }
        glEnd();
    }

    // Draw the base
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= slices; ++i) {
        float angle = i * angleStep;
        glVertex3f(baseRadius * cos(angle), 0.0f, baseRadius * sin(angle));
    }
    glEnd();
}

void drawCube(float size) {
    float halfSize = size / 2.0f;

    glBegin(GL_QUADS);

    // Front face
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);

    // Back face
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);

    // Left face
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);

    // Right face
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);

    // Top face
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);

    // Bottom face
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);

    glEnd();
}

void drawHeart() {
    // Draw the half-sphere
    glPushMatrix(); // Push current matrix onto the stack
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotate by 45 degrees around the z-axis

        glPushMatrix();
        glRotatef(-90, 1.0f, 0.0f, 0.0f); // Rotate cone to point upwards
        drawCone(0.5f, 1.0f, 20, 20);  // Base radius 0.5, height 1.0, 20 slices, 20 stacks
        glPopMatrix();
//
    glPopMatrix();
}

void drawWeapon() {
    // Draw the handle of the gun (cuboid)
//
    glPushMatrix();
        glScalef(0.2f, 0.6f, 0.2f); // Scale the cuboid to the size of the handle
        drawCube(1.0f); // Draw the cuboid
    glPopMatrix();

    // Draw the barrel of the gun (cuboid)
    glPushMatrix();
        glTranslatef(0.0f, 0.3f, -0.5f); // Move the cuboid to the position of the barrel
        glScalef(0.2f, 0.2f, 1.0f); // Scale the cuboid to the size of the barrel
        drawCube(1.0f); // Draw the cuboid
    glPopMatrix();

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
//        y -= 0.1f; // Move downwards
//        if (y < -10.0f) {
//            active = false; // Deactivate if it goes out of screen
//        }
    }
}

bool Pickable::isActive() {
    return active;
}