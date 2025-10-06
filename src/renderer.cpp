#include <renderer.hpp>
#include <utils.hpp>
#include <shaderLoader.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr uint32_t numJacobiIterations = 20;

Renderer::Renderer()
 : velIndex(0), divPresIndex(0), colorIndex(0), timeSeconds(glfwGetTime()), drawColor(255.0f, 0.0f, 0.0f), colorState(0)
{
    //Setup for full screen quad
    glGenVertexArrays(1, &fullQuadVAO);
    glBindVertexArray(fullQuadVAO);

    fullQuadVBO.createBuffer(GL_ARRAY_BUFFER, &quadVertices[0], sizeof(quadVertices));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Setup for user input
    glGenVertexArrays(1, &smallSquareVAO);
    glBindVertexArray(smallSquareVAO);

    smallSquareVBO.createBuffer(GL_ARRAY_BUFFER, &squareVertices[0], sizeof(squareVertices));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Rebind full quad vertex array afterwards
    glBindVertexArray(fullQuadVAO);

    //Setup shaders
    advectionShader = createShader("texture.vert", "advection.frag");
    dtAdvectionUniform = glGetUniformLocation(advectionShader, "dt");
    textureAdvectionUniform = glGetUniformLocation(advectionShader, "velocityTexture");
    texelSizeAdvectionUniform = glGetUniformLocation(advectionShader, "texelSize");

    divergenceShader = createShader("texture.vert", "divergence.frag");
    textureDivergenceUniform = glGetUniformLocation(divergenceShader, "velocityTexture");
    texelSizeDivergenceUniform = glGetUniformLocation(divergenceShader, "texelSize");

    pressureShader = createShader("texture.vert", "pressure.frag");
    texturePressureUniform = glGetUniformLocation(pressureShader, "divPresTexture");
    texelSizePressureUniform = glGetUniformLocation(pressureShader, "texelSize");

    gradientShader = createShader("texture.vert", "gradient.frag");
    divPresTextureGradientUniform = glGetUniformLocation(gradientShader, "divPresTexture");
    velTextureGradientUniform = glGetUniformLocation(gradientShader, "velocityTexture");
    texelSizeGradientUniform = glGetUniformLocation(gradientShader, "texelSize");

    advectColorShader = createShader("texture.vert", "advectColor.frag");
    velTextureAdvectColorUniform = glGetUniformLocation(advectColorShader, "velocityTexture");
    colorTextureAdvectColorUniform = glGetUniformLocation(advectColorShader, "colorTexture");
    texelSizeAdvectColorUniform = glGetUniformLocation(advectColorShader, "texelSize");
    dtAdvectColorUniform = glGetUniformLocation(advectColorShader, "dt");

    pushFluidShader = createShader("userInput.vert", "pushFluid.frag");
    offsetPushFluidUniform = glGetUniformLocation(pushFluidShader, "offset");
    texturePushFluidUniform = glGetUniformLocation(pushFluidShader, "velocityTexture");
    forcePushFluidUniform = glGetUniformLocation(pushFluidShader, "pushForce");
    dtPushFluidUniform = glGetUniformLocation(pushFluidShader, "dt");

    drawColorShader = createShader("draw.vert", "drawColor.frag");
    offsetDrawColorUniform = glGetUniformLocation(drawColorShader, "offset");
    colorDrawColorUniform = glGetUniformLocation(drawColorShader, "inputColor");

    visualizeShader = createShader("texture.vert", "visualize.frag");
    textureVisualizeUniform = glGetUniformLocation(visualizeShader, "colorTexture");

    //Setup for textures and framebuffers
    std::pair firstBufferPair = createFrameBufferTexture(GL_RG16F);
    std::pair secondBufferPair = createFrameBufferTexture(GL_RG16F);
    std::cout << secondBufferPair.second << std::endl;
    std::cout << firstBufferPair.second << std::endl;
    velocity[0].emplace(FramebufferTexture(firstBufferPair.first, secondBufferPair.second));
    velocity[1].emplace(FramebufferTexture(secondBufferPair.first, firstBufferPair.second));
    
    firstBufferPair = createFrameBufferTexture(GL_RG16F);
    secondBufferPair = createFrameBufferTexture(GL_RG16F);
    divergencePressure[0].emplace(FramebufferTexture(firstBufferPair.first, secondBufferPair.second));
    divergencePressure[1].emplace(FramebufferTexture(secondBufferPair.first, firstBufferPair.second));

    firstBufferPair = createFrameBufferTexture(GL_RGB8);
    secondBufferPair = createFrameBufferTexture(GL_RGB8);
    color[0].emplace(FramebufferTexture(firstBufferPair.first, secondBufferPair.second));
    color[1].emplace(FramebufferTexture(secondBufferPair.first, firstBufferPair.second));

    printOpenGLErrors("Init Error");
}

void Renderer::renderFrame(userPointer* mouseInfo) {
    float newTime = glfwGetTime();
    float deltaTime = (newTime - timeSeconds);
    timeSeconds = newTime;

    //std::cout << "dt: " << deltaTime << "\nMouse position: " << mouseInfo->mousePos.x << ", " << mouseInfo->mousePos.y << std::endl;

    //Step 1: Advection pass
    //Full quad is already bound
    velocity[velIndex]->setupPass(GL_TEXTURE0);
    glUseProgram(advectionShader);
    glUniform1i(textureAdvectionUniform, 0);
    glUniform2fv(texelSizeAdvectionUniform, 1, glm::value_ptr(texelSize));
    glUniform1f(dtAdvectionUniform, deltaTime);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    velIndex = 1 - velIndex; //swap between 0 and 1

    //Step 2: User input
    velocity[velIndex]->setupPass(GL_TEXTURE0);
    glBindVertexArray(smallSquareVAO);
    glUseProgram(pushFluidShader); //First write to velocity field
    glUniform2fv(offsetPushFluidUniform, 1, glm::value_ptr(mouseInfo->mousePos));
    glUniform1i(texturePushFluidUniform, 0);
    glUniform1f(dtPushFluidUniform, deltaTime);
    glUniform2fv(forcePushFluidUniform, 1, glm::value_ptr(mouseInfo->relativeMovement));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    velIndex = 1 - velIndex;

    //User input drawColor Step 2B
    color[colorIndex]->setupFramebuffer();
    glUseProgram(drawColorShader);
    glUniform2fv(offsetDrawColorUniform, 1, glm::value_ptr(mouseInfo->mousePos));
    glUniform3fv(colorDrawColorUniform, 1, glm::value_ptr(drawColor / 255.0f));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    colorIndex = 1 - colorIndex; //Otherwise it gets overwritten by advection

    /*velocity[velIndex]->setupTexture();
    glBindVertexArray(fullQuadVAO);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(visualizeShader);
    glUniform1i(textureVisualizeUniform, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/

    //Step 3: compute divergence
    velocity[velIndex]->setupTexture(GL_TEXTURE0);
    divergencePressure[divPresIndex]->setupFramebuffer();
    glBindVertexArray(fullQuadVAO);
    glUseProgram(divergenceShader);
    glUniform1i(textureDivergenceUniform, 0);
    glUniform2fv(texelSizeDivergenceUniform, 1, glm::value_ptr(texelSize));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    divPresIndex = 1 - divPresIndex;

    //Visualize
    divergencePressure[divPresIndex]->setupTexture(GL_TEXTURE0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glUseProgram(visualizeShader);
    glUniform1i(textureVisualizeUniform, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    /*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);*/

    //Step 4: 25 Jacobi Iterations to estimate pressure
    glUseProgram(pressureShader); //Shader and uniforms are the same in all iterations
    glUniform1i(texturePressureUniform, 0);
    glUniform2fv(texelSizePressureUniform, 1, glm::value_ptr(texelSize));
    for (uint32_t i = 0; i < numJacobiIterations; i++) {
        divergencePressure[divPresIndex]->setupPass();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        divPresIndex = 1 - divPresIndex;
    }

    //Step 5: Use the computed pressure (gradient) to update velocity
    divergencePressure[divPresIndex]->setupTexture(GL_TEXTURE0); //Only reading from pressure
    velocity[velIndex]->setupPass(GL_TEXTURE1); //We are reading and writing to velocity
    glUseProgram(gradientShader);
    glUniform1i(divPresTextureGradientUniform, 0);
    glUniform1i(velTextureGradientUniform, 1);
    glUniform2fv(texelSizeGradientUniform, 1, glm::value_ptr(texelSize));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    velIndex = 1 - velIndex; //Since we're writing to velocity, must switch the index

    //Step 6: advect color
    color[colorIndex]->setupPass(GL_TEXTURE0); //Read/write to color
    velocity[velIndex]->setupTexture(GL_TEXTURE1); //Read from velocity
    glUseProgram(advectColorShader);
    glUniform1i(colorTextureAdvectColorUniform, 0);
    glUniform1i(velTextureAdvectColorUniform, 1);
    glUniform2fv(texelSizeAdvectColorUniform, 1, glm::value_ptr(texelSize));
    glUniform1f(dtAdvectColorUniform, deltaTime);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Bind the framebuffer that holds the latest velocity output (opposite of current index)
    /*velocity[1 - velIndex]->bindReadFramebuffer();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);*/

    //the last color framebuffer is still bound to GL_READ_FRAMEBUFFER
    /*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);*/

    getNextColor(drawColor, colorState);
}
