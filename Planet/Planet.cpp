#include "Planet.h"
#include <iostream>
#include <GL/glew.h>
#include "../Texture/getBMP.h"
#include <cmath>
#include <vector>

using namespace std;

// Scaling factors
const float ORBITAL_SPEED_SCALE = 1.0f;
const float ROTATIONAL_SPEED_SCALE = 0.0001f;


Planet::Planet() = default;

void loadTextures(std::string file, unsigned int t) {
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



void Planet::initTexture() {
    loadTextures("..//Images//Planets//" + planetName + ".bmp", textureID);
}

void applyLighting(void) {
    // Define light properties
    GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0}; // Light at the Sun's position
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

    // Set light properties
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void Planet::setup(void) {
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // Enable light source 0

    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY); // Enable normal array
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Use GL_MODULATE for lighting effects

    vertices = new float[3 * (p + 1) * (q + 1)];
    textureCoordinates = new float[2 * (p + 1) * (q + 1)];
    normals = new float[3 * (p + 1) * (q + 1)]; // Allocate memory for normals

    // Set the array pointers.
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates);
    glNormalPointer(GL_FLOAT, 0, normals); // Set normal pointer

    // Fill the vertex, texture coordinates, and normals arrays.
    fillVertexArray();
    fillTextureCoordArray();

    // Initialize texture
    initTexture();

    // Apply lighting only once here
    applyLighting();

    // Enable normal rescaling
    glEnable(GL_RESCALE_NORMAL); // Use GL_NORMALIZE if the normals are not unit length
}

void Planet::draw() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPushMatrix();

    for (int j = 0; j < q; j++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= p; i++) {
            glArrayElement((j + 1) * (p + 1) + i);
            glArrayElement(j * (p + 1) + i);
        }
        glEnd();
    }

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Planet::fillVertexArray(void) {
    int i, j, k = 0;
    for (j = 0; j <= q; j++) {
        for (i = 0; i <= p; i++) {
            vertices[k] = f(i, j);
            vertices[k + 1] = g(i, j);
            vertices[k + 2] = h(i, j);

            // Calculate and store normals
            float length = sqrt(vertices[k] * vertices[k] +
                                vertices[k + 1] * vertices[k + 1] +
                                vertices[k + 2] * vertices[k + 2]);
            normals[k] = vertices[k] / length;
            normals[k + 1] = vertices[k + 1] / length;
            normals[k + 2] = vertices[k + 2] / length;

            k += 3;
        }
    }
}

void Planet::fillTextureCoordArray(void) {
    int i, j, k = 0;
    for (j = 0; j <= q; j++) {
        for (i = 0; i <= p; i++) {
            textureCoordinates[k++] = (float)i / p;
            textureCoordinates[k++] = (float)j / q;
        }
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

void Planet::setMaterialProperties(float r, float g, float b) {
    GLfloat mat_ambient[] = {0.2f * r, 0.2f * g, 0.2f * b, 1.0f};
    GLfloat mat_diffuse[] = {0.8f * r, 0.8f * g, 0.8f * b, 1.0f};
    GLfloat mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat mat_shininess[] = {100.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}


void Planet::drawPlanets(Planet *planets, float angle) {
    glEnable(GL_LIGHTING); // Enable lighting

    vector<int> x = {0, 30, 50, 150, -90, -110, -130, 50, -150, 125};
    vector<int> z = {-150, -140, -110, -90, -70, -200, -210, -70, -50, -90};

//    vector<int> x = {0, 30, 60, 120, -90, -110, -130, 50, -150, 125};
//    vector<int> z = {-150, -140, -110, -70, -180, -210, -240, -100, -280, -70};


    // Orbital and rotational speeds (relative to some base speed)
    float orbitalSpeeds[] = {0.0f, 4.74f, 3.50f, 2.98f, 2.41f, 1.31f, 0.97f, 0.68f, 0.54f, 2.98f};
    float rotationalSpeeds[] = {0.0f, 10.83f, -6.52f, 1674.4f, 868.22f, 45583.0f, 36840.0f, -9320.0f, 9660.0f, 0.0f};

    // True colors of planets
    float planetColors[10][3] = {
            {1.0f, 1.0f, 0.0f}, // Sun (yellowish)
            {0.8f, 0.8f, 0.8f}, // Mercury (gray)
            {1.0f, 0.8f, 0.6f}, // Venus (yellowish)
            {0.0f, 0.5f, 1.0f}, // Earth (blue)
            {1.0f, 0.0f, 0.0f}, // Mars (red)
            {1.0f, 0.8f, 0.6f}, // Jupiter (orange-brown)
            {1.0f, 1.0f, 0.8f}, // Saturn (pale yellow)
            {0.6f, 0.8f, 1.0f}, // Uranus (light blue)
            {0.3f, 0.3f, 1.0f}, // Neptune (blue)
            {0.8f, 0.8f, 0.8f}  // Moon (gray)
    };

    // Array of planet scales
    float planetScales[] = {
            1.0f,                      // Sun
            4880.0f / 1392700.0f,      // Mercury
            12104.0f / 1392700.0f,     // Venus
            12742.0f / 1392700.0f,     // Earth
            6779.0f / 1392700.0f,      // Mars
            139820.0f / 1392700.0f,    // Jupiter
            116460.0f / 1392700.0f,    // Saturn
            50724.0f / 1392700.0f,     // Uranus
            49244.0f / 1392700.0f,     // Neptune
            3474.0f / 1392700.0f       // Moon
    };


    glPushMatrix();
    glScalef(0.3, 0.3, 0.3);

    for (int i = 0; i < 10; i++) {
        glPushMatrix();
        auto p = planets[i];
//        float scaleFactor = planetScales[i] * 10.0;

        if (i == 0) {
            // The Sun: No rotation, just draw it at its position without lighting
            glEnable(GL_LIGHTING);
            glTranslatef(x[i], 0, z[i]);
            p.setMaterialProperties(planetColors[i][0], planetColors[i][1], planetColors[i][2]);
//            glScalef(scaleFactor, scaleFactor, scaleFactor); // Example scaling
            p.draw();
            glEnable(GL_LIGHTING);
        } else {
            // Other planets: Rotate around the Sun
            glTranslatef(x[0], 0, z[0]);             // Translate to the Sun's position
            glRotatef(angle * orbitalSpeeds[i] * ORBITAL_SPEED_SCALE, 0, 1, 0); // Rotate around the Sun
            glTranslatef(x[i] - x[0], 0, z[i] - z[0]); // Translate to the planet's orbit position

            if (i == 9) { // Moon
                // Rotate the Moon around the Earth
                glTranslatef(x[3] - x[0], 0, z[3] - z[0]); // Translate to Earth's position
                glRotatef(angle * orbitalSpeeds[i] * ORBITAL_SPEED_SCALE, 0, 1, 0); // Rotate around the Earth
                glTranslatef(x[i] - x[3], 0, z[i] - z[3]); // Translate to the Moon's orbit position relative to the Earth
            }

            // Scale the planet if necessary


//            glScalef(scaleFactor, scaleFactor, scaleFactor); // Example scaling

            // Rotate the planet around its own axis
            float rotationAngle = angle * (rotationalSpeeds[i] * ROTATIONAL_SPEED_SCALE);
            glRotatef(rotationAngle, 0, 1, 0); // Rotate around its own axis

            // Apply material properties
            p.setMaterialProperties(planetColors[i][0], planetColors[i][1], planetColors[i][2]);
            p.draw();
        }
        glPopMatrix();
    }

    glPopMatrix();
}