#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QStandardItemModel>
#include <QMap>
#include <QList>

#include "xlogic.h"
#include "defines.h"

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
    void initGraphView();
    void initSignal();
    void initControl();
    void initTableView();

    void onClickPic(int x, int y);
    void showAngleTable(QList<AngleItem> &data);

private:
    // 设置断开时候的控件状态
    void setDisConnectControlState();
    // 设置连接时候的控件状态
    void setConnectControlState();

private slots:
    void on_x_connect(bool ret);
    void on_x_disconnect(bool ret);
    void on_x_recieved_serial(const QString &serial);
    void on_x_recieved_camerashot(QByteArray &data);
    void on_x_plate_check_result(int plateCount, const QString &plateResult);

    void on_pushButton_srvConnect_clicked();

    void on_pushButton_srvDisConnect_clicked();

    void on_pushButton_baseAngelRun_clicked();

    void on_pushButton_neckAngleRun_clicked();

    void on_doubleSpinBox_baseAngle_editingFinished();

    void on_doubleSpinBox_neckAngle_editingFinished();

    void on_pushButton_singleScreenShot_clicked();

    void on_pushButton_continuousScreenShot_clicked();

    void on_pushButton_plateCheck_clicked();

    void on_pushButton_saveCurAngle_clicked();

    void on_pushButton_saveCurDevice_clicked();

public slots:
     bool eventFilter(QObject *,QEvent *);

private:

    Ui::MainWindow *ui;
    XLogic *m_pLogic;

    QGraphicsScene m_scene;
    int m_ilastCropX;
    int m_ilastCropY;

    QStandardItemModel m_tableModel;

    bool m_bShotContinuous;

    QMap< QString, QList<AngleItem> > m_angelData;

};

#endif // MAINWINDOW_H
