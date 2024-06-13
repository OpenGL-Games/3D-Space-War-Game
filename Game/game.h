//
// Created by Mahmoud on 5/15/2024.
//

#ifndef GAME_GAME_H
#define GAME_GAME_H
#include <GL/freeglut.h>
#include <string>

class Game {
public:

    Game();

    void draw();

    void resize(int w, int h);

    void setup(void);

    void keyInput(unsigned char key, int x, int y);

    void specialKeyInput(int key, int x, int y);

private:
    float *vertices = NULL;
    float *textureCoordinates = NULL;
    unsigned int texture[12];
    int p = 200; // Number of grid columns.
    int q = 200; // Number of grid rows
    int R = 10;

    void fillVertexArray(void);

    void fillTextureCoordArray(void);

    float f(int i, int j);

    float g(int i, int j);

    float h(int i, int j);

    void loadTextures(std::string file, unsigned int t);

    void drawPlanet(std::string planetName, unsigned int textureId);
};


#endif //GAME_GAME_H