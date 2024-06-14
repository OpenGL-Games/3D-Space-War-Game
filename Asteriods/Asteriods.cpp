#include "Asteriods.h"
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>

void Asteriods::init() {
    // Draw sphere with given center (x, y, z) and radius
    int slices = 50, stacks = 50, radius = 1;
    int x = 0, y = 0, z = 0;
    for (int i = 0; i <= slices; ++i) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / slices);
        double z0 = sin(lat0) * radius + z;
        double zr0 = cos(lat0) * radius;

        double lat1 = M_PI * (-0.5 + (double) i / slices);
        double z1 = sin(lat1) * radius + z;
        double zr1 = cos(lat1) * radius;

        for (int j = 0; j <= stacks; ++j) {
            double lng = 2 * M_PI * (double) (j - 1) / stacks;
            double x0 = cos(lng) * zr0 + x;
            double y0 = sin(lng) * zr0 + y;
            double x1 = cos(lng) * zr1 + x;
            double y1 = sin(lng) * zr1 + y;

            points.push_back(x0);
            points.push_back(y0);
            points.push_back(z0);
            points.push_back(x1);
            points.push_back(y1);
            points.push_back(z1);
        }
    }

    randX = vector<float>(num);
    randY = vector<float>(num);
    randZ = vector<float>(num);
    for (int i = 0; i < num; i++) {
        randX[i] = rand() % 151 - 50;    // Range: -50 to 50
        randY[i] = rand() % 151 - 50;    // Range: -50 to 50
        randZ[i] = -(rand() % 250 + 1);
    }
}

void Asteriods::draw() {

    glColor3f(0.2f, 0.2f, 0.2f);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);  // Enable light source 0 (you can use multiple light sources if needed)

    // Define light parameters
    GLfloat light_position[] = { 10.0, 10.0, 10.0, 1.0 };  // Example positional light
    GLfloat ambient[] = { 0.4, 0.4, 0.4, 1.0 };         // Ambient light
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };         // Diffuse light
    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };        // Specular light

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    // Material properties
    GLfloat material_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat shininess = 300.0;


    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);

    for (int i = 0; i < num; i++) {
        glPushMatrix();
        glTranslatef(randX[i], randY[i], randZ[i]);
        glScalef(0.2, 0.2, 0.2);  // Scale the sphere to 0.5 times its original size
        // Set material properties for this object
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < points.size(); j += 3) {
            glVertex3f(points[j], points[j + 1], points[j + 2]);
        }
        glEnd();
        glPopMatrix();
    }

    glPopMatrix();

    glDisable(GL_LIGHTING);  // Disable lighting after drawing the sphere

}

Asteriods::Asteriods(int count) {
    num = count;
    init();
}