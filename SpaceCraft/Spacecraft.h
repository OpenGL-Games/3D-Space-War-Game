// Spacecraft.h

#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include <GL/freeglut.h>
#include <vector>
#include <string>
#include "../Projectile/Projectile.h"

using namespace std;

class Spacecraft {
public:
    Spacecraft(unsigned int tex1, unsigned int tex2); // Constructor

    void draw(); // Draw method
    void update(float delta_time); // Update method for animation
    void move(float dx, float dz); // Method to move the spacecraft
    void rotate(float angle_change); // Method to rotate the spacecraft
    void shoot(); // Method to shoot projectiles
    void takeDamage(int damage); // Method to decrease health
    void increaseHealth(int healthIncrease); // Method to increase health
    void increaseScore(int val);
    void setup();
    // Getters
    float getX() const { return xVal; }
    float getZ() const { return zVal; }
    float getAngle() const { return angle; }
    int getHealth() const { return health; }
    int getScore() const { return score; }
    bool isEnemy() const { return enemy; }
    // Setters
    void setX(float x) { xVal = x; }
    void setZ(float z) { zVal = z; }
    void setAngle(float a) { angle = a; }
    void setHealth(int h) { health = h; }
    void setScore(int s) { score = s; }

    void setEnemy(bool e) { enemy = e; }
    unsigned int texture[2];
    vector<Projectile> projectiles; // Vector to store projectiles

    int count;
private:
    void fillVertexArray(void);

    void fillTextureCoordArray(void);

    vector<float> vertices;
    vector<float> textureCoordinates;
    float xVal, zVal; // Coordinates of the spacecraft
    float angle; // Angle of the spacecraft
    int health; // Health of the spacecraft
    int score;
    bool enemy;
    unsigned int spacecraftModel; // Display list for the spacecraft model
};

#endif // SPACECRAFT_H