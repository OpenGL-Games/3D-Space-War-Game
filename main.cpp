#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Menu/menu.h"
#include "Texture/getBMP.h"
#include <windows.h>
#include <mmsystem.h>

using namespace std;

static unsigned int texture[2]; // Array of texture ids.

void loadTextures()
{
    // Local storage for image data.
    imageFile *image[1];

    // Load the image.
    image[0] = getBMP("..//Images//background.bmp");

    // Create texture object texture[0].
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->width, image[0]->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void loadSound() {
    PlaySound("..//Sounds//game.wav", NULL, SND_ASYNC);
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(2.0, 0.0, -8);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // Map the texture onto a square polygon.
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(10.0, -10.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(10.0, 10.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-10.0, 10.0, 0.0);
    glEnd();


    Menu mainMenu;
    mainMenu.draw();

    glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
    glClearColor(0.0352941f, 0.0431373f, 0.0705882f, 0.0);

    // Create texture ids.
    glGenTextures(1, texture);

    // Load external texture.
    loadTextures();

    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Turn on OpenGL texturing.
    glEnable(GL_TEXTURE_2D);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

void setWindowIcon() {
    // Load icon file
    auto hIcon = (HICON)LoadImage(nullptr, "..//Images//icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    if (hIcon == NULL) {
        // Error loading icon
        std::cerr << "Failed to load icon!" << std::endl;
        return;
    }
    // Set window icon
    HWND hWnd = FindWindow(NULL, "Space War"); // Replace "Space War" with your window title
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

// Main routine.
int main(int argc, char **argv)
{
    loadSound();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Space War");
    setWindowIcon();

    glutFullScreen();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
