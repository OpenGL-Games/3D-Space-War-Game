#ifndef TEXT_MENU_H
#define TEXT_MENU_H
#include <GL/freeglut.h>

class Menu {
public:

    Menu();
    void render();
    static void keyboardWrapper(unsigned char key, int x, int y);
    static void specialKeysWrapper(int key, int x, int y);

private:

    int selectedOption;
    static Menu* instance;

    void drawText(int x, int y, const char* text, bool selected);
    void selectNext();
    void selectPrevious();
    void executeOption();


};
#endif //TEXT_MENU_H
