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
    Spacecraft();

    Spacecraft(unsigned int tex1, unsigned int tex2); // Constructor

    Spacecraft(float x, float y ,float z, float a,bool e, unsigned int tex1, unsigned int tex2);

    void draw(); // Draw method
    void updateEnemy(float xTarget, float zTarget); // Update method for animation
    void move(float dx, float dz); // Method to move the spacecraft
    void rotate(float angle_change); // Method to rotate the spacecraft
    void shoot(); // Method to shoot projectiles
    void takeDamage(int damage); // Method to decrease health
    void increaseHealth(int healthIncrease); // Method to increase health
    void deactivate() { active = false; }
    void increaseScore(int val);
    void setup();
    // Getters
    float getX() const { return xVal; }
    float getZ() const { return zVal; }
    float getY() const { return 0.0; }
    float getAngle() const { return angle; }
    int getHealth() const { return health; }
    int getScore() const { return score; }
    int getPower() const { return power; }
    bool isEnemy() const { return enemy; }
    // Setters
    void setX(float x) { xVal = x; }
    void setZ(float z) { zVal = z; }
    void setAngle(float a) { angle = a; }
    void setHealth(int h) { health = h; }
    void setScore(int s) { score = s; }
    void setPower(int p) { power = p; }
    void increasePower(int val) {power += val; }

    void setEnemy(bool e) { enemy = e; }
    void setTextures(unsigned int tx1, unsigned int tx2) { text1 = tx1; text2 = tx2; }
    unsigned int texture[2];
    vector<Projectile> projectiles; // Vector to store projectiles
    bool isActive() const { return active; }
    int count;
private:
    void fillVertexArray(void);

    void fillTextureCoordArray(void);

    void loadTextures(string file, unsigned int t);


    vector<float> vertices;
    vector<float> textureCoordinates;
    float xVal, zVal; // Coordinates of the spacecraft
    float angle; // Angle of the spacecraft
    int health; // Health of the spacecraft
    int score;
    bool enemy;
    bool active;
    int power = 1;
    unsigned int spacecraftModel; // Display list for the spacecraft model
    unsigned int text1;
    unsigned int text2;

    int lastShootTime; // Last shoot time in milliseconds

    static const int shootInterval = 3000; // 3 seconds in milliseconds

};

#endif // SPACECRAFT_H