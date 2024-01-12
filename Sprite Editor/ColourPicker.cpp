#include "ColourPicker.h"

using namespace PandaSpriteEditor;

ColourPickerWidget::ColourPickerWidget(QWidget* parent)
    : QWidget(parent),
      m_colour(Qt::white)
{

}

void ColourPickerWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_colour);
}

void ColourPickerWidget::mousePressEvent(QMouseEvent*)
{
    const auto newColor = QColorDialog::getColor(m_colour, this, "Pick a Colour");
    if (newColor.isValid())
    {
        m_colour = newColor;
        update();  // Trigger a repaint to show the new color
    }
}

QColor ColourPickerWidget::getColour() const
{
    return m_colour;
}
