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

#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QFileDialog>
#include <QString>
#include <QScrollBar>

namespace
{
    constexpr int DEFAULT_GRID_X = 16;
    constexpr int DEFAULT_GRID_Y = 16;
    constexpr int DEFAULT_ROOM_X = 16;
    constexpr int DEFAULT_ROOM_Y = 16;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupDefaults();
    connectSignals();
}

void MainWindow::setupDefaults()
{
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(&m_canvas);
    ui->statusbar->addWidget(&m_zoomLabel);
    ui->statusbar->addWidget(&m_mouseLabel);

    ui->gridXSpinBox->setValue(DEFAULT_GRID_X);
    ui->gridYSpinBox->setValue(DEFAULT_GRID_Y);

    ui->roomSizeXSpinBox->setValue(DEFAULT_ROOM_X);
    ui->roomSizeYSpinBox->setValue(DEFAULT_ROOM_Y);
}

void MainWindow::connectSignals()
{
    connect(&m_canvas, &Canvas::signalUpdateMouse, this, &MainWindow::slotUpdateMouse);
    connect(&m_canvas, &Canvas::signalUpdateZoom, this, &MainWindow::slotUpdateZoom);
    connect(&m_canvas, &Canvas::signalScrollVBar, this, &MainWindow::slotScrollVBar);
    connect(&m_canvas, &Canvas::signalScrollBars, this, &MainWindow::slotScrollBars);

    connect(ui->gridXSpinBox, &QSpinBox::valueChanged, &m_canvas, &Canvas::slotGridXValueChanged);
    connect(ui->gridYSpinBox, &QSpinBox::valueChanged, &m_canvas, &Canvas::slotGridYValueChanged);
    connect(ui->enableGridCheckbox, &QCheckBox::toggled, this, &MainWindow::slotEnableGrid);
    connect(this, &MainWindow::signalEnableGrid, &m_canvas, &Canvas::slotEnableGrid);
    connect(ui->snapToGridCheckbox, &QCheckBox::toggled, &m_canvas, &Canvas::slotSnapToGrid);

    connect(this, &MainWindow::signalMoveMouseReferenceH, &m_canvas, &Canvas::slotMoveMouseReferenceH);
    connect(this, &MainWindow::signalMoveMouseReferenceV, &m_canvas, &Canvas::slotMoveMouseReferenceV);
}

void MainWindow::slotEnableGrid(bool enable)
{
    emit signalEnableGrid(enable, ui->gridXSpinBox->value(), ui->gridYSpinBox->value());
}

void MainWindow::slotUpdateMouse(double mx, double my)
{
    if (mx != -1 && my != -1)
    {
        m_mouseLabel.setText(QString::number(mx) + ", " + QString::number(my));
    }
    else
    {
        m_mouseLabel.setText("");
    }
}

void MainWindow::slotUpdateZoom(double zoomFactor)
{
    m_zoomLabel.setText("Zoom: " + QString::number(100 * zoomFactor) + '%');
}

void MainWindow::slotScrollVBar(int vscroll)
{
    auto vScrollBar = ui->scrollArea->verticalScrollBar();
    auto const cy = vScrollBar->value();
    vScrollBar->setValue(cy + vscroll);
}

void MainWindow::slotScrollBars(int dx, int dy)
{
    auto hScrollBar = ui->scrollArea->horizontalScrollBar();
    auto vScrollBar = ui->scrollArea->verticalScrollBar();
    auto const cx = hScrollBar->value();
    auto const cy = vScrollBar->value();

    hScrollBar->setValue(cx + dx);
    vScrollBar->setValue(cy + dy);

    // TODO: Fix bug here with middle mouse marker position at min/max scroll
    if (!(hScrollBar->value() == hScrollBar->minimum()))
    {
        if (dx < 0)
        {
            emit signalMoveMouseReferenceH(dx);
        }
    }
    if (!(hScrollBar->value() == hScrollBar->maximum()))
    {
        if (dx > 0)
        {
            emit signalMoveMouseReferenceH(dx);
        }
    }

    if (!(vScrollBar->value() == vScrollBar->minimum()))
    {
        if (dy < 0)
        {
            emit signalMoveMouseReferenceV(dy);
        }
    }
    if (!(vScrollBar->value() == vScrollBar->maximum()))
    {
        if (dy > 0)
        {
            emit signalMoveMouseReferenceV(dy);
        }
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_triggered()
{
    m_about.display();
}


void MainWindow::on_actionLoad_image_triggered()
{
    // TODO: File watcher on image updated
    auto const imageFilter = "PNG (*.png)";
    auto const image = QFileDialog::getOpenFileName(this, "Load image", R"(D:\Users\Ayman\Desktop\lacpp\Resources\Background\Dungeon)", imageFilter);
    if (!image.isEmpty())
    {
        if (!m_canvas.loadImage(image))
        {
            QMessageBox::critical(this, "Error", "Failed to load image: " + image);
        }
    }
}

