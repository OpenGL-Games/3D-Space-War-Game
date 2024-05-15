#include "menu.h"
#include <cstdlib>
#include <iostream>
#include <GL/freeglut.h>

using namespace std;
Menu* Menu::instance = nullptr;

Menu::Menu() : selectedOption(0) {

}

void Menu::render() {
    drawText(-9, 1, "Survival Mode", (selectedOption == 0));
    drawText(-9, -2, "Time Attack Mode", (selectedOption == 1));
    drawText(-9, -5, "Exit", (selectedOption == 2));
}

void Menu::drawText(int x, int y, const char* text, bool selected) {
    glPushMatrix();
    glTranslatef(-13.0f, 0.0f + y, -15.0f);  // Shift selected option to the right
    glRasterPos2f(x, y);
    if (selected) {
        glTranslatef(2.0, 0, 0);
//        glScalef(0.007, 0.007, 1);
        glColor3f(0.0f, 0.0f, 1.0f);        // Red color for selected option
    }
    else {
        glColor3f(1.0f, 0.0f, 1.0f);        // Blue color for unselected option
    }
    glScalef(0.006,0.006,1);
    glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)text);
//    for (const char* c = text; *c != '\0'; ++c) {
//        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);  // Use a bitmap font
//    }
    glPopMatrix();
}

void Menu::selectNext() {
    selectedOption = (selectedOption + 1) % 3;
    glutPostRedisplay();
}

void Menu::selectPrevious() {
    selectedOption = (selectedOption - 1) % 3;
    glutPostRedisplay();
}

void Menu::keyboardWrapper(unsigned char key, int x, int y) {
    if (key == 13) { // Enter key
        if (instance->selectedOption == 2) {
            exit(0);
        }
        // Handle other selections (START_GAME, MODES)
        std::cout << "Selected option: " << instance->selectedOption << std::endl;
    }
    glutPostRedisplay();
}

void Menu::specialKeysWrapper(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN) {
        instance->selectNext();
    }
    else if (key == GLUT_KEY_UP) {
        instance->selectPrevious();
    }
    glutPostRedisplay();
}