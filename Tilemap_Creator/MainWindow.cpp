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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(&m_canvas);

    connect(&m_canvas, &Canvas::signalScrollVBar, this, &MainWindow::slotScrollVBar);
    connect(&m_canvas, &Canvas::signalScrollBars, this, &MainWindow::slotScrollBars);
    connect(this, &MainWindow::signalMoveMouseReferenceH, &m_canvas, &Canvas::slotMoveMouseReferenceH);
    connect(this, &MainWindow::signalMoveMouseReferenceV, &m_canvas, &Canvas::slotMoveMouseReferenceV);
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
    ui->scrollArea->horizontalScrollBar()->setValue(100);

}


void MainWindow::on_actionLoad_image_triggered()
{
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

