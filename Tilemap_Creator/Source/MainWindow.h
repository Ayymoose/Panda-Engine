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

#include <QMainWindow>
#include "About.h"
#include "Canvas.h"
#include "qlabel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString applicationName() const;

signals:
    void signalMoveMouseReferenceH(int);
    void signalMoveMouseReferenceV(int);
    void signalEnableGrid(bool, int, int);
    void signalGridXValueChanged(int);
    void signalGridYValueChanged(int);
    void signalRoomSizeXValueChanged(int);
    void signalRoomSizeYValueChanged(int);


public slots:
    void slotScrollBars(int, int);
    void slotScrollVBar(int);
    void slotUpdateZoom(double);
    void slotUpdateMouse(double, double);
    void slotEnableGrid(bool);

private slots:
    void on_actionAbout_triggered();
    void on_actionLoad_image_triggered();
    void on_generateTilemapButton_clicked();
    void on_saveToToolButton_clicked();

    void slotCheckWidget();
    void slotMapWidthValueChanged();
    void slotRoomSizeXValueChanged();
    void slotRoomSizeYValueChanged();

    void slotGridYValueChanged(int);
    void slotGridXValueChanged(int);

private:

    void connectSignals();

    void connectCanvasSignals();

    void setupDefaults();

    Ui::MainWindow *ui;
    About m_about;
    Canvas m_canvas;
    QLabel m_zoomLabel;
    QLabel m_mouseLabel;
    QLabel m_statusLabel;
    QLabel m_dimensionLabel;
};

