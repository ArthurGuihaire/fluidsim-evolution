#include <initializer.hpp>
#include <iostream>
#include <gl.h>
#include <constants.hpp>
#include <utils.hpp>
#include <cstring> //For std::memcpy

//Compiler wants a definition in a cpp file for some reason
uint32_t windowWidth;
uint32_t windowHeight;
glm::vec2 texelSize;
float squareVertices[16];

Initializer::Initializer() {}

void Initializer::setWindowSize(int startWidth, int startHeight) {
    width = startWidth;
    height = startHeight;
}

void Initializer::initGLFW(int majorVersion, int minorVersion) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* Initializer::createWindow(bool fullscreen) {
    GLFWwindow* window;
    if (fullscreen) {
        GLFWmonitor* mon = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(mon);
        window = glfwCreateWindow(vidMode->width, vidMode->height, "OpenGL", mon, NULL);
        windowWidth = vidMode->width;
        windowHeight = vidMode->height;

        texelSize.x = 1.0f / windowWidth;
        texelSize.y = 1.0f / windowHeight;

        //We can't reinitialize square vertices, but we can memcpy
        float verticesToCopy[16] = {
            -drawSize / windowWidth, -drawSize / windowHeight, 0.0f, 0.0f,
            -drawSize / windowWidth, drawSize / windowHeight, 0.0f, drawSize / windowHeight, 
            drawSize / windowWidth, -drawSize / windowHeight, drawSize / windowWidth, 0.0f, 
            drawSize / windowWidth, drawSize / windowHeight, drawSize / windowWidth, drawSize / windowHeight
        };
        std::memcpy(&squareVertices[0], &verticesToCopy[0], 16 * sizeof(float));
    }
    else {
        window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    }
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    //Create an OpenGL contex for the window
    glfwSwapInterval(1); //Limit framerate
    glfwMakeContextCurrent(window);
    return window;
}

void Initializer::initGLAD() {
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, windowWidth, windowHeight);
}
