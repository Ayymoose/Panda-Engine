#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(const QString& title, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(std::make_unique<Ui::MainWindow>())
{
    m_ui->setupUi(this);
    setWindowTitle(title);
}

MainWindow::~MainWindow() = default;
