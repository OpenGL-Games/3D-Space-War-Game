//
// Created by Mahmoud on 5/15/2024.
//

#ifndef GAME_GAME_H
#define GAME_GAME_H
#include <GL/freeglut.h>

class Game {
public:

    Game();
    void draw();
    void resize(int w, int h);
    void setup(void);
    void keyInput(unsigned char key, int x, int y);
    void specialKeyInput(int key, int x, int y);

private:

    int selectedOption;
    unsigned int texture[2];

    void drawText(int x, int y, const char* text, bool selected);
    void selectNext();
    void selectPrevious();
    void loadTextures();
};


#endif //GAME_GAME_H
