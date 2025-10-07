#include <optional>
class FramebufferTexture {
    public:
        FramebufferTexture(); // useless constructor
        FramebufferTexture(const uint32_t framebuffer, const uint32_t texture);
        ~FramebufferTexture();
        //Prevent copy but allow assignment
        FramebufferTexture(const FramebufferTexture&) = delete;
        FramebufferTexture& operator=(const FramebufferTexture&) = delete;
        //Allow assignment
        FramebufferTexture(FramebufferTexture&& other) noexcept;
        FramebufferTexture& operator=(FramebufferTexture&& other) noexcept;
        void setupFramebuffer();
        void bindReadFramebuffer();
        void setupTexture(const uint32_t textureBinding);
        void setupPass(const uint32_t textureBinding);
        void resizeTextures(const uint32_t newFramebuffer, const uint32_t newTexture, const bool blit, const std::optional<uint32_t> newWidth, const std::optional<uint32_t> newHeight);

    private:
        uint32_t writeFramebuffer;
        uint32_t readTexture;
};
