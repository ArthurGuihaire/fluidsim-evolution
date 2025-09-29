#pragma once
#include <gpuBuffer.hpp>
#include <constants.hpp>
#include <inputMethods.hpp>

class Renderer {
    public:
        Renderer();
        void renderFrame(userPointer* mouseInfo);
    private:
        uint32_t fullQuadVAO;
        uint32_t smallSquareVAO;
        gpuBuffer fullQuadVBO;
        gpuBuffer smallSquareVBO;
        gpuBuffer indexBuffer;

        uint32_t framebuffers[2];
        uint32_t textures[2];

        uint32_t shaders[numShaders];
        uint32_t textureUniformLocations[numShaders];
        uint32_t texelSizeUniformLocations[numShaders];
        uint32_t drawSquareShader;
        uint32_t colorUniformLocation;
        uint32_t mousePosUniformLocation;

        uint32_t readIndex = 0;
        uint32_t writeIndex = 1;
};
