#pragma once

#include <QLineEdit>

#include <memory>

#include "MinMax.h"

namespace PandaSpriteEditor
{
    class IntegerLineEdit : public QLineEdit
    {
        Q_OBJECT

    public:
        IntegerLineEdit(QWidget *parent = nullptr);

        void setMinMax(const int min, const int max);

    private:
        void onEditingFinished();

        int m_min{-1};
        int m_max{-1};

    };
}
