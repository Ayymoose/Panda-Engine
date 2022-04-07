#pragma once

#include <QTimer>
#include <QLineEdit>

namespace PandaSpriteEditor
{
    // Can't template QObject class // template <typename WidgetType>
    class FlashLineEdit :  public QObject
    {
        Q_OBJECT
    public:
        FlashLineEdit(QLineEdit& widget, const int intervalInMilliseconds, int count);

        void start();

    private slots:
        void flash();



    private:
        QLineEdit& m_widget;
        QTimer m_flashTimer;
        int m_intervalInMilliseconds;
        int m_count;
        int m_flashCount;
        QString m_originalStylesheet;
        const QString m_flashStylesheet = "QLineEdit { border: 1px solid red; height: 22px; }";
    };
}
