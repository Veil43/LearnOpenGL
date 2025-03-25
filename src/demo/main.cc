#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

#include "ch5_hello_triangle.h"

void FramebufferSizeCallback(GLFWwindow*, int, int);
void ProcessInput(GLFWwindow *);

int 
main(int argc, char **argv)
{
    std::cout << "Hello OpenGL Learner!\n";
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

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }
    // Set the callback for window resizing
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

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
}

void 
FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
