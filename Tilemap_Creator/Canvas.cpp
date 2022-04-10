// Copyright (C) 2022  Ayymooose

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Canvas.h"
#include <QPainter>
#include <QtGlobal>
#include <QGuiApplication>
#include <QScrollBar>

namespace
{
    constexpr double ZOOM_FACTOR = 0.125f;
    constexpr double SCALE_MAX_LENGTH = 8;
    constexpr int VSCROLL_INCREMENT = 32;
}

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    m_cursorImage.load(":/images/Images/move_all_cursor.png");
    connect(&m_mouseMoveTimer, &QTimer::timeout, this, &Canvas::middleMouseScrollBars);
}

void Canvas::middleMouseScrollBars()
{
    if (m_middleMouseButtonHeld)
    {
        if (parentWidget())
        {
            // Scale by distance
            auto const vectorLength = std::sqrt(m_delta.x() * m_delta.x() + m_delta.y() * m_delta.y());

            if (vectorLength != 0)
            {
                auto const scale = std::min(SCALE_MAX_LENGTH, vectorLength / 2);
                auto const dx = scale * (m_delta.x() / vectorLength);
                auto const dy = scale * (m_delta.y() / vectorLength);
                emit signalScrollBars(dx,dy);
            }
        }
    }
}

void Canvas::slotMoveMouseReferenceH(int dx)
{
    m_reference.rx() += dx;
}

void Canvas::slotMoveMouseReferenceV(int dy)
{
    m_reference.ry() += dy;
}

void Canvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.scale(m_scale, m_scale);


    if (!m_image.isNull())
    {
        // TODO: Remove image in future
        setMinimumSize(m_image.width() * m_scale, m_image.height() * m_scale);

        painter.drawImage(QRectF(0,0, m_image.width(), m_image.height()), m_image);

        if (m_middleMouseButtonHeld)
        {
            auto const cursorX = (m_reference.x() / m_scale) - (m_cursorImage.width() / 2);
            auto const cursorY = (m_reference.y() / m_scale) - (m_cursorImage.height() / 2);
            painter.drawImage(QPointF(cursorX, cursorY), m_cursorImage);
            update();
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{

    switch(event->button())
    {
    case Qt::MiddleButton:
        m_reference = event->pos();
        m_delta = QPoint(0,0);
        m_middleMouseButtonHeld = true;
        m_mouseMoveTimer.start(5);
        update();
        break;
    default:
        break;
    }

}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    m_delta = event->pos()  - m_reference;
}

void Canvas::mouseReleaseEvent(QMouseEvent *)
{
    m_middleMouseButtonHeld = false;
    m_mouseMoveTimer.stop();
}

void Canvas::wheelEvent(QWheelEvent* event)
{
    if (!m_image.isNull())
    {
        // Enable zooming
        if (event->modifiers() & Qt::ControlModifier)
        {
            auto const yDelta = event->angleDelta().y();
            if (yDelta > 0)
            {
                // TODO: Maybe limit the maximum here
                m_scale += ZOOM_FACTOR;
            }
            else
            {
                m_scale = std::max(ZOOM_FACTOR, m_scale - ZOOM_FACTOR);
            }
            update();
        }
        else
        {
            // Scroll the bars normally
            auto const yDelta = event->angleDelta().y();
            if (yDelta > 0)
            {
                emit signalScrollVBar(-VSCROLL_INCREMENT);
            }
            else
            {
                emit signalScrollVBar(VSCROLL_INCREMENT);

            }
        }
    }
}

bool Canvas::loadImage(const QString& imagePath)
{
    Q_ASSERT(!imagePath.isEmpty());
    bool loaded = m_image.load(imagePath);
    if (loaded)
    {
        m_scale = 1;
        setMinimumSize(m_image.width(), m_image.height());
    }
    return loaded;
}
