#include "Spacecraft.h"
#include <iostream>
#include <cmath>
#include "../Texture/getBMP.h"

int slices = 20;

Spacecraft::Spacecraft(unsigned int tex1, unsigned int tex2) {
    texture[0] = tex1;
    texture[1] = tex2;
    xVal = 0;
    zVal = -10;
    angle = 0;
    health = 100; // Initial health
}

void loadTextures(string file, unsigned int t) {
    imageFile *image[1];
    image[0] = getBMP(file.c_str());

    glBindTexture(GL_TEXTURE_2D, t);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->width, image[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Spacecraft::draw() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if(enemy) {
        loadTextures("..//Images//enemy.bmp", texture[1]);
        // Map the texture onto the sphere.
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        // Draw base circle
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < slices; ++i) {
            glArrayElement(i);
        }
        glEnd();

        // Draw sides
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < slices; ++i) {
            glArrayElement(i);
            glArrayElement(slices); // Apex vertex
        }
        glEnd();
    }
    else {
        loadTextures("..//Images//spacecraft.bmp", texture[0]);
        // Map the texture onto the sphere.
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        // Draw base circle
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < slices; ++i) {
            glArrayElement(i);
        }
        glEnd();

        // Draw sides
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < slices; ++i) {
            glArrayElement(i);
            glArrayElement(slices); // Apex vertex
        }
        glEnd();
    }

    // Disable texturing
    glDisable(GL_TEXTURE_2D);
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

void Spacecraft::fillVertexArray(void) {
    float baseRadius = 3.0f;
    float height = 7.0f;
    float angleStep = 2.0f * M_PI / slices;

    // Generate vertices for the base circle
    for (int i = 0; i < slices; ++i) {
        float angle = i * angleStep;
        float x = baseRadius * cos(angle);
        float y = baseRadius * sin(angle);
        cout << x << " " << y << endl;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }

    // Add the apex vertex
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(height);
}

void Spacecraft::fillTextureCoordArray(void) {
    float angleStep = 2.0f * M_PI / slices;

    // Generate texture coordinates for the base circle
    for (int i = 0; i < slices; ++i) {
        float angle = i * angleStep;
        textureCoordinates.push_back((cos(angle) + 1.0f) / 2.0f);
        textureCoordinates.push_back((sin(angle) + 1.0f) / 2.0f);
    }

    // Add the texture coordinate for the apex
    textureCoordinates.push_back(0.5f);
    textureCoordinates.push_back(1.0f);
}

void Spacecraft::setup() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

//    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Fill the vertex and texture coordinates arrays
    fillVertexArray();
    fillTextureCoordArray();

    // Set the array pointers
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates.data());


    // Cull the back faces of the cone
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


