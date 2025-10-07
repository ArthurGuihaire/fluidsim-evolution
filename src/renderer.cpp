#include <renderer.hpp>
#include <utils.hpp>
#include <shaderLoader.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr float pushRadiusSquared = 400.0f * scalingFactor * scalingFactor;

Renderer::Renderer()
 : velIndex(0), divPresIndex(0), colorIndex(0), timeSeconds(glfwGetTime()), drawColor(255.0f, 0.0f, 0.0f), colorState(0), relativeMovement(0.0f)
{
    //Setup for full screen quad
    glGenVertexArrays(1, &fullQuadVAO);
    glBindVertexArray(fullQuadVAO);

    fullQuadVBO.createBuffer(GL_ARRAY_BUFFER, &quadVertices[0], sizeof(quadVertices));

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

    pushFluidShader = createShader("texture.vert", "pushFluid.frag");
    texturePushFluidUniform = glGetUniformLocation(pushFluidShader, "velocityTexture");
    forcePushFluidUniform = glGetUniformLocation(pushFluidShader, "pushForce");
    centerPushFluidUniform = glGetUniformLocation(pushFluidShader, "pushCenter");
    radiusPushFluidUniform = glGetUniformLocation(pushFluidShader, "pushRadiusSquared");
    texelSizePushFluidUniform = glGetUniformLocation(pushFluidShader, "texelSize");

    drawColorShader = createShader("texture.vert", "drawColor.frag");
    inputDrawColorUniform = glGetUniformLocation(drawColorShader, "inputColor");
    radiusDrawColorUniform = glGetUniformLocation(drawColorShader, "drawRadiusSquared");
    centerDrawColorUniform = glGetUniformLocation(drawColorShader, "drawCenter");
    texelSizeDrawColorUniform = glGetUniformLocation(drawColorShader, "texelSize");

    //Setup for textures and framebuffers
    std::pair firstBufferPair = createFrameBufferTexture(GL_RG16F, textureWidth, textureHeight);
    std::pair secondBufferPair = createFrameBufferTexture(GL_RG16F, textureWidth, textureHeight);
    std::cout << secondBufferPair.second << std::endl;
    std::cout << firstBufferPair.second << std::endl;
    velocity[0].emplace(FramebufferTexture(firstBufferPair.first, secondBufferPair.second));
    velocity[1].emplace(FramebufferTexture(secondBufferPair.first, firstBufferPair.second));
    
    firstBufferPair = createFrameBufferTexture(GL_RG16F, textureWidth, textureHeight);
    secondBufferPair = createFrameBufferTexture(GL_RG16F, textureWidth, textureHeight);
    divergencePressure[0].emplace(FramebufferTexture(firstBufferPair.first, secondBufferPair.second));
    divergencePressure[1].emplace(FramebufferTexture(secondBufferPair.first, firstBufferPair.second));

    firstBufferPair = createFrameBufferTexture(GL_RGB16, textureWidth, textureHeight);
    secondBufferPair = createFrameBufferTexture(GL_RGB16, textureWidth, textureHeight);
    color[0].emplace(FramebufferTexture(firstBufferPair.first, secondBufferPair.second));
    color[1].emplace(FramebufferTexture(secondBufferPair.first, firstBufferPair.second));

    printOpenGLErrors("Init Error");
}

void Renderer::renderFrame(userPointer* mouseInfo) {
    float newTime = glfwGetTime();
    float deltaTime = (newTime - timeSeconds);
    timeSeconds = newTime;

    //Step 1: User input
    velocity[velIndex]->setupPass(GL_TEXTURE0);
    glBindVertexArray(fullQuadVAO);
    glUseProgram(pushFluidShader); //First write to velocity field
    glUniform1i(texturePushFluidUniform, 0);
    glUniform1f(radiusPushFluidUniform, pushRadiusSquared);
    glUniform2fv(centerPushFluidUniform, 1, glm::value_ptr(mouseInfo->mousePos));
    relativeMovement = (mouseInfo->relativeMovement * 99.0f + relativeMovement) / 100.0f;
    glUniform2fv(forcePushFluidUniform, 1, glm::value_ptr(mouseInfo->relativeMovement * -10000.0f));
    glUniform2fv(texelSizePushFluidUniform, 1, glm::value_ptr(texelSize));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    velIndex = 1 - velIndex;

    //User input drawColor Step 2B
    color[colorIndex]->setupFramebuffer();
    glUseProgram(drawColorShader);
    glUniform3fv(inputDrawColorUniform, 1, glm::value_ptr(drawColor));
    glUniform1f(radiusDrawColorUniform, pushRadiusSquared);
    glUniform2fv(centerDrawColorUniform, 1, glm::value_ptr(mouseInfo->mousePos));
    glUniform2fv(texelSizeDrawColorUniform, 1, glm::value_ptr(texelSize));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    colorIndex = 1 - colorIndex; //Otherwise it gets overwritten by advection

    //Step 2: compute divergence
    velocity[velIndex]->setupTexture(GL_TEXTURE0);
    divergencePressure[divPresIndex]->setupFramebuffer();
    glBindVertexArray(fullQuadVAO);
    glUseProgram(divergenceShader);
    glUniform1i(textureDivergenceUniform, 0);
    glUniform2fv(texelSizeDivergenceUniform, 1, glm::value_ptr(texelSize));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    divPresIndex = 1 - divPresIndex;

    //Step 3: 25 Jacobi Iterations to estimate pressure
    glUseProgram(pressureShader); //Shader and uniforms are the same in all iterations
    glUniform1i(texturePressureUniform, 0);
    glUniform2fv(texelSizePressureUniform, 1, glm::value_ptr(texelSize));
    for (uint32_t i = 0; i < numJacobiIterations; i++) {
        divergencePressure[divPresIndex]->setupPass(GL_TEXTURE0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        divPresIndex = 1 - divPresIndex;
    }

    //Step 4: Use the computed pressure (gradient) to update velocity
    divergencePressure[divPresIndex]->setupTexture(GL_TEXTURE0); //Only reading from pressure
    velocity[velIndex]->setupPass(GL_TEXTURE1); //We are reading and writing to velocity
    glUseProgram(gradientShader);
    glUniform1i(divPresTextureGradientUniform, 0);
    glUniform1i(velTextureGradientUniform, 1);
    glUniform2fv(texelSizeGradientUniform, 1, glm::value_ptr(texelSize));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    velIndex = 1 - velIndex; //Since we're writing to velocity, must switch the index

    //Step 5: Advection pass
    //Full quad is already bound
    velocity[velIndex]->setupPass(GL_TEXTURE0);
    glUseProgram(advectionShader);
    glUniform1i(textureAdvectionUniform, 0);
    glUniform2fv(texelSizeAdvectionUniform, 1, glm::value_ptr(windowTexelSize));
    glUniform1f(dtAdvectionUniform, 5.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    velIndex = 1 - velIndex; //swap between 0 and 1

    //Step 6: advect color
    color[colorIndex]->setupPass(GL_TEXTURE0); //Read/write to color
    velocity[velIndex]->setupTexture(GL_TEXTURE1); //Read from velocity
    glUseProgram(advectColorShader);
    glUniform1i(colorTextureAdvectColorUniform, 0);
    glUniform1i(velTextureAdvectColorUniform, 1);
    glUniform2fv(texelSizeAdvectColorUniform, 1, glm::value_ptr(windowTexelSize));
    glUniform1f(dtAdvectColorUniform, deltaTime * 1000);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //Visualize by blitting framebuffer to the screen
    //Color framebuffer is still bound to GL_FRAMEBUFFER
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, textureWidth, textureHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    getNextColor(drawColor, colorState);
}
