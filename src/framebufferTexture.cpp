#include <framebufferTexture.hpp>
#include <constants.hpp>
#include <glm/gtc/type_ptr.hpp>

FramebufferTexture::FramebufferTexture()
 : writeFramebuffer(0), readTexture(0) //Prevent random textures and framebuffers from getting deleted
{} // This constructor does not initialize anything

FramebufferTexture::FramebufferTexture(const uint32_t framebuffer, const uint32_t texture)
 : writeFramebuffer(framebuffer), readTexture(texture)
{
}

FramebufferTexture::~FramebufferTexture() {
    glDeleteFramebuffers(1, &writeFramebuffer);
    glDeleteTextures(1, &readTexture);
}

FramebufferTexture::FramebufferTexture(FramebufferTexture&& other) noexcept {
    writeFramebuffer = other.writeFramebuffer;
    readTexture = other.readTexture;
    other.writeFramebuffer = 0;
    other.readTexture = 0;
}

FramebufferTexture& FramebufferTexture::operator=(FramebufferTexture&& other) noexcept {
    if (this != &other) {
        glDeleteFramebuffers(1, &writeFramebuffer);
        glDeleteTextures(1, &readTexture);
        writeFramebuffer = other.writeFramebuffer;
        readTexture = other.readTexture;
        other.writeFramebuffer = 0;
        other.readTexture = 0;
    }
    return *this;
}

void FramebufferTexture::setupTexture() {
    glBindTexture(GL_TEXTURE_2D, readTexture);
}

void FramebufferTexture::setupTexture(const uint32_t textureBinding) {
    glActiveTexture(textureBinding);
    glBindTexture(GL_TEXTURE_2D, readTexture);
}

void FramebufferTexture::setupFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, writeFramebuffer);
}

void FramebufferTexture::bindReadFramebuffer() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, writeFramebuffer);
}

void FramebufferTexture::setupPass() {
    setupFramebuffer();
    setupTexture();
}

void FramebufferTexture::setupPass(const uint32_t textureBinding) {
    setupFramebuffer();
    setupTexture(textureBinding);
}

void FramebufferTexture::resizeTextures(const uint32_t newFramebuffer, const uint32_t newTexture, const bool blit, const std::optional<uint32_t> newWidth, const std::optional<uint32_t> newHeight) {
    if (blit) {
        //Blit window size rectangle onto new window size
        //Crop instead of letting OpenGL scale
        const uint32_t width = std::min(*newWidth, windowWidth);
        const uint32_t height = std::min(*newHeight, windowHeight);
        //In case it isn't bound yet
        glBindFramebuffer(GL_READ_FRAMEBUFFER, writeFramebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, newFramebuffer);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    //If framebuffers and textures are still bound, OpenGL won't delete them
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteFramebuffers(1, &writeFramebuffer);
    glDeleteTextures(1, &readTexture);

    writeFramebuffer = newFramebuffer;
    readTexture = newTexture;
}
