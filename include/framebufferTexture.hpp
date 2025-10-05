#include <optional>
class FramebufferTexture {
    public:
        FramebufferTexture(); // useless constructor
        FramebufferTexture(const uint32_t framebuffer, const uint32_t texture);
        ~FramebufferTexture();
        void setupFramebuffer();
        void setupTexture();
        void setupTexture(const uint32_t textureBinding);
        void setupPass();
        void setupPass(const uint32_t textureBinding);
        void resizeTextures(const uint32_t newFramebuffer, const uint32_t newTexture, const bool blit, const std::optional<uint32_t> newWidth, const std::optional<uint32_t> newHeight);

    private:
        uint32_t writeFramebuffer;
        uint32_t readTexture;
};
