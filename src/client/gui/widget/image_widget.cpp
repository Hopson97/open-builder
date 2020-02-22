#include "../widget.h"

#include "../component.h"

namespace gui {

ImageWidget::ImageWidget(RectangleComponent* rectangleComponent)
    : mp_rectangle(rectangleComponent)
{
}
void ImageWidget::setPosition(const GuiDimension& position)
{
    mp_rectangle->setPosition(position);
}

void ImageWidget::setSize(const GuiDimension& size)
{
    mp_rectangle->setSize(size);
}

void ImageWidget::setImage(int image)
{
    mp_rectangle->setTexture(image);
}

} // namespace gui
