#pragma once

constexpr uint32_t gpuBufferMultiple = 1;
constexpr uint32_t numShaders = 3;
\
constexpr float scalingFactor = 0.5f;

constexpr float drawSize = 100.0f * scalingFactor;

extern uint32_t windowWidth;
extern uint32_t windowHeight;

extern uint32_t textureWidth;
extern uint32_t textureHeight;

extern glm::vec2 texelSize;

constexpr inline float quadVertices[16] = {
    // Screen space coords    texture coords
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};

extern float squareVertices[16];
