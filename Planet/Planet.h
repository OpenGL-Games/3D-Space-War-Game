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


};

#endif //GAME_PLANET_H
