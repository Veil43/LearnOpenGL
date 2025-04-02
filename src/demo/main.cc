#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#define ENABLE_SAN 1

void GLQueryError(void);
#include "chcommon.h"
#include "ch5.h"
#include "ch6.h"
#include "ch7.h"
#include "ch8.h"
#include "ch9.h"

void FramebufferSizeCallback(GLFWwindow*, int, int);
void ProcessInput(GLFWwindow *);
void ToggleWireframe(void);
void GLQueryAttribCount(void);

int 
main(int argc, char **argv)
{
    std::cout << "Hello OpenGL Learner!" << std::endl;
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
    // if (!gladLoadGL())
    // {
    //     std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
    //     glfwTerminate();
    //     return 1;
    // }

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }

    int *ptr = (int*) malloc(3*sizeof(int));

    // Set the callback for window resizing
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    GLQueryAttribCount();
 
    Object3D obj5 = ch5::Start();
    ch_return ch6work = ch6::Start("../src/demo/shaders/ch6/ch6.vs", "../src/demo/shaders/ch6/ch6.fs");
    ch_return ch6exercise1 = ch6::Start("../src/demo/shaders/ch6/ch6.1.vs", "../src/demo/shaders/ch6/ch6.fs");
    ch_return ch6exercise2 = ch6::Start("../src/demo/shaders/ch6/ch6.2.vs", "../src/demo/shaders/ch6/ch6.fs");
    auto ch6ex = [&]() -> void 
    {
        ch6exercise2.shader.Use();
        ch6exercise2.shader.SetFloat("offset", 0.5);
    };
    ch6ex();
    ch_return ch7work = ch7::Start("../src/demo/shaders/ch7/ch7.vs", "../src/demo/shaders/ch7/ch7.fs");
    ch_return ch8work = ch8::Start(ch7work.obj, "../src/demo/shaders/ch8/ch8.vs", "../src/demo/shaders/ch8/ch8.fs");
    ch_return ch9work = ch9::Start((kWWidth/kWHeight), "../src/demo/shaders/ch9/ch9.vs", "../src/demo/shaders/ch9/ch9.fs");

    // Render
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ch9::Run(ch9work);

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

void GLQueryError()
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