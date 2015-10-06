#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "xlogic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // 设置断开时候的控件状态
    void setDisConnectControlState();
    // 设置连接时候的控件状态
    void setConnectControlState();

private slots:
    void on_x_connect(bool ret);
    void on_x_disconnect(bool ret);
    void on_x_recieved_serial(const QString &serial);

    void on_pushButton_srvConnect_clicked();

    void on_pushButton_srvDisConnect_clicked();

    void on_pushButton_baseAngelRun_clicked();

    void on_pushButton_neckAngleRun_clicked();

    void on_doubleSpinBox_baseAngle_editingFinished();

    void on_doubleSpinBox_neckAngle_editingFinished();

    void on_pushButton_singleScreenShot_clicked();

private:

    Ui::MainWindow *ui;
    XLogic *m_pLogic;




};

#endif // MAINWINDOW_H
