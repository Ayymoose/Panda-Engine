#pragma once

#include "MainWindow.h"

#include <QApplication>
#include <memory>

namespace PandaSpriteEditor
{
    class Application : public QObject
    {
        Q_OBJECT
    public:
        Application(int argc, char* argv[]);
        void initialise();
        int run();
    private:
        void setTheme();
        void setIcon();
        void showWindow();

        std::unique_ptr<QApplication> m_application;
        std::unique_ptr<MainWindow> m_applicationWindow;
    };
};
