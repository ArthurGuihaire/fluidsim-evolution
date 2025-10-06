#pragma once
#include <gpuBuffer.hpp>
#include <constants.hpp>
#include <inputMethods.hpp>
#include <framebufferTexture.hpp>

class Renderer {
    public:
        Renderer();
        void renderFrame(userPointer* mouseInfo);
    private:
        //Vertex stuff
        uint32_t fullQuadVAO;
        uint32_t smallSquareVAO;
        gpuBuffer fullQuadVBO;
        gpuBuffer smallSquareVBO;

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
        uint32_t offsetPushFluidUniform;
        uint32_t texturePushFluidUniform;
        uint32_t forcePushFluidUniform;
        uint32_t dtPushFluidUniform;

        uint32_t drawColorShader;
        uint32_t offsetDrawColorUniform;
        uint32_t colorDrawColorUniform;

        uint32_t visualizeShader;
        uint32_t textureVisualizeUniform;

        //Framebuffers
        FramebufferTexture velocity[2];
        uint32_t velIndex;

        FramebufferTexture divergencePressure[2];
        uint32_t divPresIndex; //Divergence and pressure

        FramebufferTexture color[2];
        uint32_t colorIndex;

        //Other random stuff
        float timeSeconds;
        glm::vec3 drawColor;
        uint32_t colorState;
};
