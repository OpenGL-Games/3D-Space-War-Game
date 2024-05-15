#ifndef TEXT_MENU_H
#define TEXT_MENU_H
#include <GL/freeglut.h>

class Menu {
public:

    Menu();
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
#endif //TEXT_MENU_H
