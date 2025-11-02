#pragma once

constexpr uint32_t gpuBufferMultiple = 1;

constexpr uint32_t numJacobiIterations = 35;
constexpr float scalingFactor = 0.5f;

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
