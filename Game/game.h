//
// Created by Mahmoud on 5/15/2024.
//

#ifndef GAME_GAME_H
#define GAME_GAME_H
#include <GL/freeglut.h>
#include <string>
#include <vector>

using namespace std;

class Game {
public:

    Game();

    void draw();

    void static resize(int w, int h);

    void setup(void);

    void keyInput(unsigned char key, int x, int y);

    void specialKeyInput(int key, int x, int y);

    vector<string> planetNames = {"sun", "mercury", "venus", "earth", "mars", "jupiter", "saturn", "uranus", "neptune", "moon"};

private:

    static const int animationPeriod = 100;
    unsigned int texture[12];

    void static animate(int value);

};


#endif //GAME_GAME_H