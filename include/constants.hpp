#pragma once

constexpr uint32_t gpuBufferMultiple = 1;
constexpr uint32_t numShaders = 3;

constexpr float drawSize = 500.0f;

extern uint32_t windowWidth;
extern uint32_t windowHeight;

extern glm::vec2 texelSize;

constexpr inline float quadVertices[16] = {
    // Screen space coords    texture coords
    -1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};

extern float squareVertices[16];
