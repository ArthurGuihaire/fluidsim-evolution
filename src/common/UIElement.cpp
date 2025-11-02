#include <UIElement.hpp>
#include <glm/glm.hpp>
UIElement::UIElement(void (*clickFunction)(), float boundingBoxLeft, float boundingBoxRight, float boundingBoxTop, float boundingBoxBottom)
 : onClickFunction(clickFunction), boundingBoxLeft(boundingBoxLeft), boundingBoxRight(boundingBoxRight), boundingBoxTop(boundingBoxTop), boundingBoxBottom(boundingBoxBottom)
{
}

bool UIElement::isHovering(float x, float y) const {
    return x >= boundingBoxLeft && x <= boundingBoxRight && x <= boundingBoxTop && x >= boundingBoxBottom;
}

void UIElement::checkClick(float x, float y) {
    if (isHovering(x, y))
        onClick();
}

void UIElement::onClick() {} // do nothing, this is overwritten in constructor
