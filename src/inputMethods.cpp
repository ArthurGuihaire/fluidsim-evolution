#include <inputMethods.hpp>
#include <utils.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    textureWidth = width * scalingFactor;
    textureHeight = height * scalingFactor;
    texelSize.x = 1.0f / textureWidth;
    texelSize.y = 1.0f / textureHeight;
    glViewport(0, 0, textureWidth, textureHeight);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    userPointer* mouseInfo = static_cast<userPointer*>(glfwGetWindowUserPointer(window));
    glm::vec2 pos = {xpos, ypos};
    mouseInfo->relativeMovement = mouseInfo->mousePos - screenToWorld(pos);
    mouseInfo->mousePos = screenToWorld(pos);
}

userPointer* getMouseInfo(GLFWwindow* window) {
    userPointer* ptr = static_cast<userPointer*>(glfwGetWindowUserPointer(window));
    return ptr;
}
