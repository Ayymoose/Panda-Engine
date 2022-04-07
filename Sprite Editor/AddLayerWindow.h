#pragma once

#include "MinMax.h"

#include <memory>

// generated includes
#include "ui_AddLayerWindow.h"

namespace Ui
{
    class AddLayerWindow;
}

class AddLayerWindow : public QDialog
{
    Q_OBJECT

public:

    explicit AddLayerWindow(const QString& title,
                            const QString& defaultLayerName,
                            const PandaSpriteEditor::MinMax<int>& x,
                            const PandaSpriteEditor::MinMax<int>& y,
                            const PandaSpriteEditor::MinMax<int>& width,
                            const PandaSpriteEditor::MinMax<int>& height,
                            QWidget *parent = nullptr);

signals:
    void signalCheckLayerName(const QString& layerName);

    void signalAddOrEditLayer(const QString& layerName,
                              const int x,
                              const int y,
                              const int width,
                              const int height,
                              const QColor& colour);

public slots:
    void slotCheckLayerName(bool layerNameOk);

private:
    std::unique_ptr<Ui::AddLayerWindow> ui;
};

