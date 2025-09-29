#pragma once

constexpr uint32_t gpuBufferMultiple = 4096;
constexpr uint32_t numShaders = 1;

extern uint32_t windowWidth;
extern uint32_t windowHeight;

constexpr inline float quadVertices[16] = {
    // Screen space coords    texture coords
    -1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f
};

constexpr inline uint32_t quadIndices[] = {
    0, 1, 2,
    2, 3, 0
};
