#pragma once

#include <QDialog>
#include <QString>
#include <QLineEdit>

#include <memory>

#include "MinMax.h"
#include "FlashLineEdit.h"

// generated includes
#include "ui_AddReferenceWindow.h"

namespace Ui
{
    class AddReferenceWindow;
}

class AddReferenceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddReferenceWindow(const QString& windowTextReference,
                                const QString& reference,
                                const PandaSpriteEditor::MinMax<int> x,
                                const PandaSpriteEditor::MinMax<int> y,
                                const PandaSpriteEditor::MinMax<int> width,
                                const PandaSpriteEditor::MinMax<int> height,
                                QWidget *parent = nullptr);

signals:
    void signalAddReferenceWindow(const QString& reference,
                                  const int x,
                                  const int y,
                                  const int width,
                                  const int height);

private slots:
    void onOkButtonClicked();


private:

    void accept() override;

    std::unique_ptr<Ui::AddReferenceWindow> ui;
    std::unique_ptr<PandaSpriteEditor::FlashLineEdit> m_referenceFlashLineEdit;
};

