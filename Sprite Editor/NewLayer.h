#pragma once

#include <QDialog>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class NewLayer;
}
QT_END_NAMESPACE

class NewLayer : public QDialog
{
    Q_OBJECT

public:
    NewLayer(QWidget *parent = nullptr);
    ~NewLayer();

private:
    std::unique_ptr<Ui::NewLayer> m_ui;
};
