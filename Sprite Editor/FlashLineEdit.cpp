#include "FlashLineEdit.h"
#include <QStyle>

using namespace PandaSpriteEditor;

FlashLineEdit::FlashLineEdit(QLineEdit& widget, const int intervalInMilliseconds, int count)
    : m_widget(widget),
      m_intervalInMilliseconds(intervalInMilliseconds),
      m_count(count),
      m_flashCount(0)
{
    m_flashTimer.setInterval(intervalInMilliseconds);
    m_flashTimer.setSingleShot(false);
    connect(&m_flashTimer, &QTimer::timeout, this, &FlashLineEdit::flash);
    m_originalStylesheet = m_widget.styleSheet();
}

void FlashLineEdit::start()
{
    m_flashTimer.start();
}

void FlashLineEdit::flash()
{
    qDebug() << "QLineEdit Stylesheet: " << m_widget.styleSheet();

    if (m_flashCount < m_count * 2)
    {
        if (m_flashCount % 2 == 0)
        {
            m_widget.setStyleSheet(m_flashStylesheet);
        }
        else
        {
            m_widget.setStyleSheet(m_originalStylesheet);
        }
        ++m_flashCount;
    }
    else
    {
        m_flashTimer.stop();
        m_flashCount = 0;
    }
}
