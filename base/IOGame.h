#ifndef IOGAME_H_
#define IOGAME_H_

#include "SoundPlayer.h"

extern float g_width, g_height;
extern int ShadeMode;
extern SoundPlayer musicPlayer;

//Keyboard callback for Game Mode
void glfwGameKeyboard(void);
//Mousemove callback for Game Mode
void glfwGameGetCursorPos(GLFWwindow *window, double xpos, double ypos);
//Mousemove callback for Game mode
void glfwGameKeyPress(GLFWwindow *window, int key, int scan, int action, int mods);
//Returns bool of char pressed
int getPressed(char ch);

void glfwPlayMouse(GLFWwindow *window, int button, int action, int mods);
#endif
