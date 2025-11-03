#pragma once

constexpr uint32_t gpuBufferMultiple = 1024;

constexpr uint32_t numJacobiIterations = 25;
constexpr float scalingFactor = 1.0f;

extern uint32_t windowWidth;
extern uint32_t windowHeight;

extern uint32_t textureWidth;
extern uint32_t textureHeight;

extern glm::vec2 texelSize;
extern glm::vec2 windowTexelSize;

constexpr inline float quadVertices[16] = {
    // Screen space coords    texture coords
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};
