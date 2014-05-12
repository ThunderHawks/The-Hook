extern float g_height, g_width;

float magnitude(glm::vec3 vector);
void glfwError(int error, const char *description);
void glfwWindowResize(GLFWwindow *window, int width, int height);
float p2i_x(int p_x);
float p2i_y(int p_y);
float p2w_x(int p_x);
float p2w_y(int p_y);
float RandomFloat(float a, float b);
