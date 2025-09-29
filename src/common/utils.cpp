#include <utils.hpp>
#include <initializer.hpp>
#include <gl.h>
#include <inputMethods.hpp>

GLFWwindow* initOpenGL() {
    Initializer init;
    init.initGLFW(3, 3);
    GLFWwindow* window = init.createWindow(true);
    init.initGLAD();

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

std::pair<uint32_t, uint32_t> createFrameBufferTexture() {
    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    uint32_t tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = {0.0f, 0.0f, 0.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer " << fbo << " complete" << std::endl;

    return std::make_pair(fbo, tex);
}

void printOpenGLErrors(const char* printString) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("%s: 0x%x\n", printString, err);
    }
}

glm::vec2 screenToWorld(glm::vec2& screen) {
    return {screen.x * 2 / windowWidth - 1, 1 - 2 * screen.y / windowHeight};
}
