#include "settings.h"
#include "ui_settings.h"

#include <QProcess>
#include <QtCore/QCoreApplication>
#include <QFileInfo>
#include <QSettings>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    startupcheck(false),
    ui(new Ui::Settings),

    m_process(new QProcess(this))

{
    ui->setupUi(this);

    QSettings settings("flopswap", "flopswap");

   //Set Text
    ui->checkBoxEventd->setText(tr("Use External Swap on boot"));
    ui->checkBoxCheck->setText(tr("Check Usage on Start"));
    ui->label_2->setText(tr("Threshold for Swap Used Warning"));
    ui->pushButtonSave->setText(tr("Save"));
    ui->pushButtonDefault->setText(tr("Default"));

   //Check for existance of /etc/event.d/relocateswap to draw checkbox correctly
    if ( QFileInfo("/etc/event.d/relocateswap").exists() ) ui->checkBoxEventd->setChecked(true);

   //Initial QSettings for Checkbox
    if (settings.contains("StartupCheck"))
    {
      startupcheck = settings.value("StartupCheck").toBool();
    }
    ui->checkBoxCheck->setChecked(startupcheck);

   //Initial QSettings for Spin Box
    if (settings.contains("PercentForDialog"))
    {
      int psb = settings.value("PercentForDialog").toInt();
      ui->PercentspinBox->setValue(psb);
    }
    else
    {
      settings.setValue("PercentForDialog", 94);
      settings.value("PercentForDialog").toInt();
    }

}

Settings::~Settings()
{
      //Save Startup Checkbox on destruction
      QSettings settings("flopswap", "flopswap");
      settings.setValue("StartupCheck", startupcheck);

      delete ui;
}

void Settings::on_pushButtonSave_clicked()
{
    QSettings settings("flopswap", "flopswap");
    settings.setValue("PercentForDialog", ui->PercentspinBox->value());
}

void Settings::on_checkBoxEventd_clicked()
{
    if (ui->checkBoxEventd->isChecked())
    {
        m_process->start("sudo /opt/flopswap/script/setupstart.sh");
    }
    else
    {
        m_process->start("sudo /opt/flopswap/script/rmupstart.sh");
    }
}

//Bool Startup Check
bool Settings::startupCheck()
{
  return startupcheck;
}
//Slot Startup Check
void Settings::on_checkBoxCheck_toggled(bool checked)
{
  startupcheck = checked;
}

void Settings::on_PercentspinBox_valueChanged(int psb)
{
    ui->PercentspinBox->setValue(psb);
}

void Settings::on_pushButtonDefault_clicked()
{
    QSettings settings("flopswap", "flopswap");
    settings.setValue("PercentForDialog", 94);
    int psb = settings.value("PercentForDialog").toInt();
    ui->PercentspinBox->setValue(psb);
}
