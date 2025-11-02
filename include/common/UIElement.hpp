class UIElement {
    public:
        UIElement(void (*clickFunction)(), float boundingBoxLeft, float boundingBoxRight, float boundingBoxTop, float boundingBoxBottom);
        bool isHovering(float x, float y) const;
        void checkClick(float x, float y);
        void onClick();
    private:
        void (*onClickFunction)();
        float boundingBoxLeft, boundingBoxRight, boundingBoxTop, boundingBoxBottom;
};