#pragma once

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class TransparencyColour;
}
QT_END_NAMESPACE

class TransparencyColour : public QMainWindow
{
    Q_OBJECT

public:
    TransparencyColour(QWidget *parent = nullptr);
    ~TransparencyColour();

private:
    std::unique_ptr<Ui::TransparencyColour> m_ui;
};
