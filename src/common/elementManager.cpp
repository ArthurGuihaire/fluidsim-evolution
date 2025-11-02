#include <openglPCH.hpp>
#include <elementManager.hpp>
#include <gpuBuffer.hpp>
#include <shaderLoader.hpp>

constexpr inline uint32_t quadIndices[6] = {
    0, 1, 2, 2, 3, 0
};

ElementManager::ElementManager()
 : vertexBuffer(GL_ARRAY_BUFFER), indexBuffer(GL_ELEMENT_ARRAY_BUFFER)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    vertexBuffer.bind();
    indexBuffer.bind();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    roundedCornerShader = createShader("UI/rect.vert", "UI/rounded.frag");
}

void ElementManager::addElement(void (*onClickFunction)(), float boundingBox[4]) {
    elements.emplace_back(onClickFunction, boundingBox[0], boundingBox[2], boundingBox[1], boundingBox[3]);

    //constuct the quad
    vertices.reserveOrDouble(vertices.size() + 8);
    vertices.addMultiple(boundingBox, 2); //top left
    vertices.add(boundingBox[0]); //bottom left
    vertices.add(boundingBox[3]);
    vertices.addMultiple(&boundingBox[2], 2); // bottom right
    vertices.add(boundingBox[2]); // top right
    vertices.add(boundingBox[1]);

    indices.reserveOrDouble(indices.size() + 6);
    const uint32_t indexOffset = vertices.size() / 2;
    for (uint32_t i = 0; i < 6; i++) {
        indices.add(quadIndices[i] + indexOffset);
    }

    //gpu buffer upload
    vertexBuffer.addData(&vertices[vertices.size() - 8], 8 * sizeof(float));
    indexBuffer.addData(&indices[indices.size() - 6], 6 * sizeof(uint32_t));
}

void ElementManager::renderUI() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_FLOAT, nullptr);
}
