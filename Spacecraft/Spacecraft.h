// Spacecraft.h

#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include <GL/freeglut.h>

class Spacecraft {
public:
    Spacecraft(); // Constructor
    void draw(); // Draw method
    void update(float delta_time); // Update method for animation
    void move(float dx, float dz); // Method to move the spacecraft
    void rotate(float angle_change); // Method to rotate the spacecraft
    void shoot(); // Method to shoot projectiles
    void takeDamage(int damage); // Method to decrease health
    void increaseHealth(int healthIncrease); // Method to increase health

    // Getters
    float getX() const { return xVal; }
    float getZ() const { return zVal; }
    float getAngle() const { return angle; }
    int getHealth() const { return health; }

private:
    float xVal, zVal; // Coordinates of the spacecraft
    float angle; // Angle of the spacecraft
    int health; // Health of the spacecraft
    unsigned int spacecraftModel; // Display list for the spacecraft model
};

#endif // SPACECRAFT_H
