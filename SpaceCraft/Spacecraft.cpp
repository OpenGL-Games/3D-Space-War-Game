#include "Spacecraft.h"
#include <iostream>
#include <cmath>
#include "../Texture/getBMP.h"

int slices = 20;

Spacecraft::Spacecraft() {
    xVal = 0;
    zVal = -10;
    angle = 0;
    health = 50; // Initial health
    score = 0;
    enemy = false;
    active = true;
    lastShootTime = 2;
}

Spacecraft::Spacecraft(unsigned int tex1, unsigned int tex2) {
    texture[0] = tex1;
    texture[1] = tex2;
    xVal = 0;
    zVal = -10;
    angle = 0;
    health = 50; // Initial health
    score = 0;
    enemy = false;
    active = true;
    lastShootTime = 2;
}

Spacecraft::Spacecraft(float x, float y, float z, float a, bool e, unsigned int tex1, unsigned int tex2) {
    xVal = x;
    zVal = z;
    angle = a;
    health = 50; // Initial health
    score = 0;
    enemy = e;
    texture[0] = tex1;
    texture[1] = tex2;
    active = true;
    lastShootTime = 2;
}


void Spacecraft::loadTextures(string file, unsigned int t) {
    imageFile* image = getBMP(file.c_str());

    if (!image) {
        std::cerr << "Failed to load image: " << file << std::endl;
        return;
    }

    glBindTexture(GL_TEXTURE_2D, t);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Free the image memory
    delete[] image->data;
    delete image;
}

void Spacecraft::draw() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!active) return;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glPushMatrix();
    glTranslatef(xVal, 0.0, zVal);
    glRotatef(angle, 0.0, 1.0, 0.0);
    if (enemy) {
        glScalef(0.5f, 0.5f, 0.5f);

        // Map the texture onto the sphere.
        glBindTexture(GL_TEXTURE_2D, texture[1]);

    } else {
        // Map the texture onto the sphere.
        glBindTexture(GL_TEXTURE_2D, texture[0]);
    }

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
    glPopMatrix();

    // Disable texturing
    glDisable(GL_TEXTURE_2D);
}

void Spacecraft::updateEnemy(float xTarget, float zTarget) {
    // Perform any animation or update logic here
    if (!isActive())
        return;
    if (enemy) {
        if (xTarget > xVal) {
            xVal += 0.05f;
        } else if (xTarget < xVal) {
            xVal -= 0.05f;
        }
        if (zTarget > zVal) {
            zVal += 0.05f;
        } else if (zTarget < zVal) {
            zVal -= 0.05f;
        }

        // Calculate the angle to face the target
        float dx = xTarget - xVal;
        float dz = zTarget - zVal;
        angle = atan2(dx, dz) * 180.0 / M_PI; // Convert radians to degrees
//        cout << angle << endl;
    }
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
    float directionX = sin(angle * M_PI / 180.0);
    float directionZ = cos(angle * M_PI / 180.0);
    if(enemy){
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        if (currentTime - lastShootTime >= shootInterval) {
            Projectile proj(xVal, 0.0, zVal, directionX, directionZ, 0.3);
            float enemyColor[3] = {0.0f, 0.5f, 0.5f}; // Red color for enemy projectiles
            proj.setColor(enemyColor);
            projectiles.push_back(proj);
            lastShootTime = currentTime;
        }
    }else{
        Projectile proj(xVal, 0.0, zVal, directionX, directionZ, 0.2);
        proj.setStrength(power);
        projectiles.push_back(proj);
    }
}

bool Spacecraft::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        active = false;
        return true;
    }
    if(!enemy){
        score -= 10;
        if(score <= 0) score = 0;
    }
    return false;
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

    // enable depth test
    glEnable(GL_DEPTH_TEST);

//     Load textures
    if (enemy) {
        loadTextures("..//Images//enemy.bmp", texture[1]);
    } else {
        loadTextures("..//Images//spacecraft.bmp", texture[0]);
    }

}

void Spacecraft::increaseScore(int val) {
    score += val;
}
