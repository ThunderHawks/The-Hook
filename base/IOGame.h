#ifndef IOGAME_H_
#define IOGAME_H_

#include "SoundPlayer.h"

extern float g_width, g_height;
extern int ShadeMode, MAX_DISTANCE;
extern SoundPlayer musicPlayer;

/*
*  returns bool of character pressed
*  takes the character for a given key
*/
int getGPressed(char ch);
//Keyboard callback for Game Mode
void glfwGameKeyboard(void);
//Mousemove callback for Game Mode
void glfwGameGetCursorPos(GLFWwindow *window, double xpos, double ypos);
//Mousemove callback for Game mode
void glfwGameKeyPress(GLFWwindow *window, int key, int scan, int action, int mods);
//Returns bool of char pressed
int getPressed(char ch);
//Mouse scroll callback for Play mode to adjust camera
void glfwGameScroll(GLFWwindow *window, double xOffset, double yOffset);
void glfwGameMouse(GLFWwindow *window, int button, int action, int mods);
#endif
