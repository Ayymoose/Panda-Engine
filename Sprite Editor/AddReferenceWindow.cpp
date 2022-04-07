#include "AddReferenceWindow.h"

void AddReferenceWindow::onOkButtonClicked()
{
    accept(); // override
}

void AddReferenceWindow::accept()
{
    if (!ui->_referenceLineEdit->text().isEmpty())
    {
        QDialog::accept();
        emit signalAddReferenceWindow(ui->_referenceLineEdit->text(),
                                      ui->_xIntegerLineEdit->text().toInt(),
                                      ui->_yIntegerLineEdit->text().toInt(),
                                      ui->_widthIntegerLineEdit->text().toInt(),
                                      ui->_heightIntegerLineEdit->text().toInt());
    }
    else
    {
        m_referenceFlashLineEdit->start();
    }
}

AddReferenceWindow::AddReferenceWindow(const QString& windowTextReference,
                                       const QString& reference,
                                       const PandaSpriteEditor::MinMax<int> x,
                                       const PandaSpriteEditor::MinMax<int> y,
                                       const PandaSpriteEditor::MinMax<int> width,
                                       const PandaSpriteEditor::MinMax<int> height,
                                       QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::AddReferenceWindow>())
{
    ui->setupUi(this);
    ui->_referenceLineEdit->setText(reference);

    ui->_xIntegerLineEdit->setText(QString::number(x.value));
    ui->_xIntegerLineEdit->setMinMax(x.min, x.max);

    ui->_yIntegerLineEdit->setText(QString::number(y.value));
    ui->_yIntegerLineEdit->setMinMax(y.min, y.max);

    ui->_widthIntegerLineEdit->setText(QString::number(width.value));
    ui->_widthIntegerLineEdit->setMinMax(width.min, width.max);

    ui->_heightIntegerLineEdit->setText(QString::number(height.value));
    ui->_heightIntegerLineEdit->setMinMax(height.min, height.max);

    setWindowTitle("Add reference for " + windowTextReference);

    m_referenceFlashLineEdit = std::make_unique<PandaSpriteEditor::FlashLineEdit>(*ui->_referenceLineEdit, 250,2);
}
