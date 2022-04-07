#pragma once

#include <memory>

// generated includes
#include "ui_AboutWindow.h"

namespace Ui
{
    class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);

private slots:
    void slotCopyToClipboardClicked();

private:
    std::unique_ptr<Ui::AboutWindow> ui;
};

