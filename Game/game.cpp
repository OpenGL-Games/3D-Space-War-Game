#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "game.h"
#include <cmath>
#include "../Planet/Planet.h"
#include "../Asteriods/Asteriods.h"
#include "../SpaceCraft/SpaceCraft.h"
#include "../Pickable/Pickable.h"
#include <vector>
#include <ctime>

using namespace std;

// Globals.
float aspectRatio; // Aspect ratio of the window
Planet *planets = nullptr;
Asteriods *asteriods = nullptr;
Spacecraft *spaceCraft = nullptr;
vector<Spacecraft*> enemies;

float angle = 0;
float craftAngle = 0;

//time variables
float timeRemaining = 300.0; // Start at 300 seconds
int currTime = 0;

vector<Pickable*> pickables;
int pickableGenerationInterval = 10000; // Generate new pickables every 10 seconds
int lastPickableGenerationTime = 0;

Game::Game() = default;

void generatePickables() {
   float x = static_cast<float>(rand() % 20 - 10);
    // float x = 0;
    float y = 20.0f; // Start from the top
   float z = static_cast<float>(rand() % 20 - 10);
    // float z = -10;
    int type = rand() % 2; // 0 for health, 1 for weapon strength
    pickables.push_back(new Pickable(x, y, z, type));
}

void drawPickables() {
    for (auto &pickable : pickables) {
        if (pickable->isActive()) {
            pickable->draw();
        }
    }
}

void updatePickables() {
    for (auto &pickable : pickables) {
        if (pickable->isActive()) {
            pickable->update();
        }
    }
}

void checkCollisions() {
    // Check collisions between enemy projectiles and player
    for (auto &enemy : enemies) {
        if (enemy->isActive()) {
            for (auto &projectile : enemy->projectiles) {
                if (projectile.isActive()) {
                    float distance = sqrt(pow(spaceCraft->getX() - projectile.getX(), 2) +
                                          pow(spaceCraft->getY() - projectile.getY(), 2) +
                                          pow(spaceCraft->getZ() - projectile.getZ(), 2));
                    if (distance < 1.0f) { // Collision threshold
                        spaceCraft->takeDamage(10); // Decrease player health
                        projectile.deactivate(); // Deactivate projectile
                    }
                }
            }
        }
    }

    // Check collisions between player projectiles and enemies
    for (auto &projectile : spaceCraft->projectiles) {
        if (projectile.isActive()) {
            for (auto &enemy : enemies) {
                if (enemy->isActive()) {
                    float distance = sqrt(pow(enemy->getX() - projectile.getX(), 2) +
                                          pow(enemy->getY() - projectile.getY(), 2) +
                                          pow(enemy->getZ() - projectile.getZ(), 2));
                    if (distance < 1.0f) { // Collision threshold
                        enemy->deactivate(); // Deactivate enemy
                        projectile.deactivate(); // Deactivate projectile
                        spaceCraft->increaseScore(100); // Increase player score
                    }
                }
            }
        }
    }
}

void checkPickableCollisions() {
    for (auto &pickable : pickables) {
        if (pickable->isActive()) {
            float distance = sqrt(pow(spaceCraft->getX() - pickable->x, 2) +
                                  pow(spaceCraft->getZ() - pickable->z, 2) + pow(pickable->y, 2));
//            cout << spaceCraft->getX() << " " << spaceCraft->getZ() << endl;
//            cout << pickable->x << " " << pickable->y << " " << pickable->z << endl;
//            cout << distance << endl;
            if (distance < 2.0) { // Collision threshold
                if (pickable->type == 0) {
                    cout << "health collision" << endl;
                    spaceCraft->increaseHealth(10); // Increase health
                } else {
                    cout << "weapon collision" << endl;
//                    spaceCraft->increaseWeaponStrength(); // Increase weapon strength
                    for (auto& projectile : spaceCraft->projectiles) {
                        if (projectile.isActive()) {
                            projectile.increaseStrength(1);
                        }
                    }
                }
                pickable->active = false; // Deactivate pickable
            }
        }
    }
}


void drawFrame(float x, float y, float width, float height) {
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawHUD() {
    // Set orthographic projection for HUD rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Adjust to your window size

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing for HUD rendering
    glDisable(GL_DEPTH_TEST);

    // Draw health bar squares
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < (float)spaceCraft->getHealth() / 10; ++i) {
        glBegin(GL_QUADS);
        glVertex2f(50 + (i * 20), 550);
        glVertex2f(65 + (i * 20), 550);
        glVertex2f(65 + (i * 20), 565);
        glVertex2f(50 + (i * 20), 565);
        glEnd();
    }

    // Draw frame for health bar
    drawFrame(45, 545, 210, 30);

    // Draw score with fancy frame
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(600, 550);
    string scoreText = "Score: " + to_string(spaceCraft->getScore());
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    drawFrame(590, 540, 150, 30);

    // Draw time remaining with fancy frame
    glRasterPos2f(600, 520);
    string timeText = "Time: " + to_string((int)timeRemaining);

    // Decrease time remaining
    timeRemaining -= (static_cast<int>(time(nullptr)) - currTime);
    currTime = static_cast<int>(time(nullptr));
    if (timeRemaining < 0) timeRemaining = 0; // Ensure it doesn't go below 0

    for (char c : timeText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    drawFrame(590, 510, 150, 30);

    // Restore previous state
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void generateEnemies(unsigned int* texture){
    // Add enemy spacecraft with different positions and angles
    enemies.emplace_back(new Spacecraft(0.0f, 0.0f, -5.0f, 0.0f, true, texture[9], texture[10]));
    enemies.emplace_back(new Spacecraft(5.0f, 0.0f, -7.0f, 0.0f, true, texture[9], texture[10]));
}

void drawEnemies() {
    for (auto &enemy : enemies) {
        if (enemy->isActive()) {
            enemy->draw();
        }
    }
}

void updateEnemies(float dt) {
    for (auto &enemy : enemies) {
        if (enemy->isActive()) {
            enemy->update(dt);
            // Shooting logic (e.g., shoot every few seconds)
            enemy->shoot();
        }
    }
}

// Function to draw all active projectiles
void drawProjectiles() {
    for (auto& projectile : spaceCraft->projectiles) {
        if (projectile.isActive()) {
            projectile.draw();
        }
    }
}

// Function to update all active projectiles
void updateProjectiles(float dt) {
    for (auto& projectile : spaceCraft->projectiles) {
        if (projectile.isActive()) {
            projectile.update(dt);
        }
    }
}

void Game::animate(int value = 0) {
    angle += 0.5;
    updateProjectiles(12);
    updateEnemies(12);
    //checkCollisions();
    updatePickables();
    checkPickableCollisions();
    if(angle > 360) angle = 0;

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - lastPickableGenerationTime > pickableGenerationInterval) {
        generatePickables();
        lastPickableGenerationTime = currentTime;
    }

    glutPostRedisplay();
    glutTimerFunc(animationPeriod, animate, value);
}

void Game::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // camera transformation
    gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // global transformation
    glPushMatrix();
    glTranslatef(0.0, 0.0, -6);
    glScalef(1.0, 1.0 * aspectRatio, 1.0);

    // Space Craft Setup and draw
    spaceCraft->setEnemy(false);
    spaceCraft->setup();

    for(auto& enemy : enemies){
        enemy->setup();
    }

    glPushMatrix();
    glTranslatef(spaceCraft->getX(), 0.0, spaceCraft->getZ());
    glRotatef(spaceCraft->getAngle(), 0.0, 1.0, 0.0);
    spaceCraft->draw();
    //drawEnemies();
    glPopMatrix();
    drawProjectiles();

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
    drawPickables();
    // Draw HUD
    drawHUD();
    
    glPopMatrix();
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

    currTime = static_cast<int>(time(nullptr));
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

    generateEnemies(texture);

    generatePickables();
    animate();
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
            spaceCraft->shoot();
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
    if (key == GLUT_KEY_RIGHT) tempAngle = craftAngle + 8.0;
    if (key == GLUT_KEY_LEFT) tempAngle = craftAngle - 8.0;
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

