extern float g_width, g_height, beta, alpha, eyeAtx, eyeAty, eyeAtz;

void glfwKeyboard(void);
void glfwGetCursorPos(GLFWwindow *window, double xpos, double ypos);
void glfwKeyPress(GLFWwindow *window, int key, int scan, int action, int mods);
int getPressed(char ch);
