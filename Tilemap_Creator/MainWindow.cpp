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
#include <QDir>
#include "Tilemap.h"

namespace
{
    constexpr char DEFAULT_TILEMAP_NAME[] = "TILEMAP_1";
    constexpr int DEFAULT_MAP_WIDTH = 160;
}

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
    ui->statusbar->addWidget(&m_statusLabel);

    ui->gridXSpinBox->setValue(CanvasDefaults::DEFAULT_GRID_X);
    ui->gridYSpinBox->setValue(CanvasDefaults::DEFAULT_GRID_Y);

    ui->roomSizeXSpinBox->setValue(CanvasDefaults::DEFAULT_ROOM_X);
    ui->roomSizeYSpinBox->setValue(CanvasDefaults::DEFAULT_ROOM_Y);

    ui->levelNameEdit->setText(DEFAULT_TILEMAP_NAME);
    ui->mapWidthSpinBox->setValue(DEFAULT_MAP_WIDTH);

    ui->mapWidthSpinBox->setSingleStep(ui->gridXSpinBox->value());

    ui->levelSaveToEdit->setText(QDir::rootPath());

    slotCheckWidget();
}

void MainWindow::connectSignals()
{
    connect(&m_canvas, &Canvas::signalUpdateMouse, this, &MainWindow::slotUpdateMouse);
    connect(&m_canvas, &Canvas::signalUpdateZoom, this, &MainWindow::slotUpdateZoom);
    connect(&m_canvas, &Canvas::signalScrollVBar, this, &MainWindow::slotScrollVBar);
    connect(&m_canvas, &Canvas::signalScrollBars, this, &MainWindow::slotScrollBars);

    connect(ui->roomSizeXSpinBox, &QSpinBox::valueChanged, &m_canvas, &Canvas::slotRoomSizeXValueChanged);
    connect(ui->roomSizeYSpinBox, &QSpinBox::valueChanged, &m_canvas, &Canvas::slotRoomSizeYValueChanged);

    connect(ui->gridXSpinBox, &QSpinBox::valueChanged, &m_canvas, &Canvas::slotGridXValueChanged);
    connect(ui->gridYSpinBox, &QSpinBox::valueChanged, &m_canvas, &Canvas::slotGridYValueChanged);
    connect(ui->enableGridCheckbox, &QCheckBox::toggled, this, &MainWindow::slotEnableGrid);
    connect(this, &MainWindow::signalEnableGrid, &m_canvas, &Canvas::slotEnableGrid);
    connect(ui->snapToGridCheckbox, &QCheckBox::toggled, &m_canvas, &Canvas::slotSnapToGrid);

    connect(this, &MainWindow::signalMoveMouseReferenceH, &m_canvas, &Canvas::slotMoveMouseReferenceH);
    connect(this, &MainWindow::signalMoveMouseReferenceV, &m_canvas, &Canvas::slotMoveMouseReferenceV);

    connect(ui->levelNameEdit, &QLineEdit::textChanged, this, &MainWindow::slotCheckWidget);
    connect(ui->levelSaveToEdit, &QLineEdit::textChanged, this, &MainWindow::slotCheckWidget);

}

void MainWindow::slotCheckWidget()
{
    auto const levelName = ui->levelNameEdit->text().trimmed();
    auto const savePath = ui->levelSaveToEdit->text().trimmed();
    if (levelName.isEmpty() || savePath.isEmpty())
    {
        ui->generateTilemapButton->setEnabled(false);
    }
    else
    {
        ui->generateTilemapButton->setEnabled(true);
    }
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
    // TODO: Support other image types
    auto const imageFilter = "PNG (*.png)";
    auto const image = QFileDialog::getOpenFileName(this, "Load image", QDir::currentPath(), imageFilter);
    if (!image.isEmpty())
    {
        if (!m_canvas.loadImage(image))
        {
            QMessageBox::critical(this, "Error", "Failed to load image: " + image);
        }
    }
}


void MainWindow::on_generateTilemapButton_clicked()
{
    auto const savePath = ui->levelSaveToEdit->text().trimmed();
    auto const placements = m_canvas.placements();
    if (placements.size())
    {
        Tilemap::TilemapConfig config;
        config.outputMapWidth = ui->mapWidthSpinBox->value();
        config.tileX = ui->gridXSpinBox->value();
        config.tileY = ui->gridYSpinBox->value();

        auto const tileMap = Tilemap::generate(m_canvas.canvasImage(), config, placements);
        auto const saved = tileMap.save(savePath);
        if (!saved)
        {
            m_statusLabel.setText("FAILED to save generated tilemap to: " + savePath);
        }
        else
        {
            m_statusLabel.setText("SUCCESSFULLY saved generated tilemap to: " + savePath + " at " + QDateTime::currentDateTime().toString());
        }
    }
    else
    {
        m_statusLabel.setText("No areas placed to generate tilemap");
    }

}


void MainWindow::on_saveToToolButton_clicked()
{
    auto const savePath = QFileDialog::getSaveFileName(this, "Save as", QDir::currentPath());
    if (!savePath.isEmpty())
    {
        ui->levelSaveToEdit->setText(savePath);
    }
}

