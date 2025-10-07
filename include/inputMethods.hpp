#pragma once

//Forward declare renderer, otherwise causes issues
class Renderer;

struct userPointer {
    glm::vec2 mousePos;
    glm::vec2 relativeMovement;
    Renderer& rendererRef;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
userPointer* getMouseInfo(GLFWwindow* window);
