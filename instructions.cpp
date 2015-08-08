#include "instructions.h"
#include "ui_instructions.h"

Instructions::Instructions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instructions)

{
    ui->setupUi(this);

    QFont f = ui->labelMain->font();
    f.setPointSize( f.pointSize() + 1 );
    f.setBold(true);
    ui->labelHead->setFont(f);
    ui->labelHead->setText("Quick Guide to FlopSwap");
    ui->labelMain->setText(tr("<b>There MUST be two uSD swaps,</b><br>\
                               <b>and one device swap minimum.</b><br>\
                               <b>Fresh Swap</b><br>\
                               Moves Swap to a fresh SD location<br>\
                               <b>Swap to Device</b><br>\
                               To allow back cover removal<br>\
                               <b>Check</b><br>\
                               Shows current swap location(s)<br>\
                               and checks current swap usage<br>"));

}

Instructions::~Instructions()
{
    delete ui;
}
