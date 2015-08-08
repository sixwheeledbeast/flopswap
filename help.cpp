#include "help.h"
#include "ui_help.h"

#include <QtCore/QCoreApplication>
#include <QDesktopServices>
#include <QUrl>


Help::Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);

    //Set Button Text
    ui->pushButtonTalk->setText(tr("[Announce] talk.maemo.org"));
    ui->pushButtonWiki->setText(tr("Wiki"));
    ui->pushButtonGf->setText(tr("Icons by Gentleface"));

}

Help::~Help()
{
    delete ui;
}


void Help::on_pushButtonTalk_clicked()
{
     QDesktopServices::openUrl(QUrl("http://talk.maemo.org/showthread.php?t=87985"));
}


void Help::on_pushButtonWiki_clicked()
{
     QDesktopServices::openUrl(QUrl("http://wiki.maemo.org/Flopswap"));
}


void Help::on_pushButtonGf_clicked()
{
     QDesktopServices::openUrl(QUrl("http://www.gentleface.com"));
}
