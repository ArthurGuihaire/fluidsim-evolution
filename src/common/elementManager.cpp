#include <elementManager.hpp>
#include <openglPCH.hpp>
#include <gpuBuffer.hpp>
#include <shaderLoader.hpp>
#include <utils.hpp>

constexpr inline uint32_t quadIndices[6] = {
    0, 1, 2, 2, 3, 0
};

ElementManager::ElementManager()
 : elementDataBuffer(GL_ARRAY_BUFFER)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Bounding box
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(UIElementData), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(0, 1);
    //rounding radius
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(UIElementData), (void*) (4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    //color
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(UIElementData), (void*) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    roundedCornerShader = createShader("UI/rect.vert", "UI/rounded.frag");
}

void ElementManager::addElement(void (*onClickFunction)(), float boundingBox[4]) {
    elements.emplace_back(onClickFunction, boundingBox[0], boundingBox[2], boundingBox[1], boundingBox[3]);

    UIElementData newElementData {
        {boundingBox[0], boundingBox[1], boundingBox[2], boundingBox[3]}, 0.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)};

    elementData.add(newElementData);
    if (elementDataBuffer.getUsedMemorySize() + sizeof(UIElementData) > elementDataBuffer.getBufferSize()) {
        elementDataBuffer.uploadBufferReserve(&elementData[0], elementData.size() * sizeof(float));
    }
    else {
        elementDataBuffer.addData(&elementData[elementData.size() - 1], sizeof(UIElementData));
    }
}

void ElementManager::renderUI() {
    glBindVertexArray(vao);
    glUseProgram(roundedCornerShader);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, elements.size());
}
