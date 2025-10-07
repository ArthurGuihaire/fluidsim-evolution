#include <utils.hpp>
#include <initializer.hpp>
#include <GLFW/glfw3.h>
#include <inputMethods.hpp>
#include <renderer.hpp>

int main() {
    GLFWwindow* window = initOpenGL();

    userPointer mousePosInfo = {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)};
    glfwSetWindowUserPointer(window, &mousePosInfo);
    Renderer renderer;

    while (!(glfwWindowShouldClose(window)) /*iteration < 200*/) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.renderFrame(getMouseInfo(window));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
