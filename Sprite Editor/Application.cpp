#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include <QIcon>

#include "Application.h"
#include "AboutWindow.h"
#include "Build.h"

using namespace PandaSpriteEditor;

Application::Application(int argc, char* argv[]) :
    m_application(std::make_unique<QApplication>(argc, argv)),
    m_applicationWindow(std::make_unique<MainWindow>(APP_NAME))
{

}

void Application::showWindow()
{
    //m_applicationWindow->show();
    //auto aw = new AboutWindow;
    //aw->show();
}

void Application::setIcon()
{
    Q_ASSERT(m_application);
    m_application->setWindowIcon(QIcon(":/images/Images/app_icon16x16.png"));
}

void Application::setTheme()
{
    // Set Fusion style (modern style) for a consistent look on different platforms
    qApp->setStyle(QStyleFactory::create("Fusion"));

    // Fusion palette customization
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);

    // Set the dark style for Fusion
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

void Application::initialise()
{
    setTheme();
    setIcon();
    showWindow();
}

int Application::run()
{
    return m_application->exec();
}
