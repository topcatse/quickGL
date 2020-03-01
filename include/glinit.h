#ifndef QUICKGL_GL_INIT_H_
#define QUICKGL_GL_INIT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__) 

extern GLuint vao;
extern int width;
extern int height;

GLenum checkGLError(const char * file, int line);
void printStatus(const char *step, GLuint context, GLuint status);

GLuint loadShader(std::string filename, GLenum shader_type);

class GLController
{
public:
    bool initQuickGL(int argc, char** argv);
    int render();
    void closeQuickGL();

private:
    GLFWwindow* m_window{nullptr};
};

void onDisplay(void);

void onResize(int w, int h);

void onClick(int button, int state, int x, int y);

void onMouseMove(int x, int y);

void onKeyPress(unsigned char key, int x, int y);

void onSpecialKeyPress(int key, int x, int y);


#endif


