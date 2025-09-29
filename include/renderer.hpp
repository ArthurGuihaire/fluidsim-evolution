#pragma once
#include <gpuBuffer.hpp>
#include <constants.hpp>

class Renderer {
    public:
        Renderer();
        void renderFrame();
    private:
        uint32_t vao;
        gpuBuffer vertexBuffer;
        gpuBuffer indexBuffer;

        uint32_t framebuffers[2];
        uint32_t textures[2];

        uint32_t shaders[numShaders];
        uint32_t textureUniformLocations[numShaders];
        uint32_t texelSizeUniformLocations[numShaders];
        uint32_t drawSquareShader;
        uint32_t colorUniformLocation;

        uint32_t readIndex = 0;
        uint32_t writeIndex = 1;
};
