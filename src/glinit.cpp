#include "../include/glinit.h"

#include <iostream>

#include "../include/shape.h"
#include "../include/camera.h"
#include "../include/tool_base.h"

using namespace std;

GLuint vao;
int width = 320;
int height = 240;


// TODO: Should there be a GLInterface class that controls the globals? 
//       e.g. It will store the active shapes list and active tools list? 
// 		 Maybe not?

GLenum checkGLError(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

void printStatus(const char *step, GLuint context, GLuint status){
	GLint result = GL_FALSE;
	CHECK_GL_ERROR();
	glGetShaderiv(context, status, &result);
	CHECK_GL_ERROR();
	if (result == GL_FALSE) {
		char buffer[1024];
		if (status == GL_COMPILE_STATUS)
			glGetShaderInfoLog(context, 1024, NULL, buffer);
		else
			glGetProgramInfoLog(context, 1024, NULL, buffer);
		if (buffer[0])
			fprintf(stderr, "%s: %s\n", step, buffer);
	}
}

void GLController::closeQuickGL(){
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        onKeyPress(key, 0, 0);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	onMouseMove(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	onClick(button, action,  0, 0);
}

void onResize(GLFWwindow*, int w, int h){
//	width = w; height = h;
	if (Shape::activeCamera) Shape::activeCamera->onResize(w,h); //glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void onClick(int button, int state, int x, int y){
	if (!Tool::activeTools.empty())	Tool::activeTools.front()->onClick(button, state, x, y);
}


void onMouseMove(int x, int y){
	if (!Tool::activeTools.empty())	Tool::activeTools.front()->onMouseMove(x, y);
}

void onKeyPress(unsigned char key, int x, int y){
	if (!Tool::activeTools.empty())	Tool::activeTools.front()->onKeyPress(key, x, y);
}

void onSpecialKeyPress(int key, int x, int y){
	if (!Tool::activeTools.empty())	Tool::activeTools.front()->onSpecialKeyPress(key, x, y);
}


bool GLController::initQuickGL(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	m_window = glfwCreateWindow(width, height, "mini", NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);

	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glewExperimental = GL_TRUE;
	glewInit();

	// BUFFERS etc

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glfwSetFramebufferSizeCallback(m_window, onResize);  
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    
	// tell GLFW to capture our mouse
    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

int GLController::render()
{
	glfwSwapInterval(1);

    while (!glfwWindowShouldClose(m_window))
    {
        processInput(m_window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Shape::activeCamera == NULL) {
			cout << "WARNING: No camera is active. Cannot draw." << endl;
			return 0;
		}

		for (auto it : Shape::allShapes) it->render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


