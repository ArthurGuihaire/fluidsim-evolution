#include <renderer.hpp>
#include <utils.hpp>
#include <shaderLoader.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vertexBuffer.createBuffer(GL_ARRAY_BUFFER, &quadVertices[0], sizeof(quadVertices));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    indexBuffer.createBuffer(GL_ELEMENT_ARRAY_BUFFER, &quadIndices, sizeof(quadIndices));

    for (uint32_t i = 0; i < 2; i++) {
        std::pair<uint32_t, uint32_t> framebufferTexture = createFrameBufferTexture();
        framebuffers[i] = framebufferTexture.first;
        textures[i] = framebufferTexture.second;
        std::cout << "framebuffer: " << framebuffers[i] << "\ntexture: " << textures[i] << std::endl;
    }

    for (uint32_t i = 0; i < numShaders; i++) {
        ShaderProgramSource shaderSource = parseShader(shaderNames[i]);
        shaders[i] = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
        textureUniformLocations[i] = glGetUniformLocation(shaders[i], "inputTexture");
        texelSizeUniformLocations[i] = glGetUniformLocation(shaders[i], "texelSize");
    }

    ShaderProgramSource shaderSource = parseShader("shaders/drawSquare.shader");
    drawSquareShader = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    colorUniformLocation = glGetUniformLocation(drawSquareShader, "inputColor");
}

void Renderer::renderFrame() {
    for (uint32_t pass = 0; pass < numShaders; pass++) {
        glUseProgram(shaders[pass]);
        glBindTexture(GL_TEXTURE_2D, textures[readIndex]);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[writeIndex]);
        glUniform1i(textureUniformLocations[pass], 0);
        glUniform2fv(texelSizeUniformLocations[pass], 1, glm::value_ptr(texelSize));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

        std::swap(readIndex, writeIndex);
    }

    glUseProgram(drawSquareShader);
    glBindTexture(GL_TEXTURE_2D, textures[readIndex]);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[writeIndex]);
    glUniform3f(colorUniformLocation, 1.0f, 1.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

    std::swap(readIndex, writeIndex);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowWidth, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}