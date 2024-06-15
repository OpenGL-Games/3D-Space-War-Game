#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Menu/menu.h"
#include "Game/game.h"
#include <windows.h>
#include <mmsystem.h>

using namespace std;

// Create instances of Menu and Game classes
int windowChoice = 0;
Menu menu;
Game game;

// Display function
void displayFunc() {
    if(windowChoice == 0)
        menu.draw();
    else if(windowChoice == 1)
        game.draw();
}

// Keyboard function
void keyboardFunc(unsigned char key, int x, int y) {
    if(windowChoice == 0)
        menu.keyInput(key, x, y);
    else if(windowChoice == 1)
        game.keyInput(key, x, y);
}

// Special function
void specialFunc(int key, int x, int y) {
    if(windowChoice == 0)
        menu.specialKeyInput(key, x, y);
    else if(windowChoice == 1)
        game.specialKeyInput(key, x, y);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    if(windowChoice == 0)
        menu.resize(w, h);
    else if(windowChoice == 1)
        game.resize(w, h);
}

void setWindowIcon() {
    // Load icon file
    auto hIcon = (HICON)LoadImage(nullptr, "Images//icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    if (hIcon == nullptr) {
        // Error loading icon
        std::cerr << "Failed to load icon!" << std::endl;
        return;
    }
    // Set window icon
    HWND hWnd = FindWindow(NULL, "Space War"); // Replace "Space War" with your window title
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

void loadBackGroundSound() {
    // PlaySound("..//Sounds//game.wav", nullptr, SND_ASYNC);
}

// Main routine.
int main(int argc, char **argv)
{
    loadBackGroundSound();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Space War");
    setWindowIcon();

//    glutFullScreen();

    glewExperimental = GL_TRUE;
    glewInit();

    menu.setup();

    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    glutReshapeFunc(resize);
    glutSpecialFunc(specialFunc);

    glutMainLoop();
}