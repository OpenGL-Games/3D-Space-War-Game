#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "game.h"
#include <cmath>
#include "../Planet/Planet.h"
#include "../Asteriods/Asteriods.h"
#include "../SpaceCraft/SpaceCraft.h"
#include <vector>

using namespace std;

// Globals.
float aspectRatio; // Aspect ratio of the window
Planet *planets = nullptr;
Asteriods *asteriods = nullptr;
Spacecraft *spaceCraft = nullptr;
float angle = 0;
float craftAngle = 0;

Game::Game() = default;

void Game::animate(int value = 0) {
    angle += 0.5;
    if(angle > 360) angle = 0;
    glutPostRedisplay();
    glutTimerFunc(animationPeriod, animate, value);
}

void Game::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // camera transformation
    gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // global transformation
    glTranslatef(0.0, 0.0, -6);
    glScalef(1.0, 1.0 * aspectRatio, 1.0);

    // Space Craft Setup and draw
    spaceCraft->setEnemy(false);
    spaceCraft->setup();

    glPushMatrix();
        glTranslatef(spaceCraft->getX(), 0.0, spaceCraft->getZ());
        glRotatef(spaceCraft->getAngle(), 0.0, 1.0, 0.0);
        spaceCraft->draw();
    glPopMatrix();

    // planets setup and draw
    planets = new Planet[9];
    for (int i = 0; i < 9; i++) {
        planets[i].textureID = texture[i];
        planets[i].planetName = planetNames[i];
        planets[i].setup();
    }
    Planet::drawPlanets(planets, angle);

    // draw asteroids
    asteriods->draw();

    glutSwapBuffers();
}

void Game::resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
    glMatrixMode(GL_MODELVIEW);
}

void Game::setup(void) {
    glEnable(GL_DEPTH_TEST);
    // background color
    glClearColor(0.0352941f, 0.0431373f, 0.0705882f, 0.0);

    // calc aspect ratio
    int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
    int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
    aspectRatio = (float)screenWidth / (float)screenHeight;

    // Load textures
    glGenTextures(12, texture);

    // Asteroids instance
    asteriods = new Asteriods();

    // spacecraft instance
    spaceCraft = new Spacecraft(texture[9], texture[10]);

    animate();
    glEnable(GL_DEPTH_TEST);
}

// handle ESC and Shoot
void Game::keyInput(unsigned char key, int x, int y) {
    switch (key)
    {
        // ESC key
        case 27:
            exit(0);
            break;
        // Shoot
        case 32:
        {
            // shoot logic
            cout << "Shoot" << endl;
            break;
        }
        default:
            break;
    }
}

// handle arrow keys
void Game::specialKeyInput(int key, int x, int y) {


    float tempxVal = spaceCraft->getX(), tempzVal = spaceCraft->getZ(), tempAngle = spaceCraft->getAngle();

    // Compute next position.
    if (key == GLUT_KEY_RIGHT) tempAngle = craftAngle + 5.0;
    if (key == GLUT_KEY_LEFT) tempAngle = craftAngle - 5.0;
    if (key == GLUT_KEY_DOWN)
    {
        tempxVal = spaceCraft->getX() - sin(craftAngle * M_PI / 180.0);
        tempzVal = spaceCraft->getZ() - cos(craftAngle * M_PI / 180.0);
    }
    if (key == GLUT_KEY_UP)
    {
        tempxVal = spaceCraft->getX() + sin(craftAngle * M_PI / 180.0);
        tempzVal = spaceCraft->getZ() + cos(craftAngle * M_PI / 180.0);
    }

    // Angle correction.
    if (tempAngle > 360.0) tempAngle -= 360.0;
    if (tempAngle < 0.0) tempAngle += 360.0;

    spaceCraft->setX(tempxVal);
    spaceCraft->setZ(tempzVal);
    spaceCraft->setAngle(tempAngle);

    craftAngle = tempAngle;

    glutPostRedisplay();
}


