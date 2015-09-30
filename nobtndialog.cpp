#include "nobtndialog.h"
#include "ui_nobtndialog.h"

NoBtnDialog::NoBtnDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoBtnDialog)
{
    ui->setupUi(this);
}

NoBtnDialog::~NoBtnDialog()
{
    delete ui;
}
