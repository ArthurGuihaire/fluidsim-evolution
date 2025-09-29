#include <renderer.hpp>
#include <utils.hpp>
#include <shaderLoader.hpp>

Renderer::Renderer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vertexBuffer.createBuffer(GL_ARRAY_BUFFER, &quadVertices[0], sizeof(quadVertices));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 2);
    glEnableVertexAttribArray(1);
    
    indexBuffer.createBuffer(GL_ELEMENT_ARRAY_BUFFER, &quadIndices, sizeof(quadIndices));

    for (uint32_t i = 0; i < 2; i++) {
        std::pair<uint32_t, uint32_t> framebufferTexture = createFrameBufferTexture();
        framebuffers[i] = framebufferTexture.first;
        textures[i] = framebufferTexture.second;
        std::cout << "framebuffer: " << framebuffers[i] << "\ntexture: " << textures[i] << std::endl;
    }

    ShaderProgramSource shaderSource = parseShader("shaders/default.shader");
    shaders[0] = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shaders[0]);

    textureUniformLocations[0] = glGetUniformLocation(shaders[0], "inputTexture");
}

void Renderer::renderFrame() {
    glBindTexture(GL_TEXTURE_2D, textures[readIndex]);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[writeIndex]);
    glUniform1i(textureUniformLocations[0], 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

    std::swap(readIndex, writeIndex);

    glBindTexture(GL_TEXTURE_2D, textures[readIndex]);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[writeIndex]);
    glUniform1i(textureUniformLocations[0], 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);
    
    std::swap(readIndex, writeIndex);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowWidth, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}