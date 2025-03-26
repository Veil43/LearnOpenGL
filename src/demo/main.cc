#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <thread>
#include <chrono>

void GLErrorQuery(void);
#include "ch5_hello_triangle.h"
void GLQueryAttribCount(void);

void FramebufferSizeCallback(GLFWwindow*, int, int);
void ProcessInput(GLFWwindow *);
void ToggleWireframe(void);

int 
main(int argc, char **argv)
{
    std::cout << "Hello OpenGL Learner!" << std::endl;;
    // Initialize glfw
    if (!glfwInit())
    {
        std::cerr << "ERROR: Could Not initialize GLFW" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    const int kWWidth = 800;
    const int kWHeight = 600;
    GLFWwindow* window = glfwCreateWindow(kWWidth, kWHeight, "OpenGL Demo", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Load the functions from the opengl dll or so with GLAD using gladLoadGLLoader
    // You can pass it a function loader like glfwGetProcAddress or use gladLoadGL()
    // if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    // {
    //     std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
    //     glfwTerminate();
    //     return 1;
    // }

    if (!gladLoadGL())
    {
        std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Set the callback for window resizing
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    /*
    [STRANGE STRANGE OPENGL ERROR]
    block
    {
        GLint nr_attributes = 0;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nr_attributes);
        std::cout << "OpenGL supports: " << nr_attributes << " attributes on this machine" << std::endl;
    }
        
    Incredibly strange error happening here where the above code is causing ch5::Run() not to draw anything.
    This does not trigger a glError.

    What I've done:
    ==============
    To fix this I have: 
        moved the block after ch5::Start() and it has worked.
        placed std::this_thread::sleep_for(std::chrono::milliseconds(1)) before ch5::Start() and it has worked
        moved the block to a function GLQueryAttribCount() and it has worked
        placed a call to GLQueryAttribCount between the block and ch::Start() and it has worked
        removed the std::cout statement from the block and it has worked
        removed the nr_attributes from the std::cout statement and it has worked

    I thought this was some kind of concurrency/timing issue where OpenGL was not yet fully initiallized when we call
    glGetIntegerv leading to the draw calls not rendering as expected so I tried:
        placing a for(int i = GI_NUMBER; i > 0; i--); before ch5::start() and it did not work
        placing glGetError() before glGetIntegerv to force openGL to initialize missing states, and it did not work
        placing glFinish() before glGetIngergerv to finish the init process and it did not work

    So what the fuck is going here?????
    Why does printing nr_attributes cause opengl not to work as intended?
    I DO NOT KNOW.
    */

    GLQueryAttribCount();
    
    ch5::Start();
    // Render
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ch5::Run();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        ToggleWireframe();
    }
}

void 
FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ToggleWireframe()
{
    static bool line = false;
    u32 mode = 0;
    if (line)
        mode = GL_LINE;
    else
        mode = GL_FILL;

    glPolygonMode(GL_FRONT_AND_BACK, mode);
    line = !line;
}

void GLErrorQuery()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error: " << error << std::endl;
    }
}

inline void GLQueryAttribCount(void)
{
    int n_supported_gl_attributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n_supported_gl_attributes);
    std::cout << "OpenGL supports: " << n_supported_gl_attributes << " attributes on this machine" << std::endl;
}