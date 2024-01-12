#include "AddLayerWindow.h"

AddLayerWindow::AddLayerWindow(const QString& title,
                               const QString& defaultLayerName,
                               const PandaSpriteEditor::MinMax<int>& x,
                               const PandaSpriteEditor::MinMax<int>& y,
                               const PandaSpriteEditor::MinMax<int>& width,
                               const PandaSpriteEditor::MinMax<int>& height,
                               QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::AddLayerWindow>())
{
    ui->setupUi(this);
    setWindowTitle(title);

    // TODO: This is a hack so that the UI doesn't force the bottom widgets down
    // when the text is shown
    ui->_errorLabel->setText("");


    ui->_layerNameLineEdit->setText(defaultLayerName);
    ui->_xIntegerLineEdit->setText(QString::number(x.value));
    ui->_xIntegerLineEdit->setMinMax(x.min, x.max);
    ui->_yIntegerLineEdit->setText(QString::number(y.value));
    ui->_yIntegerLineEdit->setMinMax(y.min, y.max);
    ui->_widthIntegerLineEdit->setText(QString::number(width.value));
    ui->_widthIntegerLineEdit->setMinMax(width.min, width.max);
    ui->_heightIntegerLineEdit->setText(QString::number(height.value));
    ui->_heightIntegerLineEdit->setMinMax(height.min, height.max);

    connect(ui->_okButton, &QAbstractButton::clicked, this, [this](){
        const auto layerName = ui->_layerNameLineEdit->text();
        if (layerName.isEmpty())
        {
            ui->_errorLabel->setText("Layer name is empty");
        }
        else
        {
            // Receiver will check if this layer name exists and connect to slotCheckLayerName()
            // below
            emit signalCheckLayerName(layerName);
        }
    });
}


void AddLayerWindow::slotCheckLayerName(bool layerNameOk)
{
    if (layerNameOk)
    {
        // Receiver connects to this
        emit signalAddOrEditLayer(ui->_layerNameLineEdit->text(),
                                  ui->_xIntegerLineEdit->text().toInt(),
                                  ui->_yIntegerLineEdit->text().toInt(),
                                  ui->_widthIntegerLineEdit->text().toInt(),
                                  ui->_heightIntegerLineEdit->text().toInt(),
                                  ui->_colourPickerWidget->getColour());
        accept();
    }
    else
    {
        ui->_errorLabel->setText("Layer name already exists");
    }
}

