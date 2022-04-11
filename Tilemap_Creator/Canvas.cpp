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


Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    m_image.load(R"(D:\Users\Ayman\Desktop\lacpp\Resources\Background\Dungeon\dungeon_tail_cave.png)");
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
            auto const scale = std::min(CanvasDefaults::SCALE_MAX_LENGTH, vectorLength / 2);
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
        if (m_middleMouseButtonHeld)
        {
            drawMouseScrollCursor(painter);
        }

        if (m_enableGrid)
        {
            drawGridLines(painter);
        }

        if (m_drawPlacementMarker)
        {
            drawPlacementMarker(painter);
        }

        update();
    }
}

void Canvas::drawPlacementMarker(QPainter& painter)
{
    auto const mouseCoords = mapFromGlobal(QCursor::pos());
    auto const px = mouseCoords.x() / m_scale;
    auto const py = mouseCoords.y() / m_scale;

    if (withinCanvasImage(px + m_roomSizeX, py + m_roomSizeY) && withinCanvasImage(px, py))
    {
        painter.setOpacity(0.5);
        painter.setBrush(QBrush(Qt::green));
        Q_ASSERT(m_roomSizeX);
        Q_ASSERT(m_roomSizeY);

        painter.drawRect(px, py, m_roomSizeX, m_roomSizeY);
    }
}

bool Canvas::withinCanvasImage(int x, int y) const
{
    // Includes scaled co-ordinates
    return (x >= 0 && x <= m_image.width() && y >=0 && y <= m_image.height());
}

void Canvas::slotEnableGrid(bool enable, int gx, int gy)
{
    m_enableGrid = enable;
    m_gridX = gx;
    m_gridY = gy;
}

void Canvas::slotGridXValueChanged(int dx)
{
    m_gridX = dx;
}

void Canvas::slotGridYValueChanged(int dy)
{
    m_gridY = dy;
}

void Canvas::slotRoomSizeXValueChanged(int rx)
{
    m_roomSizeX = rx;
}

void Canvas::slotRoomSizeYValueChanged(int ry)
{
    m_roomSizeY = ry;
}

void Canvas::slotSnapToGrid(bool enable)
{
    m_snapToGrid = enable;
}

void Canvas::drawCanvasImage(QPainter& painter)
{
    Q_ASSERT(!m_image.isNull());
    setMinimumSize(m_image.width() * m_scale, m_image.height() * m_scale);
    painter.drawImage(QRectF(0,0, m_image.width(), m_image.height()), m_image);
}

void Canvas::drawMouseScrollCursor(QPainter& painter)
{
    Q_ASSERT(!m_cursorImage.isNull());
    auto const cursorX = (m_reference.x() / m_scale) - (m_cursorImage.width() / 2);
    auto const cursorY = (m_reference.y() / m_scale) - (m_cursorImage.height() / 2);
    painter.drawImage(QPointF(cursorX, cursorY), m_cursorImage);
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
        updateMouseCoordinates();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent*)
{
    m_middleMouseButtonHeld = false;
    m_mouseMoveTimer.stop();
}

void Canvas::updateMouseCoordinates() const
{
    auto const mouse = mapFromGlobal(QCursor::pos());
    auto const mx = (int)(mouse.x() / m_scale);
    auto const my = (int)(mouse.y() / m_scale);

    if (withinCanvasImage(mx, my))
    {
        emit signalUpdateMouse(mx, my);
    }
    else
    {
        emit signalUpdateMouse(-1,-1);
    }
}

void Canvas::drawGridLines(QPainter& painter)
{
    Q_ASSERT(!m_image.isNull());

    painter.setPen(QPen(Qt::black));

    for (int x=0; x < m_image.width(); x+=m_gridX)
    {
        painter.drawLine(QLine(x, 0, x, m_image.height()));
    }
    for (int y=0; y < m_image.height(); y+=m_gridY)
    {
        painter.drawLine(QLine(0, y, m_image.width(), y));
    }
}

void Canvas::zoomCanvasImage(int delta)
{
    if (delta > 0)
    {
        // TODO: Maybe limit the maximum here
        m_scale += CanvasDefaults::ZOOM_FACTOR;
    }
    else
    {
        m_scale = std::max(CanvasDefaults::ZOOM_FACTOR, m_scale - CanvasDefaults::ZOOM_FACTOR);
    }

    updateMouseCoordinates();

    emit signalUpdateZoom(m_scale);
}

void Canvas::scrollCanvasBars(int delta)
{
    if (delta > 0)
    {
        emit signalScrollVBar(-CanvasDefaults::VSCROLL_INCREMENT);
    }
    else
    {
        emit signalScrollVBar(CanvasDefaults::VSCROLL_INCREMENT);
    }

    updateMouseCoordinates();
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
        m_scale = CanvasDefaults::DEFAULT_CANVAS_SCALE;
        setMinimumSize(m_image.width(), m_image.height());
        emit signalUpdateZoom(m_scale);
    }
    return loaded;
}
