extern float g_height, g_width;

/* Input:  a vec3 vector
*  output: the magnitude of the vec3
*	Summary: finds the magnitude of a vec3 
*/ 
float magnitude(glm::vec3 vector);
// GLFW callback for any detected errors.
void glfwError(int error, const char *description);
// GLFW callback for when the window is resized.
void glfwWindowResize(GLFWwindow *window, int width, int height);
/*The next 4 functions do the followng with a pixel:
	convert the x if the x of the pixel to 
	convert the y if the y of the pixel to 
	convert the x if the x of the pixel to world
	convert the y if the y of the pixel to world
*/
float p2i_x(int p_x);
float p2i_y(int p_y);
float p2w_x(int p_x);
float p2w_y(int p_y);
/* Input:  two floats that determine the bounds of the random number
	Output: a random float in between the two numbers
	Summary: get a random float in between two numbers
*/
float RandomFloat(float a, float b);
