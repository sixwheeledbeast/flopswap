#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QProcess>

namespace Ui {
    class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

   bool startupCheck();

private slots:

    void on_checkBoxEventd_clicked();

    void on_PercentspinBox_valueChanged(int arg1);

    void on_pushButtonSave_clicked();

    void on_pushButtonDefault_clicked();

    void on_checkBoxCheck_toggled(bool checked);

private:
    bool startupcheck;
    Ui::Settings *ui;
    QProcess *m_process;
};

#endif // SETTINGS_H
