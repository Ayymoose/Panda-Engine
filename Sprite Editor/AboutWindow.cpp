#include "AboutWindow.h"
#include "Build.h"

#include <QGuiApplication>
#include <QClipboard>

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::AboutWindow>())
{
    ui->setupUi(this);
    setWindowTitle("About");

    ui->_appVersionLabel->setText(APP_VERSION);
    ui->_buildDateLabel->setText(BUILD_DATE);
    ui->_buildCommitLabel->setText(BUILD_COMMIT);
    ui->_qtVersionLabel->setText(QT_APP_VERSION);
    ui->_compilerVersionLabel->setText(COMPILER_VERSION);
    ui->_buildTypeLabel->setText(BUILD_TYPE " " BUILD_ARCH);

    connect(ui->copyToClipboard, &QAbstractButton::clicked, this, &AboutWindow::slotCopyToClipboardClicked);
}

void AboutWindow::slotCopyToClipboardClicked()
{
    QString details;

    details += ui->_appVersionNameLabel->text() + ' ' + ui->_appVersionLabel->text() + '\n';
    details += ui->_buildDateNameLabel->text() + ' ' + ui->_buildDateLabel->text() + '\n';
    details += ui->_buildCommitNameLabel->text() + ' ' + ui->_buildCommitLabel->text() + '\n';
    details += ui->_buildTypeNameLabel->text() + ' ' + ui->_buildTypeLabel->text() + '\n';
    details += ui->_qtVersionNameLabel->text() + ' ' + ui->_qtVersionLabel->text() + '\n';
    details += ui->_compilerVersionNameLabel->text() + ' ' + ui->_compilerVersionLabel->text() + '\n';

    QGuiApplication::clipboard()->setText(details);
    accept();
}

