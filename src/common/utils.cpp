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

constexpr uint32_t getTextureType(const uint32_t internalFormat) {
     switch (internalFormat) {
        case GL_R8:
        case GL_RG8:
        case GL_RGB8:
        case GL_RGBA8:
            return GL_UNSIGNED_BYTE;
        case GL_R16F:
        case GL_RG16F:
        case GL_RGB16F:
        case GL_RGBA16F:
            return GL_HALF_FLOAT;
        case GL_R32F:
        case GL_RG32F:
        case GL_RGB32F:
        case GL_RGBA32F:
            return GL_FLOAT;
        default:
            std::cerr << "Unsupported internal format: 0x" << std::hex << internalFormat << std::dec << std::endl;
            return GL_UNSIGNED_BYTE;
    }
}

constexpr uint32_t getCorrespondingFormat(const uint32_t internalFormat) {
    switch (internalFormat) {
        case GL_R8:
        case GL_R8UI:
        case GL_R16F:
        case GL_R32F:
            return GL_RED;
        case GL_RG8:
        case GL_RG16F:
        case GL_RG32F:
            return GL_RG;
        case GL_RGB8:
        case GL_RGB16F:
        case GL_RGB32F:
            return GL_RGB;
        case GL_RGBA8:
        case GL_RGBA16F:
        case GL_RGBA32F:
            return GL_RGBA;
        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32F:
            return GL_DEPTH_COMPONENT;
        case GL_DEPTH24_STENCIL8:
        case GL_DEPTH32F_STENCIL8:
            return GL_DEPTH_STENCIL;
        default:
            std::cerr << "Unsupported internal format: 0x" << std::hex << internalFormat << std::dec << std::endl;
            return GL_RGBA;
    }
}

std::pair<uint32_t, uint32_t> createFrameBufferTexture(const uint32_t textureFormat, const uint32_t width, const uint32_t height) {
    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    uint32_t tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    const uint32_t format = getCorrespondingFormat(textureFormat);
    const uint32_t bufferType = getTextureType(textureFormat);
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, format, bufferType, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer " << fbo << " complete" << std::endl;
    else {
        std::cerr << "Framebuffer failed with status: 0x" << std::hex << status << std::dec << std::endl;
    }

    return std::make_pair(fbo, tex);
}

std::pair<uint32_t, uint32_t> createFrameBufferTexture(const uint32_t textureFormat) {
    return createFrameBufferTexture(textureFormat, windowWidth, windowHeight);
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

void getNextColor(glm::vec3& color, uint32_t& colorState) {
    switch(colorState) {
        case 0:
            color.g++;
            if (color.g >= 255)
                colorState = 1;
            break;
        case 1:
            color.r--;
            if (color.r <= 0)
                colorState = 2;
            break;
        case 2:
            color.b++;
            if (color.b >= 255)
                colorState = 3;
            break;
        case 3:
            color.g--;
            if (color.g <= 0)
                colorState = 4;
            break;
        case 4:
            color.r++;
            if (color.r >= 255)
                colorState = 5;
            break;
        case 5:
            color.b--;
            if (color.b <= 0)
                colorState = 0;
            break;
    }
}
