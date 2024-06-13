#ifndef GAME_PLANET_H
#define GAME_PLANET_H

#include <string>

using namespace std;

class Planet {
private:
    string textureFile;
    float radius;
    float centerX;


    static void loadTexture();
    void loadVertexArray();
    void setupPlanet();

public:
    Planet(string textureFile, float radius, float centerX); // Constructor
    void draw(); // Draw the planet
    void resize(int w, int h);
    void keyInput(unsigned char key, int x, int y);
    void specialKeyInput(int key, int x, int y);
    void drawPlanet(string planetName, unsigned int textureId);
    void fillVertexArray();
    void fillTextureCoordArray();
    void spinDisplay();
    float f(int i, int j); // Function to calculate f(i, j), used in fillVertexArray
    float g(int i, int j);
    float h(int i, int j);
//    void loadTextures(string file, unsigned int t);
//    void setup(void );

};

#endif //GAME_PLANET_H
