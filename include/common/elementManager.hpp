#include <UIElement.hpp>
#include <dynamicArray.hpp>
#include <gpuBuffer.hpp>

typedef struct {
    float boundingBox[4];
    float roundingRadius;
    glm::vec4 color;
} UIElementData;

class ElementManager {
    public:
        ElementManager();
        void renderUI();
        void addElement(void (*onClickFunction)(), float boundingBox[4]);
    private:
        std::vector<UIElement> elements;
        uint32_t vao;

        dynamicArray<UIElementData> elementData;
        gpuBuffer elementDataBuffer;

        uint32_t roundedCornerShader;
};
