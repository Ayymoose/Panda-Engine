#include "NewLayer.h"
#include "./ui_NewLayer.h"

NewLayer::NewLayer(QWidget *parent)
    : QDialog(parent)
    , m_ui(std::make_unique<Ui::NewLayer>())
{
    m_ui->setupUi(this);
}

NewLayer::~NewLayer() = default;
