#include <renderer.hpp>
#include <UIElement.hpp>
#include <dynamicArray.hpp>
class ElementManager {
    public:
        ElementManager();
        void renderUI();
        void addElement(void (*onClickFunction)(), float boundingBox[4]);
    private:
        std::vector<UIElement> elements;
        uint32_t vao;
        dynamicArray<float> vertices;
        gpuBuffer vertexBuffer;

        gpuBuffer indexBuffer;
        dynamicArray<uint32_t> indices;

        uint32_t roundedCornerShader;
};
