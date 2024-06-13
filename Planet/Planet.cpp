//
// Created by Hp on 5/16/2024.
//

#include "Planet.h"
#include <iostream>
#include <GL/glew.h>
#include "../Texture/getBMP.h"
#include <cmath>
#include <vector>

using namespace std;

void Planet::loadTextures(string file, unsigned int t) {
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

Planet::Planet() = default;

void Planet::setup(void) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    vertices = new float[3 * (p + 1)*(q + 1)];
    textureCoordinates = new float[2 * (p + 1)*(q + 1)];

    // Set the array pointers.
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates);

    // Fill the vertex and texture co-ordinates arrays.
    fillVertexArray();
    fillTextureCoordArray();

}

void Planet::fillVertexArray(void) {
    int i, j, k;
    int mai, maj, mak;
    k = 0;
    for (j = 0; j <= q; j++) {
        for (i = 0; i <= p; i++) {
            vertices[k++] = f(i, j);
            vertices[k++] = g(i, j);
            vertices[k++] = h(i, j);
            mai = max(mai, i);
            maj = max(maj, j);
            mak = max(mak, k);
        }
    }
}

void Planet::fillTextureCoordArray(void) {
    int i, j, k;

    k = 0;
    for (j = 0; j <= q; j++)
        for (i = 0; i <= p; i++)
        {
            textureCoordinates[k++] = (float)i / p;
            textureCoordinates[k++] = (float)j / q;
        }
}

float Planet::f(int i, int j) {
    return (R * cos(-M_PI / 2.0 + (float)j / q * M_PI) * cos(2.0 * (float)i / p * M_PI));
}

float Planet::g(int i, int j) {
    return (R * sin(-M_PI / 2.0 + (float)j / q * M_PI));
}

float Planet::h(int i, int j) {
    return (-R * cos(-M_PI / 2.0 + (float)j / q * M_PI) * sin(2.0 * (float)i / p * M_PI));
}

void Planet::draw() {

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    loadTextures("..//Images//Planets//" + planetName + ".bmp", textureID);

    // Set material properties
    GLfloat mat_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat mat_diffuse[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat mat_shininess[] = {50.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Map the texture onto the sphere.
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPushMatrix();

    for (int j = 0; j < q; j++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= p; i++)
        {
            glArrayElement((j + 1)*(p + 1) + i);
            glArrayElement(j*(p + 1) + i);
        }
        glEnd();
    }
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void Planet::drawPlanets(Planet *planets, float angle) {

    vector<int> x = {0, 30, 50, 150, -90, -110, -130, 50, -150};
    vector<int> z = {-150, -140, -110, -90, -70, -200, -210, -70, -50};

    glPushMatrix();
    glScalef(0.3, 0.3, 0.3);

    for (int i = 0; i < 9; i++) {
        glPushMatrix();
        auto p = planets[i];

        if (i == 0) {
            // The Sun: No rotation, just draw it at its position
            glTranslatef(x[i], 0, z[i]);
            p.draw();
        } else {
            // Other planets: Rotate around the Sun
            glTranslatef(x[0], 0, z[0]);             // Translate to the Sun's position
            glRotatef(angle, 0, 1, 0);               // Rotate around the Sun
            glTranslatef(x[i] - x[0], 0, z[i] - z[0]); // Translate to the planet's orbit position
            // Rotate the planet around its own axis
            glRotatef(angle, 0, 1, 0);               // Rotate around its own axis
            p.draw();
        }

        glPopMatrix();
    }

    glPopMatrix();

}




