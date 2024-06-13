#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "game.h"
#include <cmath>
#include "../Planet/Planet.h"
#include "../Asteriods/Asteriods.h"
#include <vector>

using namespace std;

// Globals.
float aspectRatio; // Aspect ratio of the window
Planet *planets = nullptr;
Asteriods *asteriods = nullptr;
float angle = 0;

void coodrinateSystem() {
    // draw point at origin
    glPointSize(1.0);
    glLineWidth(1.0);
    glBegin(GL_POINTS);
    glColor3f(1.0, 0, 0);
    glVertex3f(0.0, 0.0, 0.0);
    glEnd();

    // draw coordinate system
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(40.0, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 40.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 20.0);
    glEnd();
}

Game::Game() = default;

void Game::animate(int value = 0) {
    angle += 0.01;
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

    // draw coordinate system for reference
    // coodrinateSystem();

    // draw point
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, -20.0);
    glEnd();


    // draw planets
    Planet::drawPlanets(planets, angle);

    // draw spacecraft


    // draw asteroids
    asteriods->draw();

    animate();
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
    cout << "Setup" << endl;
    // background color
    glClearColor(0.0352941f, 0.0431373f, 0.0705882f, 0.0);

    // calc aspect ratio
    int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
    int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
    aspectRatio = (float)screenWidth / (float)screenHeight;

    // Planets Setup
    glGenTextures(9, texture);
    planets = new Planet[9];
    for (int i = 0; i < 9; i++) {
        planets[i].textureID = texture[i];
        planets[i].setup();
        planets[i].planetName = planetNames[i];
    }

    // Asteroids Setup
    asteriods = new Asteriods();


    // Space Craft Setup

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
        case ' ':
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

    // Compute next position.
    if (key == GLUT_KEY_LEFT) {
        cout << "Left" << endl;
    }
    if (key == GLUT_KEY_RIGHT) {
        cout << "Right" << endl;
    }
    if (key == GLUT_KEY_UP) {
        cout << "Up" << endl;
    }
    if (key == GLUT_KEY_DOWN) {
        cout << "Down" << endl;
    }

    glutPostRedisplay();
}


