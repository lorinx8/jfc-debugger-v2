#ifndef NOBTNDIALOG_H
#define NOBTNDIALOG_H

#include <QDialog>

namespace Ui {
class NoBtnDialog;
}

class NoBtnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoBtnDialog(QWidget *parent = 0);
    ~NoBtnDialog();

private:
    Ui::NoBtnDialog *ui;
};

#endif // NOBTNDIALOG_H
