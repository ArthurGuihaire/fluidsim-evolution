#include <renderer.hpp>
#include <utils.hpp>
#include <shaderLoader.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() {
    //Setup for full screen quad
    glGenVertexArrays(1, &fullQuadVAO);
    glBindVertexArray(fullQuadVAO);

    fullQuadVBO.createBuffer(GL_ARRAY_BUFFER, &quadVertices[0], sizeof(quadVertices));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    indexBuffer.createBuffer(GL_ELEMENT_ARRAY_BUFFER, &quadIndices, sizeof(quadIndices));

    //Setup for small square (UI element)
    glGenVertexArrays(1, &smallSquareVAO);
    glBindVertexArray(smallSquareVAO);

    smallSquareVBO.createBuffer(GL_ARRAY_BUFFER, &squareVertices[0], sizeof(squareVertices));
    indexBuffer.bind();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    //Setup for framebuffers and textures
    for (uint32_t i = 0; i < 2; i++) {
        std::pair<uint32_t, uint32_t> framebufferTexture = createFrameBufferTexture();
        framebuffers[i] = framebufferTexture.first;
        textures[i] = framebufferTexture.second;
    }

    //Initialize shaders
    for (uint32_t i = 0; i < numShaders; i++) {
        ShaderProgramSource shaderSource = parseShader(shaderNames[i]);
        shaders[i] = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
        textureUniformLocations[i] = glGetUniformLocation(shaders[i], "inputTexture");
        texelSizeUniformLocations[i] = glGetUniformLocation(shaders[i], "texelSize");
    }

    ShaderProgramSource shaderSource = parseShader("shaders/drawSquare.shader");
    drawSquareShader = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    mousePosUniformLocation = glGetUniformLocation(drawSquareShader, "mousePos");
    colorUniformLocation = glGetUniformLocation(drawSquareShader, "inputColor");
}

void Renderer::renderFrame(userPointer* mouseInfo) {
    glBindVertexArray(fullQuadVAO);
    for (float texelSizeMultiplier = 1; texelSizeMultiplier < 4; texelSizeMultiplier++) {
        for (uint32_t pass = 0; pass < numShaders; pass++) {
            glUseProgram(shaders[pass]);
            glBindTexture(GL_TEXTURE_2D, textures[readIndex]);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[writeIndex]);
            glUniform1i(textureUniformLocations[pass], 0);
            glUniform2fv(texelSizeUniformLocations[pass], 1, glm::value_ptr(texelSize * texelSizeMultiplier * texelSizeMultiplier));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

            std::swap(readIndex, writeIndex);
        }
    }

    
    glBindVertexArray(smallSquareVAO);
    glUseProgram(drawSquareShader);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[writeIndex]);
    //glUniform2fv(mousePosUniformLocation, 1, glm::value_ptr(mouseInfo->mousePos));
    glUniform2fv(mousePosUniformLocation, 1, glm::value_ptr(mouseInfo->mousePos));
    float magnitude = mouseInfo->relativeMovement.x * mouseInfo->relativeMovement.x + mouseInfo->relativeMovement.y * mouseInfo->relativeMovement.y;
    glUniform3f(colorUniformLocation, (mouseInfo->relativeMovement.x + 1) / 2, (mouseInfo->relativeMovement.y + 1) / 2, magnitude);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

    std::swap(readIndex, writeIndex);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}