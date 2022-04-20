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
#include "Version.h"
#include "Compiler.h"
#include "Tilemap.h"
#include "CodeGenerator.h"
#include "CppCodeGenerator.h"

#include <QFileDialog>
#include <QString>
#include <QScrollBar>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <memory>

namespace
{
    constexpr char DEFAULT_TILEMAP_NAME[] = "TILEMAP_1";
    constexpr int DEFAULT_MAP_WIDTH = 160;

    // https://stackoverflow.com/questions/3407012/rounding-up-to-the-nearest-multiple-of-a-number
    int roundTo(int number, int multiple)
    {
        Q_ASSERT(number != 0);
        Q_ASSERT(number > 0);
        return ((number + multiple - 1) / multiple) * multiple;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupCanvas();
    setupToolbar();
    setupStatusBar();
    setupMainWindow();
    setupDefaults();
    slotCheckWidget();
}

QString MainWindow::applicationName() const
{
    return APPLICATION_NAME " " BUILD_VERSION " " BUILD_ARCHITECTURE;
}

void MainWindow::setupDefaults()
{
    ui->gridXSpinBox->setValue(CanvasDefaults::DEFAULT_GRID_X);
    ui->gridYSpinBox->setValue(CanvasDefaults::DEFAULT_GRID_Y);

    ui->roomSizeXSpinBox->setValue(CanvasDefaults::DEFAULT_ROOM_X);
    ui->roomSizeYSpinBox->setValue(CanvasDefaults::DEFAULT_ROOM_Y);

    ui->levelNameEdit->setText(DEFAULT_TILEMAP_NAME);

    ui->mapWidthSpinBox->setValue(DEFAULT_MAP_WIDTH);
    ui->mapWidthSpinBox->setSingleStep(ui->gridXSpinBox->value());

    ui->roomSizeXSpinBox->setSingleStep(ui->gridXSpinBox->value());
    ui->roomSizeXSpinBox->setMinimum(ui->gridXSpinBox->value());

    ui->roomSizeYSpinBox->setSingleStep(ui->gridYSpinBox->value());
    ui->roomSizeYSpinBox->setMinimum(ui->gridYSpinBox->value());

    ui->levelSaveToEdit->setText(QDir::rootPath() + DEFAULT_TILEMAP_NAME + ".png");

}

void MainWindow::setupCanvas()
{
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(&m_canvas);

    connect(&m_canvas, &Canvas::signalUpdateMouse, this, &MainWindow::slotUpdateMouse);
    connect(&m_canvas, &Canvas::signalUpdateZoom, this, &MainWindow::slotUpdateZoom);
    connect(&m_canvas, &Canvas::signalScrollVBar, this, &MainWindow::slotScrollVBar);
    connect(&m_canvas, &Canvas::signalScrollBars, this, &MainWindow::slotScrollBars);

    connect(ui->roomSizeYSpinBox, &QSpinBox::valueChanged, &m_canvas, &Canvas::slotRoomSizeYValueChanged);

    connect(this, &MainWindow::signalGridXValueChanged, &m_canvas, &Canvas::slotGridXValueChanged);
    connect(this, &MainWindow::signalGridYValueChanged, &m_canvas, &Canvas::slotGridYValueChanged);

    connect(this, &MainWindow::signalEnableGrid, &m_canvas, &Canvas::slotEnableGrid);
    connect(ui->snapToGridCheckbox, &QCheckBox::toggled, &m_canvas, &Canvas::slotSnapToGrid);

    connect(this, &MainWindow::signalMoveMouseReferenceH, &m_canvas, &Canvas::slotMoveMouseReferenceH);
    connect(this, &MainWindow::signalMoveMouseReferenceV, &m_canvas, &Canvas::slotMoveMouseReferenceV);

    connect(ui->roomSizeXSpinBox, &QSpinBox::editingFinished, this, &MainWindow::slotRoomSizeXValueChanged);
    connect(this, &MainWindow::signalRoomSizeXValueChanged, &m_canvas, &Canvas::slotRoomSizeXValueChanged);

    connect(ui->roomSizeYSpinBox, &QSpinBox::editingFinished, this, &MainWindow::slotRoomSizeYValueChanged);
    connect(this, &MainWindow::signalRoomSizeYValueChanged, &m_canvas, &Canvas::slotRoomSizeYValueChanged);

    connect(this, &MainWindow::signalEnablePlaceRooms, &m_canvas, &Canvas::slotEnablePlaceRooms);
    connect(this, &MainWindow::signalEnableLinkRooms, &m_canvas, &Canvas::slotEnableLinkRooms);

    connect(this, &MainWindow::signalPlaceAllRooms, &m_canvas, &Canvas::slotPlaceAllRooms);
    connect(this, &MainWindow::signalClearAllRooms, &m_canvas, &Canvas::slotClearAllRooms);
}

void MainWindow::setupStatusBar()
{
    ui->statusbar->addWidget(&m_zoomLabel);
    ui->statusbar->addWidget(&m_mouseLabel);
    ui->statusbar->addWidget(&m_dimensionLabel);
    ui->statusbar->addPermanentWidget(&m_statusLabel);
}

void MainWindow::setupToolbar()
{
    m_placeRoomsAction.setIcon(QPixmap(":/Images/cursor.svg"));
    m_placeRoomsAction.setToolTip("Place rooms");
    m_placeRoomsAction.setCheckable(true);

    m_linkRoomsAction.setIcon(QPixmap(":/Images/link.svg"));
    m_linkRoomsAction.setToolTip("Link rooms");
    m_linkRoomsAction.setCheckable(true);

    connect(&m_placeRoomsAction, &QAction::toggled, this, &MainWindow::slotPlaceRoomsToggled);
    connect(&m_linkRoomsAction, &QAction::toggled, this, &MainWindow::slotLinkRoomsToggled);

    ui->toolBar->addAction(&m_placeRoomsAction);
    ui->toolBar->addAction(&m_linkRoomsAction);
}

void MainWindow::slotLinkRoomsToggled(bool enableLinkRooms)
{
    emit signalEnableLinkRooms(enableLinkRooms);
}

void MainWindow::slotPlaceRoomsToggled(bool enablePlaceRooms)
{
    emit signalEnablePlaceRooms(enablePlaceRooms);
}


void MainWindow::setupMainWindow()
{
    connect(ui->gridXSpinBox, &QSpinBox::valueChanged, this, &MainWindow::slotGridXValueChanged);
    connect(ui->gridYSpinBox, &QSpinBox::valueChanged, this, &MainWindow::slotGridYValueChanged);

    connect(ui->mapWidthSpinBox, &QSpinBox::editingFinished, this, &MainWindow::slotMapWidthValueChanged);
    connect(ui->enableGridCheckbox, &QCheckBox::toggled, this, &MainWindow::slotEnableGrid);
    connect(ui->levelNameEdit, &QLineEdit::textChanged, this, &MainWindow::slotCheckWidget);
    connect(ui->levelSaveToEdit, &QLineEdit::textChanged, this, &MainWindow::slotCheckWidget);

}

void MainWindow::slotMapWidthValueChanged()
{
    ui->mapWidthSpinBox->setValue(roundTo(ui->mapWidthSpinBox->value(), ui->gridXSpinBox->value()));
}


void MainWindow::slotRoomSizeXValueChanged()
{
    ui->roomSizeXSpinBox->setSingleStep(ui->gridXSpinBox->value());
    ui->roomSizeXSpinBox->setMinimum(ui->gridXSpinBox->value());
    auto const value = ui->roomSizeXSpinBox->value();
    ui->roomSizeXSpinBox->setValue(roundTo(value, ui->gridXSpinBox->value()));
    emit signalRoomSizeXValueChanged(value);
}

void MainWindow::slotRoomSizeYValueChanged()
{
    ui->roomSizeYSpinBox->setSingleStep(ui->gridYSpinBox->value());
    ui->roomSizeYSpinBox->setMinimum(ui->gridYSpinBox->value());
    auto const value = ui->roomSizeYSpinBox->value();
    ui->roomSizeYSpinBox->setValue(roundTo(value, ui->gridYSpinBox->value()));
    emit signalRoomSizeYValueChanged(value);
}

void MainWindow::slotGridYValueChanged(int value)
{
    ui->roomSizeYSpinBox->setSingleStep(ui->gridYSpinBox->value());
    ui->roomSizeYSpinBox->setMinimum(ui->gridYSpinBox->value());
    ui->roomSizeYSpinBox->setValue(roundTo(ui->roomSizeYSpinBox->value(), ui->gridYSpinBox->value()));

    emit signalGridYValueChanged(value);
}

void MainWindow::slotGridXValueChanged(int value)
{
    ui->mapWidthSpinBox->setSingleStep(ui->gridXSpinBox->value());
    ui->mapWidthSpinBox->setMinimum(ui->gridXSpinBox->value());
    ui->mapWidthSpinBox->setMaximum(ui->gridXSpinBox->value() * 64);
    ui->mapWidthSpinBox->setValue(ui->gridXSpinBox->value() * 10);

    ui->roomSizeXSpinBox->setSingleStep(ui->gridXSpinBox->value());
    ui->roomSizeXSpinBox->setMinimum(ui->gridXSpinBox->value());
    ui->roomSizeXSpinBox->setValue(roundTo(ui->roomSizeXSpinBox->value(), ui->gridXSpinBox->value()));

    emit signalGridXValueChanged(value);
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

void MainWindow::setLimitsFromImage(const QImage& image)
{
    ui->roomSizeXSpinBox->setMaximum(image.width());
    ui->roomSizeYSpinBox->setMaximum(image.height());
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
            setWindowTitle(applicationName());
            m_dimensionLabel.setText("");
        }
        else
        {
            m_dimensionLabel.setText("(WxH) " +
                                     QString::number(m_canvas.canvasImage().width()) +
                                     ',' +
                                     QString::number(m_canvas.canvasImage().height()));
            setWindowTitle(applicationName() + " - " + image);
            setLimitsFromImage(QImage(image));
        }
    }
}


void MainWindow::on_generateTilemapButton_clicked()
{
    auto const savePath = ui->levelSaveToEdit->text().trimmed();
    auto rooms = m_canvas.rooms();
    QString status;

    if (rooms.size())
    {
        // TODO: Have tile dimensions spinboxes instead of relying on grid here
        Tilemap::TilemapConfig config;
        config.outputMapWidth = ui->mapWidthSpinBox->value();
        config.tileX = ui->gridXSpinBox->value();
        config.tileY = ui->gridYSpinBox->value();

        auto const& [generatedAreas, generatedTileMap] = Tilemap::generate(m_canvas.canvasImage(), config, rooms);
        auto const generatedCodeFilePath = QFileInfo(savePath).path() + DEFAULT_TILEMAP_NAME + ".txt";
        auto generatedCodeFile = QFile(generatedCodeFilePath);

        // TODO: Have SUCCESSFULLY appear in green color and FAILED in red
        if (generatedCodeFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            // TODO: Have option to select code generator
            QTextStream out(&generatedCodeFile);
            std::unique_ptr<CodeGenerator> codeGenerator = std::make_unique<CppCodeGenerator>();
            codeGenerator->generate(out, generatedAreas);
            generatedCodeFile.close();
            status += "SUCCESSFULLY saved generated code to: " + generatedCodeFilePath;
            status += '\t';
        }
        else
        {
            status += "FAILED to save generated code to: " + generatedCodeFilePath;
            status += '\t';
        }

        auto const saved = generatedTileMap.save(savePath);
        if (!saved)
        {
            status += "FAILED to save generated tilemap to: " + savePath;
        }
        else
        {
            status += "SUCCESSFULLY saved generated tilemap to: " + savePath;
        }
    }
    else
    {
        status += "No areas placed to generate tilemap";
    }

    m_statusLabel.setText(status);
}


void MainWindow::on_saveToToolButton_clicked()
{
    auto const savePath = QFileDialog::getSaveFileName(this, "Save as", QDir::currentPath());
    if (!savePath.isEmpty())
    {
        ui->levelSaveToEdit->setText(savePath);
    }
}


void MainWindow::on_actionPlace_all_rooms_triggered()
{
    emit signalPlaceAllRooms();
}

void MainWindow::on_actionClear_all_rooms_triggered()
{
    emit signalClearAllRooms();
}

