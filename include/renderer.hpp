#pragma once
#include <gpuBuffer.hpp>
#include <constants.hpp>
#include <inputMethods.hpp>
#include <framebufferTexture.hpp>
#include <elementManager.hpp>

class Renderer {
    public:
        Renderer();
        void renderFrame(userPointer* mouseInfo);
    private:
        //Vertex stuff
        uint32_t fullQuadVAO;
        gpuBuffer fullQuadVBO;

        //Shaders - texelSize and Texture uniforms are stored in framebufferTexture objects
        uint32_t advectionShader;
        uint32_t textureAdvectionUniform;
        uint32_t texelSizeAdvectionUniform;
        uint32_t dtAdvectionUniform;

        uint32_t divergenceShader;
        uint32_t textureDivergenceUniform;
        uint32_t texelSizeDivergenceUniform;

        uint32_t pressureShader;
        uint32_t texturePressureUniform;
        uint32_t texelSizePressureUniform;

        uint32_t gradientShader;
        uint32_t divPresTextureGradientUniform;
        uint32_t velTextureGradientUniform;
        uint32_t texelSizeGradientUniform;

        uint32_t advectColorShader;
        uint32_t velTextureAdvectColorUniform;
        uint32_t colorTextureAdvectColorUniform;
        uint32_t texelSizeAdvectColorUniform;
        uint32_t dtAdvectColorUniform;

        uint32_t pushFluidShader;
        uint32_t texturePushFluidUniform;
        uint32_t forcePushFluidUniform;
        uint32_t radiusPushFluidUniform;
        uint32_t centerPushFluidUniform;
        uint32_t texelSizePushFluidUniform;

        uint32_t drawColorShader;
        uint32_t inputDrawColorUniform;
        uint32_t radiusDrawColorUniform;
        uint32_t centerDrawColorUniform;
        uint32_t texelSizeDrawColorUniform;

        //Framebuffers
        std::optional<FramebufferTexture> velocity[2];
        uint32_t velIndex;

        std::optional<FramebufferTexture> divergencePressure[2];
        uint32_t divPresIndex; //Divergence and pressure

        std::optional<FramebufferTexture> color[2];
        uint32_t colorIndex;

        //UI stuff
        ElementManager elementManager;

        //Other random stuff
        float timeSeconds;
        glm::vec3 drawColor;
        uint32_t colorState;
        glm::vec2 relativeMovement;
};
