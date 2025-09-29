#pragma once
#include <GLFW/glfw3.h>

template <typename integerType>
constexpr integerType roundUpInt(const integerType input, const uint32_t cutoff) {
    static_assert(std::is_integral<integerType>::value, "T must be an integer type!");
    const unsigned int higherValue = input + cutoff - 1;
    const unsigned int bitmask = ~(cutoff - 1);
    return (higherValue & bitmask);
}

void printOpenGLErrors(const char* printString);
GLFWwindow* initOpenGL();
std::pair<uint32_t, uint32_t> createFrameBufferTexture();
