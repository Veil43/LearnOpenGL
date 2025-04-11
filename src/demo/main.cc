#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include <thread>
#include <chrono>

#define ENABLE_SAN 1

// Set GLFWCursorCallback to your own function and it will be call automatically by the windowing system
#define DECL_GLFW_CURSOR_CALLBACK(name) void name(GLFWwindow* window, double xpos, double ypos)
DECL_GLFW_CURSOR_CALLBACK(GLFWDefaultCursorCallback)
{
    std::cerr << "WARNING: Not capturing the cursor callback.\n";
    return;
}
typedef DECL_GLFW_CURSOR_CALLBACK(glfw_cursor_callback_type);
static glfw_cursor_callback_type* GLFWCursorCallback = GLFWDefaultCursorCallback;

#define DECL_GLFW_SCROLL_CALLBACK(name) void name(GLFWwindow* window, double xoffset, double yoffset)
DECL_GLFW_SCROLL_CALLBACK(GLFWDefaultScrollCallback)
{
    std::cerr << "WARNING: Not capturing the scroll callback.\n";
    return;
}
typedef DECL_GLFW_SCROLL_CALLBACK(glfw_scroll_callback_type);
static glfw_scroll_callback_type* GLFWScrollCallback = GLFWDefaultScrollCallback;

#define DECL_GLFW_MOVEMENT_CALLBACK(name) void name(GLFWwindow* window)
DECL_GLFW_MOVEMENT_CALLBACK(GLFWDefaultMovementCallback)
{
    std::cerr << "WARNING: Not capturing the movement callback.\n";
    return;
}
typedef DECL_GLFW_MOVEMENT_CALLBACK(glfw_movement_callback_type);
static glfw_movement_callback_type* GLFWMovementCallback = GLFWDefaultMovementCallback;

void GLQueryError(void);
static GLFWwindow* globWindow = nullptr;
#include "chcommon.h"
#include "ch5.h"
#include "ch6.h"
#include "ch7.h"
#include "ch8.h"
#include "ch9.h"
#include "ch10.h"
#include "ex10.h"
#include "lighting12-18.h"

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
    const int kAspectRatio = (kWWidth/kWHeight);
    GLFWwindow* window = glfwCreateWindow(kWWidth, kWHeight, "OpenGL Demo", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    globWindow = window;
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
    // CHAPTER 10 USES CHAPTER 9's WORK
    // ch10::Start(window);
    ex10::Start1(window); // << Excercises for chapter 10
    // lighting_return ch12work = lighting::Start((kWWidth/kWHeight), "../src/demo/shaders/lighting/vertex.vs", "../src/demo/shaders/lighting/vertex.vs", "../src/demo/shaders/lighting/phong.fs", "../src/demo/shaders/lighting/light.fs");
    // lighting_return ch13ex = lighting::Start((kWWidth/kWHeight), "../src/demo/shaders/lighting/gouraud.vs", "../src/demo/shaders/lighting/vertex.vs", "../src/demo/shaders/lighting/gouraud.fs", "../src/demo/shaders/lighting/light.fs");

    lighting_return ch14work = lighting::Start
    (
        kAspectRatio,
        "../src/demo/shaders/ch14/phong.vs",
        "../src/demo/shaders/ch14/phong.vs",
        "../src/demo/shaders/ch14/phong.fs",
        "../src/demo/shaders/ch14/light.fs"
    );

    // Set the callback for window resizing
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, GLFWCursorCallback);
    glfwSetScrollCallback(window, GLFWScrollCallback);

    // Timing
    f64 last_time = glfwGetTime();
    f64 delta_time = 0.0f;
    // Render
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ex10::Run1(ch9work, kWWidth/kWHeight, delta_time); << Cool FPS stroll in the void
        lighting::Run(ch14work, delta_time);

        glfwSwapBuffers(window);
        glfwPollEvents();
        f64 current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;
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
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        ToggleWireframe();
    }
    ch10::Ch10ProcessInput(window);
    ex10::Ex10ProcessInput(window);
    GLFWMovementCallback(window);
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
        glfwSetInputMode(globWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        std::cout << "OpenGL Error: 0x" << std::hex <<  error << std::endl;
        __builtin_trap();
    }
}

inline void GLQueryAttribCount(void)
{
    int n_supported_gl_attributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n_supported_gl_attributes);
    std::cout << "OpenGL supports: " << n_supported_gl_attributes << " attributes on this machine" << std::endl;
}