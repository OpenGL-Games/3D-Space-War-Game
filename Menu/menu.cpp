#include "menu.h"
#include <cstdlib>
#include <iostream>

Menu::Menu() : selectedOption(0) {}

void Menu::draw() {
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(0, 6, "Space War", (selectedOption == -1));
    drawText(0, 5, "Start Game", (selectedOption == 0));
    drawText(0, 2, "Exit", (selectedOption == 1));
}

void Menu::drawText(int x, int y, const char* text, bool selected) {
    glPushMatrix();
    glTranslatef(0, 0, -40);
    glScalef(4,4, 4);

    glRasterPos2f(x, y);
    if (selected) {
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text);
    glPopMatrix();
}

void Menu::selectNext() {
    selectedOption = (selectedOption + 1) % 2;
}

void Menu::selectPrevious() {
    selectedOption = (selectedOption + 1) % 2;
}

void Menu::executeOption() {
    if (selectedOption == 0) {
        // Implement start game action here
        std::cout << "START GAME" << "\n";
    }
    else if (selectedOption == 1) {
        exit(0); // Exit the program
        std::cout << "EXIT GAME" << "\n";
    }
}

Menu mainMenu; // Declare an instance of the menu
