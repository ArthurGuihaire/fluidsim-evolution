#pragma once

struct userPointer {
    glm::vec2 mousePos;
    glm::vec2 relativeMovement;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
userPointer* getMouseInfo(GLFWwindow* window);
