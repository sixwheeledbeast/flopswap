#ifndef HELP_H
#define HELP_H

#include <QDialog>

namespace Ui {
    class Help;
}

class Help : public QDialog
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = 0);
    ~Help();

private slots:
    void on_pushButtonTalk_clicked();

    void on_pushButtonWiki_clicked();

    void on_pushButtonGf_clicked();

private:
    Ui::Help *ui;
};

#endif // HELP_H
