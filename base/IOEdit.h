#ifndef IOEDIT_H_
#define IOEDIT_H_

#define EDIT_MODE 1

//returns bool of character pressed takes the character for a given key
int getEPressed(char ch);
//Mouse press callback for Edit Mode
void glfwEditMouse(GLFWwindow *window, int button, int action, int mods);
//Mouse scroll callback for Edit Mode
void glfwEditScroll(GLFWwindow *window, double xOffset, double yOffset);
//Keyboard callback for Edit Mode
void glfwEditKeyboard(void);
//Mouse press callback for Edit Mode
void glfwEditMouse(GLFWwindow *window, int button, int action, int mods);
//Mousemove callback for Edit Mode
void glfwEditGetCursorPos(GLFWwindow *window, double xpos, double ypos);
//Mousemove callback for Edit Mode
void glfwEditKeyPress(GLFWwindow *window, int key, int scan, int action, int mods);
#endif
