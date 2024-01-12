#pragma once

#include <QWidget>
#include <QColorDialog>
#include <QPainter>
#include <QMouseEvent>

namespace PandaSpriteEditor
{
    class ColourPickerWidget : public QWidget
    {
        Q_OBJECT

    public:
        ColourPickerWidget(QWidget* parent = nullptr);

        QColor getColour() const;

    private:
        void paintEvent(QPaintEvent*) override;

        void mousePressEvent(QMouseEvent*) override;

        QColor m_colour;
    };
}


