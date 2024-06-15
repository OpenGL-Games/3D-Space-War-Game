#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "game.h"
#include <windows.h>
#include <mmsystem.h>
#include <cmath>
#include "../Planet/Planet.h"
#include "../Asteriods/Asteriods.h"
#include "../SpaceCraft/SpaceCraft.h"
#include "../Pickable/Pickable.h"
#include "../Menu/menu.h"
#include <vector>
#include <ctime>
#include <algorithm>
#include <thread>

using namespace std;

// Globals.
extern int windowChoice;
extern Menu menu;

float aspectRatio; // Aspect ratio of the window
Planet *planets = nullptr;
Asteriods *asteriods = nullptr;
Spacecraft *spaceCraft = nullptr;
vector<Spacecraft *> enemies;

float angle = 0;
float craftAngle = 0;

//time variables
float timeRemaining = 300.0; // Start at 300 seconds
int currTime = 0;

vector<Pickable *> pickables;

int pickableGenerationInterval = 6000; // Generate new pickables every 10 seconds

int lastPickableGenerationTime = 0;

float explosionX = 0.0f, explosionZ = 0.0f;
bool isExplosion = false;
float explosionRadius = 0.0f, explosionMaxRadius = 8.0f;
std::time_t switchTimer;
bool isGameOver = false;
bool isWin = false;


struct Particle {
    float x, y, z;   // Position
    float dx, dy, dz; // Velocity
    float r, g, b;    // Color
    float life;       // Lifetime remaining

    Particle(float x, float y, float z, float dx, float dy, float dz, float r, float g, float b, float life)
            : x(x), y(y), z(z), dx(dx), dy(dy), dz(dz), r(r), g(g), b(b), life(life) {}
};

class ParticleSystem {
public:
    std::vector<Particle> particles;

    void generateExplosion(float x, float y, float z) {
        for (int i = 0; i < 100; ++i) {
            // Randomize direction of particles
            float dx = static_cast<float>(rand()) / RAND_MAX - 0.5f;
            float dy = static_cast<float>(rand()) / RAND_MAX - 0.5f;
            float dz = static_cast<float>(rand()) / RAND_MAX - 0.5f;
            particles.emplace_back(x, y, z, dx, dy, dz, 1.0f, 0.5f, 0.0f, 1.0f);
        }
    }

    void update(float dt) {
        for (auto &particle: particles) {
            particle.x += particle.dx * dt;
            particle.y += particle.dy * dt;
            particle.z += particle.dz * dt;
            particle.life -= dt;
        }
        particles.erase(std::remove_if(particles.begin(), particles.end(),
                                       [](const Particle &p) { return p.life <= 0; }),
                        particles.end());
    }

    void draw() {
        glDisable(GL_LIGHTING);
        glBegin(GL_POINTS);
        for (auto &particle: particles) {
            glColor3f(particle.r, particle.g, particle.b);
            glVertex3f(particle.x, particle.y, particle.z);
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
};

ParticleSystem particleSystem; // Instance of ParticleSystem

Game::Game() = default;

void loadGameOverSound() {
    PlaySound("..//Sounds//game-over.wav", nullptr, SND_ASYNC);
}

void loadDeathSound() {
    PlaySound("..//Sounds//death.wav", nullptr, SND_ASYNC);
}

void loadWinSound() {
    PlaySound("..//Sounds//win.wav", nullptr, SND_ASYNC);
}

void loadEnemyEliminateSound() {
    PlaySound("..//Sounds//enemy-eliminate.wav", nullptr, SND_ASYNC);
}


void gameOver() {
    spaceCraft->deactivate();
    explosionX = spaceCraft->getX();
    explosionZ = spaceCraft->getZ();
    isExplosion = true;
    particleSystem.generateExplosion(0, 0.0, 0);
    loadGameOverSound();
    isGameOver = true;
    switchTimer = time(nullptr);
}

void generatePickables() {
    float x = static_cast<float>(rand() % 20 - 10);
    // float x = 0;
    float y = 0.0f; // Start from the top
    float z = static_cast<float>(rand() % 20 - 10);
    // float z = -10;
    int type = rand() % 2; // 0 for health, 1 for weapon strength
//    int type = 0;
    pickables.push_back(new Pickable(x, y, z, type));
}

void drawPickables() {
    if (spaceCraft->isActive()) {
        for (auto &pickable: pickables) {
            if (pickable->isActive() && spaceCraft->isActive()) {
                pickable->draw();
            }
        }
    }
}

void updatePickables() {
    if (spaceCraft->isActive()) {
        for (auto &pickable: pickables) {
            if (pickable->isActive()) {
                pickable->update();
            }
        }
    }
}

bool isRemainingEnemy() {
    int rem = 0;
    for (auto &enemy: enemies) {
        if (enemy->isActive()) {
            rem++;
        }
    }
    return rem > 0;
}

void checkCollisionWithEnemies() {
    for (auto &enemy: enemies) {
        if (enemy->isActive() && spaceCraft->isActive()) {

            float distance = sqrt(pow(spaceCraft->getX() - enemy->getX(), 2) +
                                  pow(spaceCraft->getY() - enemy->getY(), 2) +
                                  pow(spaceCraft->getZ() - enemy->getZ(), 2));
            if (distance < 4.0f) { // Collision threshold
                gameOver();
            }
        }


    }
}

void checkProjectileCollisions() {
    // Check collisions between enemy projectiles and player
    if (!spaceCraft->isActive()) return;

    for (auto &enemy: enemies) {
        if (enemy->isActive()) {
            for (auto &projectile: enemy->projectiles) {
                if (projectile.isActive() && spaceCraft->isActive()) {
                    float distance = sqrt(pow(spaceCraft->getX() - projectile.getX(), 2) +
                                          pow(spaceCraft->getY() - projectile.getY(), 2) +
                                          pow(spaceCraft->getZ() - projectile.getZ(), 2));
                    if (distance < 2.0f) { // Collision threshold
                        bool isDied = spaceCraft->takeDamage(10); // Decrease player health
                        if (isDied) {
                            gameOver();
                        }
                        projectile.deactivate(); // Deactivate projectile
                    }
                }
            }
        }
    }

    // Check collisions between player projectiles and enemies
    for (auto &projectile: spaceCraft->projectiles) {
        if (projectile.isActive()) {
            for (auto &enemy: enemies) {
                if (enemy->isActive()) {
                    float distance = sqrt(pow(enemy->getX() - projectile.getX(), 2) +
                                          pow(enemy->getY() - projectile.getY(), 2) +
                                          pow(enemy->getZ() - projectile.getZ(), 2));
                    if (distance < 2.0f) { // Collision threshold
                        bool isDied = enemy->takeDamage(projectile.getStrength());
                        projectile.deactivate(); // Deactivate projectile
                        if (isDied) {
                            spaceCraft->increaseScore(100); // Increase player score
                            explosionX = enemy->getX();
                            explosionZ = enemy->getZ();
                            isExplosion = true;
                            particleSystem.generateExplosion(0, 0.0, 0);
                            if (isRemainingEnemy()) {
                                loadDeathSound();
                            }
                        }
                    }
                }
            }
        }
    }
}

void getPlanetPosition(float angle, int planetIndex, float &xPos, float &yPos, float &zPos) {
    vector<int> x = {0, 30, 50, 150, -90, -110, -130, 50, -150, 125};
    vector<int> z = {-150, -140, -110, -90, -70, -200, -210, -70, -50, -90};


    // Orbital and rotational speeds (relative to some base speed)
    float orbitalSpeeds[] = {0.0f, 4.74f, 3.50f, 2.98f, 2.41f, 1.31f, 0.97f, 0.68f, 0.54f, 2.98f};

    if (planetIndex < 0 || planetIndex >= 10) {
        std::cerr << "Invalid planet index: " << planetIndex << std::endl;
        return;
    }
    // Other planets: Rotate around the Sun
    float orbitalAngle = angle * orbitalSpeeds[planetIndex] * (M_PI / 180.0f);

    if (planetIndex == 0) {
        // Sun's position
        xPos = x[planetIndex] * 0.3;
        zPos = z[planetIndex] * 0.3;
    } else {
//        glTranslatef(x[3] - x[0], 0, z[3] - z[0]); // Translate to Earth's position
//        glRotatef(angle * orbitalSpeeds[i] * ORBITAL_SPEED_SCALE, 0, 1, 0); // Rotate around the Earth
//        glTranslatef(x[i] - x[3], 0, z[i] - z[3]); // Translate to the Moon's orbit posit

        if (planetIndex == 9) {
//            xPos = x[planetIndex] - x[3];
//            zPos = z[planetIndex] - z[3];

            xPos = (x[planetIndex] - x[3]) * cos(orbitalAngle) + (z[planetIndex] - z[3]) * sin(orbitalAngle);
            zPos = (x[planetIndex] - x[3]) * -sin(orbitalAngle) + (z[planetIndex] - z[3]) * cos(orbitalAngle);

            xPos += x[3] - x[0];
            zPos += z[3] - z[0];
        }
        xPos = (x[planetIndex] - x[0]) * cos(orbitalAngle) + (z[planetIndex] - z[0]) * sin(orbitalAngle);
        zPos = (x[planetIndex] - x[0]) * -sin(orbitalAngle) + (z[planetIndex] - z[0]) * cos(orbitalAngle);

        xPos += x[0];
        zPos += z[0];

        xPos *= 0.3f;
        zPos *= 0.3f;
    }
}

void checkPlanetCollisionsWithSpacecraft() {
    for (int i = 0; i < 10 && spaceCraft->isActive(); ++i) {
        float planetX, planetY, planetZ;
        getPlanetPosition(angle, i, planetX, planetY, planetZ);

        float distance = sqrt(pow(spaceCraft->getX() - planetX, 2) +
                              pow(spaceCraft->getZ() - planetZ, 2));
        if (distance < 5.0) { // Collision threshold
            gameOver();
        }
    }
}

void checkPlanetEnemyCollision() {
    if (!spaceCraft->isActive()) return;

    for (auto &enemy: enemies) {
        if (enemy->isActive()) {

            for (int i = 0; i < 10; ++i) {
                Planet &planet = planets[i];

                float planetX, planetY, planetZ;
                getPlanetPosition(angle, i, planetX, planetY, planetZ);

                float distance = sqrt(pow(enemy->getX() - planetX, 2) +
                                      pow(enemy->getZ() - planetZ, 2));

                if (distance < 5.0) {
                    enemy->deactivate();
                    explosionX = enemy->getX();
                    explosionZ = enemy->getZ();
                    isExplosion = true;
                    particleSystem.generateExplosion(0, 0.0, 0);
                    loadEnemyEliminateSound();

                }
            }
        }
    }

}

void pickableSound() {
    PlaySound("..//Sounds//pickable-object.wav", nullptr, SND_ASYNC);
}

void checkPickableCollisions() {
    if(!spaceCraft->isActive()) return;

    for (auto &pickable: pickables) {
        if (pickable->isActive()) {
            float distance = sqrt(pow(spaceCraft->getX() - pickable->x, 2) +
                                  pow(spaceCraft->getZ() - pickable->z, 2) + pow(spaceCraft->getY() - pickable->y, 2));

            if (distance < 2.0) { // Collision threshold
                pickableSound();

                if (pickable->type == 0) {
                    spaceCraft->increaseHealth(10); // Increase health
                } else {
                    spaceCraft->increasePower(10);
                }
                pickable->active = false; // Deactivate pickable
//                pickables.erase(pickables.begin() + i);
//                delete pickable;

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
    for (int i = 0; i < (float) spaceCraft->getHealth() / 10; ++i) {
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
    for (char c: scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    drawFrame(590, 540, 150, 30);

    // Draw time remaining with fancy frame
    if (menu.getSelectedOption() == 1) {
        glRasterPos2f(600, 520);
        string timeText = "Time: " + to_string((int) timeRemaining);

        // Decrease time remaining
        timeRemaining -= (static_cast<int>(time(nullptr)) - currTime);
        currTime = static_cast<int>(time(nullptr));
        if (timeRemaining < 0) timeRemaining = 0; // Ensure it doesn't go below 0

        for (char c: timeText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
        drawFrame(590, 510, 150, 30);
    }

    // Restore previous state
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

bool checkSurvivalWin() {
    if (isRemainingEnemy() || menu.getSelectedOption() != 0) return false;
    if (isWin) return false;
    if (!spaceCraft->isActive()) return false;
    loadWinSound();
    isWin = true;
    switchTimer = time(nullptr);
    spaceCraft->deactivate();
    return true;
}

bool checkTimeAttackerWin() {
    if (!spaceCraft->isActive()) return false;

    if (timeRemaining > 0) {
        if (!isRemainingEnemy() && !isWin) {
            loadWinSound();
            isWin = true;
            switchTimer = time(nullptr);
            spaceCraft->deactivate();
            return true;
        }
    } else if (!isWin) {
        //game over
        if (!isGameOver) {
            spaceCraft->deactivate();
            loadGameOverSound();
            isGameOver = true;
            switchTimer = time(nullptr);
        }
    }
    return false;
}

void generateEnemies(unsigned int *texture) {
    // Add enemy spacecraft with different positions and angles
    enemies.emplace_back(new Spacecraft(-25.0f, 0.0f, 10.0f, 0.0f, true, texture[9], texture[10]));
    enemies.emplace_back(new Spacecraft(25.0f, 0.0f, 10.0f, 0.0f, true, texture[9], texture[10]));
    enemies.emplace_back(new Spacecraft(-20.0f, 0.0f, 10.0f, 0.0f, true, texture[9], texture[10]));
    enemies.emplace_back(new Spacecraft(20.0f, 0.0f, 10.0f, 0.0f, true, texture[9], texture[10]));

    enemies.emplace_back(new Spacecraft(-15.0f, 0.0f, 0.0f, 0.0f, true, texture[9], texture[10]));
    enemies.emplace_back(new Spacecraft(15.0f, 0.0f, 0.0f, 0.0f, true, texture[9], texture[10]));
//    enemies.emplace_back(new Spacecraft(-15.0f, 0.0f, 0.0f, 0.0f, true, texture[9], texture[10]));
//    enemies.emplace_back(new Spacecraft(15.0f, 0.0f, 0.0f, 0.0f, true, texture[9], texture[10]));
//    enemies.emplace_back(new Spacecraft(-15.0f, 0.0f, -20.0f, 0.0f, true, texture[9], texture[10]));
//    enemies.emplace_back(new Spacecraft(15.0f, 0.0f, -20.0f, 0.0f, true, texture[9], texture[10]));
//    enemies.emplace_back(new Spacecraft(-25.0f, 0.0f, -20.0f, 0.0f, true, texture[9], texture[10]));
//    enemies.emplace_back(new Spacecraft(25.0f, 0.0f, -20.0f, 0.0f, true, texture[9], texture[10]));
}

void drawEnemies() {
    if(!spaceCraft->isActive()) return;

    for (auto &enemy: enemies) {
        if (enemy->isActive()) {
            enemy->setup();
            enemy->draw();
        }
    }
}

void updateEnemies() {
    if(!spaceCraft->isActive()) return;

    for (auto &enemy: enemies) {
        if (enemy->isActive()) {
            enemy->updateEnemy(spaceCraft->getX(), spaceCraft->getZ());
            // Shooting logic (e.g., shoot every few seconds)
            enemy->shoot();
        }
    }
}

// Function to draw all active projectiles
void drawProjectiles() {
    if(!spaceCraft->isActive()) return;

    if (spaceCraft->isActive()) {
        for (auto &projectile: spaceCraft->projectiles) {
            if (projectile.isActive()) {
                projectile.draw();
            }
        }
    }

    for (auto &enemy: enemies) {
        if (enemy->isActive()) {
            for (auto &projectile: enemy->projectiles) {
                if (projectile.isActive()) {
                    projectile.draw();

                }
            }
        }
    }

}

// Function to update all active projectiles
void updateProjectiles(float dt) {
    if(!spaceCraft->isActive()) return;

    if (spaceCraft->isActive()) {
        for (auto &projectile: spaceCraft->projectiles) {
            if (projectile.isActive()) {
                projectile.update(dt);
            }
        }
    }
    for (auto &enemy: enemies) {
        if (enemy->isActive()) {
            for (auto &projectile: enemy->projectiles) {
                if (projectile.isActive()) {
                    projectile.update(dt);
                }
            }
        }
    }
}

void Game::animate(int value = 0) {
    angle += 0.5;
    updateProjectiles(12);
    updateEnemies();
    checkProjectileCollisions();
    checkCollisionWithEnemies();
    updatePickables();
    checkPickableCollisions();
    if (menu.getSelectedOption() == 0) checkSurvivalWin();
    else if (menu.getSelectedOption() == 1) checkTimeAttackerWin();

    checkPlanetCollisionsWithSpacecraft();
    checkPlanetEnemyCollision();


//    checkPlanetCollisionsWithSpacecraft();

//    if(isExplosion){
//        explosionRadius += 0.1;
//        if(explosionRadius > explosionMaxRadius){
//            isExplosion = false;
//            explosionRadius = 0.0;
//        }
//    }

// Handle explosion animation
    if (isExplosion) {
        particleSystem.update(0.016f); // Update particles with a delta time (adjust as needed)
        explosionRadius += 0.1f; // Increase explosion radius
        if (explosionRadius > explosionMaxRadius) {
            isExplosion = false;
            explosionRadius = 0.0f;
            particleSystem.particles.clear(); // Clear particles after explosion
        }
    }

    if (angle > 360) angle = 0;

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - lastPickableGenerationTime > pickableGenerationInterval) {
        generatePickables();
        lastPickableGenerationTime = currentTime;
    }

    glutPostRedisplay();
    glutTimerFunc(animationPeriod, animate, value);
}

void drawWindow(string str) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Adjust to your window size

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing for HUD rendering
    glDisable(GL_DEPTH_TEST);

    // Draw score with frame
    glClearColor(0.9f, 0.8f, 0.8f, 0.0);
    glColor3f(0.0f, 1.0f, 0.0f);
    drawFrame(300, 350, 200, 30);

    glRasterPos2f(355, 360);
    string scoreText = str;
    for (char c: scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }



    // Restore previous state
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void Game::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    int miniMapSize = windowHeight / 2.5; // Size of the mini map (1/4 of window height)

    // Retrieve spacecraft position and orientation
    float spaceCraftX = spaceCraft->getX();
    float spaceCraftZ = spaceCraft->getZ();
    float spaceCraftAngle = spaceCraft->getAngle();

    // Calculate the direction vector the spacecraft is facing
    float dirX = sin(spaceCraftAngle * M_PI / 180.0);
    float dirZ = cos(spaceCraftAngle * M_PI / 180.0);

    // Calculate the camera position to be behind the spacecraft
    float cameraX = spaceCraftX - dirX * 15;
    float cameraZ = spaceCraftZ - dirZ * 15;
    float cameraY = 2.0;  // Set the camera height above the ground

    // Calculate the look-at point to be the current position of the spacecraft
    float lookAtX = spaceCraftX;
    float lookAtZ = spaceCraftZ;
    float lookAtY = 0.0;  // Look-at height (ground level)

    glViewport(0, 0, windowWidth, windowHeight); // Adjust to the full window size
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the camera position and look-at point using gluLookAt
    gluLookAt(
            cameraX, cameraY + 5, cameraZ, // Camera position
            lookAtX, lookAtY, lookAtZ, // Look-at point
            0.0, 1.0, 0.0              // Up vector
    );
    // global transformation
    glPushMatrix();
    glTranslatef(0.0, 0.0, -6);
    glScalef(1.0, 1.0 * aspectRatio, 1.0);

    // Space Craft Setup and draw
    spaceCraft->setEnemy(false);
    spaceCraft->setup();

//    glPushMatrix();
//    glTranslatef(spaceCraft->getX(), 0.0, spaceCraft->getZ());
//    glRotatef(spaceCraft->getAngle(), 0.0, 1.0, 0.0);
    if (isWin || isGameOver) {
        std::time_t currentTime = std::time(nullptr);
        if (std::difftime(currentTime, switchTimer) >= 3) {
            if (isWin) {
                drawWindow("WINNER!!! :))");
                glutSwapBuffers();
                return;
            }
            if (isGameOver) {
                drawWindow("GAME OVER!!!");
                glutSwapBuffers();
                return;
            }
        }
    }
    spaceCraft->draw();
    //    glPopMatrix();

    drawProjectiles();

    drawEnemies();


    // planets setup and draw
    planets = new Planet[10];
    for (int i = 0; i < 10; i++) {
        planets[i].textureID = texture[i];
        planets[i].planetName = planetNames[i];
        planets[i].setup();
    }
    Planet::drawPlanets(planets, angle);


    drawProjectiles();


    // draw asteroids
    asteriods->draw();
    drawPickables();
    // Draw HUD
    drawHUD();
    // Draw explosion
    if (isExplosion) {
        //        glPushMatrix();
        //        glTranslatef(explosionX, 0.0, explosionZ);
        //        glColor3f(1.0, 0.0, 0.0);
        //        glutSolidSphere(explosionRadius, 20, 20);
        //        glPopMatrix();

        glPushMatrix();
        glTranslatef(explosionX, 0.0, explosionZ);
        glScalef(50, 50, 50);
        glPointSize(5);
        particleSystem.draw();
        glPopMatrix();
    }
    glPopMatrix();





    // Set orthographic projection for HUD rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight); // Adjust to your window size

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing for HUD rendering
    glDisable(GL_DEPTH_TEST);


    glLineWidth(2.0);
    glColor3f(1.0, 1.0, 0.0); // Set color to yellow
    glBegin(GL_LINE_LOOP);
    glVertex2f(windowWidth - miniMapSize - 10, 10);
    glVertex2f(windowWidth - 10, 10);
    glVertex2f(windowWidth - 10, miniMapSize + 10);
    glVertex2f(windowWidth - miniMapSize - 10, miniMapSize + 10);
    glEnd();


    // Restore previous state
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // end of main viewport

    // Second viewport for mini-map
    glViewport(windowWidth - miniMapSize - 10, 10, miniMapSize, miniMapSize); // 10 pixel padding
    glLoadIdentity();
    glClear(GL_DEPTH_BUFFER_BIT);

    // Set up camera for the mini-map view (top-down view)
    gluLookAt(
            0.0, 50.0, -200.0, // Camera position above the origin
            0.0, 0.0, -150,     // Look-at point at the origin
            0.0, 1.0, 0.0      // Up vector (looking down)
    );


    // Draw the entire scene for the mini-map view
    glPushMatrix();
    glScalef(2, 2, 2);
    Planet::drawPlanets(planets, angle);
//    asteriods->draw();
//    drawPickables();
    // Spacecraft Setup and draw
    spaceCraft->setEnemy(false);
    spaceCraft->setup();
    glPushMatrix();
    glTranslatef(spaceCraft->getX(), 0.0, spaceCraft->getZ());
    glRotatef(spaceCraft->getAngle(), 0.0, 1.0, 0.0);
    spaceCraft->draw();
    glPopMatrix();

    glPopMatrix();


    glutSwapBuffers();
}


void Game::resize(int w, int h) {
//    glViewport(0, 0, w, h);
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
    aspectRatio = (float) screenWidth / (float) screenHeight;

    // Load textures
    glGenTextures(12, texture);

    // Asteroids instance
    asteriods = new Asteriods();

    // spacecraft instance
    spaceCraft = new Spacecraft(texture[10], texture[11]);

    generateEnemies(texture);

    generatePickables();
    animate();
}


void shootSound() {
    PlaySound("..//Sounds//shoot.wav", nullptr, SND_ASYNC);
}


// handle ESC and Shoot
void Game::keyInput(unsigned char key, int x, int y) {
    switch (key) {
        // ESC key
        case 27:
            exit(0);
            break;
            // Shoot
        case ' ': {
            if (spaceCraft->isActive()) {
                spaceCraft->shoot();

                shootSound();
                glutPostRedisplay();
            }

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

    if (key == GLUT_KEY_RIGHT) tempAngle = craftAngle - 8.0;
    if (key == GLUT_KEY_LEFT) tempAngle = craftAngle + 8.0;
    if (key == GLUT_KEY_DOWN) {
        tempxVal = spaceCraft->getX() - sin(craftAngle * M_PI / 180.0);
        tempzVal = spaceCraft->getZ() - cos(craftAngle * M_PI / 180.0);
    }
    if (key == GLUT_KEY_UP) {
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

