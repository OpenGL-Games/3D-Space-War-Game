#ifndef TEXT_MENU_H
#define TEXT_MENU_H
#include <GL/freeglut.h>

class Menu {
public:

    Menu();

    void draw();

    void selectNext();

    void selectPrevious();

    void executeOption();

private:
    enum MenuOption {
        START_GAME,
        MODES,
        EXIT,
        NUM_OPTIONS
    };

    int selectedOption;



    void drawText(int x, int y, const char* text, bool selected);
};
#endif //TEXT_MENU_H
