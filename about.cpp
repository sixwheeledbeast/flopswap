#include "about.h"
#include "ui_about.h"

#include <QtCore/QCoreApplication>
#include <QDesktopServices>
#include <QUrl>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    ui->icon->setPixmap(QPixmap("/usr/share/icons/hicolor/64x64/apps/flopswap64.png"));
    QFont f = ui->text1->font();
    f.setPointSize( f.pointSize() + 5 );
    f.setBold(true);
    QFont g = ui->text1->font();
    g.setPointSize( g.pointSize() - 1);
    QFont h = ui->text1->font();
    h.setPointSize( h.pointSize() - 3);

    //Set Font Sizes
    ui->flopswap->setFont(f);
    ui->text2->setFont(g);
    ui->text3->setFont(h);

    //Set Button Text
    ui->flopswap->setText("flopswap 0.2.4");
    ui->text1->setText(tr("Linux Swap Management GUI"));
    ui->text2->setText("(C)2012-2015 sixwheeledbeast");
    ui->text3->setText(tr("Icons provided under Creative Commons license by Gentleface"));

}

About::~About()
{
    delete ui;
}
