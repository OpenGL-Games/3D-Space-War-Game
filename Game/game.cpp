//
// Created by Mahmoud on 5/15/2024.
//
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include "game.h"
#include "../Texture/getBMP.h"
#include <map>

using namespace std;

extern int windowChoice;
float aspectRatio;
// Globals.
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int animationPeriod = 1; // Time interval between frames.
double previousTime = 0.0;
// Define global variables for zoom control
static float zoomFactor = 1.0f; // Initial zoom factor
static float zoomStep = 0.1f;   // Zoom step increment


std::map<std::string, float> planetScales = {
        {"sun", 1392700.0 / 12742.0},
        {"mercury", 4880.0 / 12742.0},
        {"venus", 12104.0 / 12742.0},
        {"earth", 1.0f}, // Baseline
        {"mars", 6779.0 / 12742.0},
        {"jupiter", 139820.0 / 12742.0},
        {"saturn", 116460.0 / 12742.0},
        {"uranus", 50724.0 / 12742.0},
        {"neptune", 49244.0 / 12742.0},
        {"moon", 3474.0 / 12742.0}
};

std::map<std::string, float> planetDistances = {
        {"sun", 0.0f},
        {"mercury", 57.9 * 5 / 100.0},
        {"venus", 108.2 * 5 / 100.0},
        {"earth", 149.6 * 5 / 100.0},
        {"mars", 227.9 * 5 / 100.0},
        {"jupiter", 778.5 * 5 / 100.0},
        {"saturn", 1427.0 * 5 / 100.0},
        {"uranus", 2871.0 * 5 / 100.0},
        {"neptune", 4497.1 * 5 / 100.0},
        {"moon", (149.6 + 0.384) * 5 / 100.0} // Distance from the Sun via Earth
};

Game::Game() = default;

// Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the sphere.
float Game::f(int i, int j)
{
    return (R * cos(-M_PI / 2.0 + (float)j / q * M_PI) * cos(2.0 * (float)i / p * M_PI));
}

float Game::g(int i, int j)
{
    return (R * sin(-M_PI / 2.0 + (float)j / q * M_PI));
}

float Game::h(int i, int j)
{
    return (-R * cos(-M_PI / 2.0 + (float)j / q * M_PI) * sin(2.0 * (float)i / p * M_PI));
}

// Routine to fill the vertex array with co-ordinates of the mapped sample points.
void Game::fillVertexArray(void)
{
    int i, j, k;

    k = 0;
    for (j = 0; j <= q; j++)
        for (i = 0; i <= p; i++)
        {
            vertices[k++] = f(i, j);
            vertices[k++] = g(i, j);
            vertices[k++] = h(i, j);
        }
}

// Routine to fill the texture co-ordinates array with the texture co-ordinate values at the mapped sample points.
void Game::fillTextureCoordArray(void)
{
    int i, j, k;

    k = 0;
    for (j = 0; j <= q; j++)
        for (i = 0; i <= p; i++)
        {
            textureCoordinates[k++] = (float)i / p;
            textureCoordinates[k++] = (float)j / q;
        }
}

//void CoordinateSystem(void)
//{
//    // Since the axes are to be drawn as solid lines, we disable lighting for them.
//    glDisable(GL_LIGHTING);
//    glBegin(GL_LINES);
//
//    glColor4f(1.0, 0.0, 0.0, 0.0);
//    glVertex3f(0.0, 0.0, 0.0);
//    glVertex3f(2*1, 0.0, 0.0);
//
//    glColor4f(0.0, 1.0, 0.0, 0.0);
//    glVertex3f(0.0, 0.0, 0.0);
//    glVertex3f(0.0, 2*1, 0.0);
//
//    glColor4f(0.0, 0.0, 1.0, 0.0);
//    glVertex3f(0.0, 0.0, 0.0);
//    glVertex3f(0.0, 0.0, 2.0);
//
//    glEnd();
//}

// Timer function.
void animate(int value)
{
//    if (isAnimate)
//    {
        latAngle += 6.0;
        if (latAngle > 360.0) latAngle -= 360.0;
        longAngle += 1.0;
        if (longAngle > 360.0) longAngle -= 360.0;

        glutPostRedisplay();
        glutTimerFunc(animationPeriod, animate, 1);
//    }
}

void Game::draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Get the aspect ratio of the window
    int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
    int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
    aspectRatio = (float)screenWidth / (float)screenHeight;
    glLoadIdentity();
    glTranslatef(/*planetDistances[planetName]*/ 0, 0, -25);


    // Fixed sun

//     Rotate scene.


    drawPlanet("sun", texture[0]);
    // Begin revolving ball.
//    glTranslatef(2.0, 0.0, 0.0);
//    glPushMatrix();
//    glRotatef(latAngle, 0.0, 1.0, 0.0);

//    glTranslatef(-2.0, 0.0, 0.0);

//    glTranslatef(-20.0, 0.0, 0.0);
    drawPlanet("mercury", texture[1]);
//    drawPlanet("venus", texture[2]);
//    drawPlanet("earth", texture[3]);
//    drawPlanet("mars", texture[4]);
//    drawPlanet("jupiter", texture[5]);
//    drawPlanet("saturn", texture[6]);
//    drawPlanet("uranus", texture[7]);
//    drawPlanet("neptune", texture[9]);
//    drawPlanet("moon", texture[8]);

    glutSwapBuffers();
}



void Game::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Adjust the viewing volume based on zoom factor
    float zoomedWidth = 5.0f * zoomFactor;
    float zoomedHeight = 5.0f * zoomFactor;
    glFrustum(-zoomedWidth, zoomedWidth, -zoomedHeight, zoomedHeight, 5.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Initialization routine.
void Game::setup(void)
{
    glClearColor(0.0352941f, 0.0431373f, 0.0705882f, 0.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Create texture ids.
    glGenTextures(12, texture);

    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    vertices = new float[3 * (p + 1)*(q + 1)];
    textureCoordinates = new float[2 * (p + 1)*(q + 1)];

    // Set the array pointers.
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates);

    // Fill the vertex and texture co-ordinates arrays.
    fillVertexArray();
    fillTextureCoordArray();

    // Cull the back faces of the sphere.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);  // Enable depth testing
    animate(1);
}

// Keyboard input processing routine.
void Game::keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'x':
            Xangle += 5.0;
            if (Xangle > 360.0) Xangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= 5.0;
            if (Xangle < 0.0) Xangle += 360.0;
            glutPostRedisplay();
            break;
        case 'y':
            Yangle += 5.0;
            if (Yangle > 360.0) Yangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Y':
            Yangle -= 5.0;
            if (Yangle < 0.0) Yangle += 360.0;
            glutPostRedisplay();
            break;
        case 'z':
            Zangle += 5.0;
            if (Zangle > 360.0) Zangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Z':
            Zangle -= 5.0;
            if (Zangle < 0.0) Zangle += 360.0;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}


void Game::specialKeyInput(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            zoomFactor -= zoomStep;
            if (zoomFactor < 0.1f) zoomFactor = 0.1f; // Limit minimum zoom
            resize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            break;
        case GLUT_KEY_DOWN:
            zoomFactor += zoomStep;
            resize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void Game::loadTextures(string file, unsigned int t) {
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

void Game::drawPlanet(string planetName, unsigned int textureId) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    loadTextures("..//Images//Planets//" + planetName + ".bmp", textureId);

    // Calculate the actual radius using the scaling factor
    float baseRadius = 0.17;
    float scaleFactor = 1;
    float actualRadius = baseRadius * scaleFactor;

    // Map the texture onto the sphere.
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPushMatrix();
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);

    if (planetName != "sun") {
//        glTranslatef(0, 0.0, -20.0);
//        glTranslatef(20.0, 0.0, 0.0);
//        glRotatef(latAngle, 0.0, 1.0, 0.0);
//        glTranslatef(10.0, 0.0, 0.0);
        float distanceFromSun = planetDistances[planetName];
        glRotatef(latAngle, 0.0, 1.0, 0.0);
        glTranslatef(-10.0, 0.0, 0.0);
    }
    glScalef(1.0f * actualRadius, aspectRatio * actualRadius, 1.0f * actualRadius);

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
}