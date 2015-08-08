#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QDialog>

namespace Ui {
    class Instructions;
}

class Instructions : public QDialog
{
    Q_OBJECT

public:
    explicit Instructions(QWidget *parent = 0);
    ~Instructions();

private:
    Ui::Instructions *ui;
};

#endif // INSTRUCTIONS_H
