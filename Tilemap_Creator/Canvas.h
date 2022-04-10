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

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    bool loadImage(const QString& imagePath);

signals:
    void signalScrollBars(int, int);
    void signalScrollVBar(int);

public slots:
    void slotMoveMouseReferenceH(int);
    void slotMoveMouseReferenceV(int);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:

    void middleMouseScrollBars();

    QImage m_image;
    double m_scale{1};

    QPoint m_reference;
    QPoint m_delta;
    bool m_middleMouseButtonHeld{false};

    QTimer m_mouseMoveTimer;
    QImage m_cursorImage;
};
