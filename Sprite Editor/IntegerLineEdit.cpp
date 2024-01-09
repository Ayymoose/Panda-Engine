#include "IntegerLineEdit.h"

using namespace PandaSpriteEditor;

IntegerLineEdit::IntegerLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    connect(this, &QLineEdit::editingFinished, this, &IntegerLineEdit::onEditingFinished);
}

void IntegerLineEdit::setMinMax(const int min, const int max)
{
    m_min = min;
    m_max = max;
}

void IntegerLineEdit::onEditingFinished()
{
    assert(m_min <= m_max);

    bool conversionOk = false;
    int intValue = text().toInt(&conversionOk);
    if (!conversionOk)
    {
        intValue = m_min;
    }

    const auto clampedValue = std::clamp(intValue, m_min, m_max);

    // Set the adjusted value back to the line edit
    setText(QString::number(clampedValue));
}
