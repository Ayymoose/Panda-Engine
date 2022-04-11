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

#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollArea>
#include <QTimer>

namespace CanvasDefaults
{
    constexpr int DEFAULT_GRID_X = 16;
    constexpr int DEFAULT_GRID_Y = 16;
    constexpr int DEFAULT_ROOM_X = 160;
    constexpr int DEFAULT_ROOM_Y = 128;
    constexpr double DEFAULT_CANVAS_SCALE = 1;
    constexpr double ZOOM_FACTOR = 0.125f;
    constexpr double SCALE_MAX_LENGTH = 8;
    constexpr int VSCROLL_INCREMENT = 32;
}

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    bool loadImage(const QString& imagePath);

signals:
    void signalScrollBars(int, int);
    void signalScrollVBar(int);
    void signalUpdateZoom(double);
    void signalUpdateMouse(double, double) const;

public slots:
    void slotMoveMouseReferenceH(int);
    void slotMoveMouseReferenceV(int);
    void slotEnableGrid(bool, int, int);
    void slotSnapToGrid(bool);
    void slotGridXValueChanged(int);
    void slotGridYValueChanged(int);
    void slotRoomSizeXValueChanged(int);
    void slotRoomSizeYValueChanged(int);

protected:
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
private:

    void middleMouseScrollBars();
    void updateMouseCoordinates() const;
    void drawMouseScrollCursor(QPainter& painter);
    void drawCanvasImage(QPainter& painter);
    void zoomCanvasImage(int delta);
    void scrollCanvasBars(int delta);
    void mouseMarkerReference(const QPoint& reference);

    void drawGridLines(QPainter& painter);
    void drawPlacementMarker(QPainter& painter);


    bool withinCanvasImage(int x, int y) const;

    QImage m_image;
    double m_scale{CanvasDefaults::DEFAULT_CANVAS_SCALE};

    QPoint m_reference;
    QPoint m_delta;
    bool m_middleMouseButtonHeld{false};

    QTimer m_mouseMoveTimer;
    QImage m_cursorImage;

    bool m_enableGrid{false};
    int m_gridX{0};
    int m_gridY{0};

    bool m_drawPlacementMarker{true};
    int m_roomSizeX{CanvasDefaults::DEFAULT_ROOM_X};
    int m_roomSizeY{CanvasDefaults::DEFAULT_ROOM_Y};


    bool m_snapToGrid{false};
};
