#include <utils.hpp>
#include <initializer.hpp>
#include <GLFW/glfw3.h>
#include <inputMethods.hpp>
#include <renderer.hpp>

int main() {
    GLFWwindow* window = initOpenGL();

    Renderer renderer;

    uint32_t iteration = 0;
    while (/*!(glfwWindowShouldClose(window))*/ iteration < 200) {
        processInput(window);

        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        renderer.renderFrame();
        printOpenGLErrors("OpenGL error");

        glfwSwapBuffers(window);
        glfwPollEvents();
        iteration++;
    }
}