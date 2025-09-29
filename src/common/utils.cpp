#include <utils.hpp>
#include <initializer.hpp>
#include <gl.h>

GLFWwindow* initOpenGL() {
    Initializer init;
    init.initGLFW(3, 3);
    GLFWwindow* window = init.createWindow(true);
    init.initGLAD();
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

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer complete" << std::endl;

    return std::make_pair(fbo, tex);
}

void printOpenGLErrors(const char* printString) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("%s: 0x%x\n", printString, err);
    }
}
