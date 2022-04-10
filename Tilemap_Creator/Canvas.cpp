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
    setMouseTracking(true);
}

void Canvas::middleMouseScrollBars()
{
    if (m_middleMouseButtonHeld)
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
    if (!m_image.isNull())
    {
        QPainter painter(this);
        painter.scale(m_scale, m_scale);

        drawCanvasImage(painter);
        // TODO: Remove once bug fixed in mainwindow
        drawMouseScrollCursor(painter);
    }
}

void Canvas::drawCanvasImage(QPainter& painter)
{
    setMinimumSize(m_image.width() * m_scale, m_image.height() * m_scale);
    painter.drawImage(QRectF(0,0, m_image.width(), m_image.height()), m_image);
}

void Canvas::drawMouseScrollCursor(QPainter& painter)
{
    if (m_middleMouseButtonHeld)
    {
        auto const cursorX = (m_reference.x() / m_scale) - (m_cursorImage.width() / 2);
        auto const cursorY = (m_reference.y() / m_scale) - (m_cursorImage.height() / 2);
        painter.drawImage(QPointF(cursorX, cursorY), m_cursorImage);
        update();
    }
}

void Canvas::mouseMarkerReference(const QPoint& reference)
{
    m_reference = reference;
    m_delta = QPoint(0,0);
    m_middleMouseButtonHeld = true;
    m_mouseMoveTimer.start(5);
    update();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{

    switch(event->button())
    {
    case Qt::MiddleButton:
        mouseMarkerReference(event->pos());
        break;
    default:
        break;
    }
}

void Canvas::leaveEvent(QEvent*)
{
    emit signalUpdateMouse(-1,-1);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_image.isNull())
    {
        m_delta = event->pos() - m_reference;
        updateMouseCoordinates(event->pos());
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent*)
{
    m_middleMouseButtonHeld = false;
    m_mouseMoveTimer.stop();
}

void Canvas::updateMouseCoordinates(const QPointF& mouse) const
{
    auto const mx = (int)(mouse.x() / m_scale);
    auto const my = (int)(mouse.y() / m_scale);

    if (mx < m_image.width() && my < m_image.height())
    {
        emit signalUpdateMouse(mx, my);
    }
    else
    {
        emit signalUpdateMouse(-1,-1);
    }
}

void Canvas::zoomCanvasImage(int delta)
{
    if (delta > 0)
    {
        // TODO: Maybe limit the maximum here
        m_scale += ZOOM_FACTOR;
    }
    else
    {
        m_scale = std::max(ZOOM_FACTOR, m_scale - ZOOM_FACTOR);
    }

    auto const mouseCoords = mapFromGlobal(QCursor::pos());
    updateMouseCoordinates(mouseCoords);

    emit signalUpdateZoom(m_scale);
    update();
}

void Canvas::scrollCanvasBars(int delta)
{
    if (delta > 0)
    {
        emit signalScrollVBar(-VSCROLL_INCREMENT);
    }
    else
    {
        emit signalScrollVBar(VSCROLL_INCREMENT);
    }
}

void Canvas::wheelEvent(QWheelEvent* event)
{
    if (!m_image.isNull())
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            // Zoom canvas
            zoomCanvasImage(event->angleDelta().y());
        }
        else
        {
            // Scroll the bars normally
            scrollCanvasBars(event->angleDelta().y());
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
        emit signalUpdateZoom(m_scale);
    }
    return loaded;
}
