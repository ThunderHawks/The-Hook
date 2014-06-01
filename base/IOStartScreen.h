#ifndef IOSS_H_
#define IOSS_H_

#define STARTSCREEN_MODE -1

//Mouse press callback for Start Screen Mode
void glfwStartScreenMouse(GLFWwindow *window, int button, int action, int mods);

//Mouse callback for keeping track of mouse position in Start Screen Mode
void glfwStartScreenGetCursorPos(GLFWwindow *window, double xpos, double ypos);

#endif
