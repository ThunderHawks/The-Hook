#ifndef IO_H_
#define IO_H_

extern float g_width, g_height;
extern int ShadeMode;

//Mouse press callback for Edit Mode
void glfwEditMouse(GLFWwindow *window, int button, int action, int mods);
//Mouse scroll callback for Edit Mode
void glfwEditScroll(GLFWwindow *window, double xOffset, double yOffset);
//Keyboard callback for Edit Mode
void glfwEditKeyboard(void);
//Keyboard callback for Game Mode
void glfwGameKeyboard(void);
//Mouse press callback for Edit Mode
void glfwEditMouse(GLFWwindow *window, int button, int action, int mods);
//Mousemove callback for Edit Mode
void glfwEditGetCursorPos(GLFWwindow *window, double xpos, double ypos);
//Mousemove callback for Game Mode
void glfwGameGetCursorPos(GLFWwindow *window, double xpos, double ypos);
//Mousemove callback for Edit Mode
void glfwEditKeyPress(GLFWwindow *window, int key, int scan, int action, int mods);
//Mousemove callback for Game mode
void glfwGameKeyPress(GLFWwindow *window, int key, int scan, int action, int mods);
//Returns bool of char pressed
int getPressed(char ch);

void glfwPlayMouse(GLFWwindow *window, int button, int action, int mods);
#endif
