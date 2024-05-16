//
// Created by Mahmoud on 5/15/2024.
//
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include "game.h"
#include "../Texture/getBMP.h"

using namespace std;

extern int windowChoice;
float aspectRatio;
// Globals.
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 1; // Animated?
static int animationPeriod = 0.5; // Time interval between frames.
double previousTime = 0.0;

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

void animate(int value) {
    if (isAnimate) {
        double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0; // Get current time in seconds
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        // Update angles based on elapsed time
        latAngle += 50.0 * deltaTime; // Adjust these multipliers as needed
        if (latAngle > 360.0) latAngle -= 360.0;
        longAngle += 10.0 * deltaTime; // Adjust these multipliers as needed
        if (longAngle > 360.0) longAngle -= 360.0;
        Yangle += 0.0001; // Adjust these multipliers as needed

        // Request a redraw
        glutPostRedisplay();
        // Register the timer function again
        glutTimerFunc(animationPeriod, animate, 1);
    }
}

void Game::draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Get the aspect ratio of the window
    int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
    int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
    aspectRatio = (float)screenWidth / (float)screenHeight;
    glLoadIdentity();

    drawPlanet("sun", texture[0], 0.2, 0);

    // Rotate scene.
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);

    drawPlanet("mercury", texture[1], 0.2, -9);
    drawPlanet("venus", texture[2], 0.2, -4);
    drawPlanet("earth", texture[3], 0.2, 1);
    drawPlanet("mars", texture[4], 0.2, 5);
    drawPlanet("jupiter", texture[5], 0.2, 10);
    drawPlanet("saturn", texture[6], 0.2, 11);
    drawPlanet("uranus", texture[7], 0.2, -7);
    drawPlanet("neptune", texture[9], 0.2, 8);
    drawPlanet("moon", texture[8], 0.2, 9);

    glutSwapBuffers();
}



// OpenGL window reshape routine.
void Game::resize(int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
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
}

// Keyboard input processing routine.
void Game::keyInput(unsigned char key, int x, int y)
{
    if (key == 27) {
        exit(0);
    } else if(key == ' ') {
        previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0; // Get initial time in seconds
        animate(1);
    }
    glutPostRedisplay();
}

void Game::specialKeyInput(int key, int x, int y)
{

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

void Game::drawPlanet(string planetName, unsigned int textureId,float radius, int offset) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    loadTextures("..//Images//Planets//" + planetName + ".bmp", textureId);
    // Map the texture onto the sphere.
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPushMatrix();
    glTranslatef(offset, 0, -30);
    glScalef( 1.0f * radius,  aspectRatio * radius, 1.0f * radius);

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
