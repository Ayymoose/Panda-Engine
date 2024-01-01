#include "TransparencyColour.h"
#include "./ui_TransparencyColour.h"

TransparencyColour::TransparencyColour(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(std::make_unique<Ui::TransparencyColour>())
{
    m_ui->setupUi(this);
}

TransparencyColour::~TransparencyColour() = default;
