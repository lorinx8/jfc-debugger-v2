#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pLogic = new XLogic();

    // 控件初始化
    ui->pushButton_baseAngelRun->setEnabled(false);
    ui->pushButton_neckAngleRun->setEnabled(false);
    ui->pushButton_srvConnect->setEnabled(true);
    ui->pushButton_srvDisConnect->setEnabled(false);

    // signal
    // 连接
    connect(m_pLogic, SIGNAL(s_connected(bool)), this, SLOT(on_x_connect(bool)));
    // 断开
    connect(m_pLogic, SIGNAL(s_disconnected(bool)), this, SLOT(on_x_disconnect(bool)));
    // 得到序列号
    connect(m_pLogic, SIGNAL(s_serial_recieved(const QString &)), this, SLOT(on_x_recieved_serial(const QString &)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 设置断开时候的控件状态
void MainWindow::setDisConnectControlState()
{
    ui->lineEdit_srvIp->setEnabled(true);
    ui->lineEdit_srvPort->setEnabled(true);

    ui->pushButton_srvConnect->setEnabled(true);
    ui->pushButton_srvDisConnect->setEnabled(false);

    ui->pushButton_baseAngelRun->setEnabled(false);
    ui->pushButton_neckAngleRun->setEnabled(false);
}

// 设置连接时候的控件状态
void MainWindow::setConnectControlState()
{
    ui->lineEdit_srvIp->setEnabled(false);
    ui->lineEdit_srvPort->setEnabled(false);

    ui->pushButton_srvConnect->setEnabled(false);
    ui->pushButton_srvDisConnect->setEnabled(true);

    ui->pushButton_baseAngelRun->setEnabled(true);
    ui->pushButton_neckAngleRun->setEnabled(true);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief 单击连接后
 */
void MainWindow::on_pushButton_srvConnect_clicked()
{
    setConnectControlState();
    m_pLogic->setCurIp(ui->lineEdit_srvIp->text());
    m_pLogic->setCurPort(ui->lineEdit_srvPort->text().trimmed().toShort());
    m_pLogic->ConnectHost();
}

/*!
 * \brief 单击断开
 */
void MainWindow::on_pushButton_srvDisConnect_clicked()
{
    m_pLogic->DisConnectHost();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 自定义槽

/*!
 * \brief 接受到序列号
 * \param serial
 */
void MainWindow::on_x_recieved_serial(const QString &serial)
{
    ui->label_serial->setText("设备序号: " + serial);
}

/*!
 * \brief 断开状态
 * \param ret
 */
void MainWindow::on_x_disconnect(bool ret)
{
    ui->label_connState->setText("未连接");
    ui->label_serial->setText("设备序号: 000000000000");
    setDisConnectControlState();
}

/*!
 * \brief 连接状态
 * \param ret
 */
void MainWindow::on_x_connect(bool ret)
{
    if (ret == true)
    {
        ui->label_connState->setText("已连接");
        ui->label_serial->setText("请等待设备序号返回");
    }
    else
    {
        ui->label_connState->setText("未连接");
        QMessageBox::warning(0, "错误", "连接失败，请检查网络", 0, 0);
        setDisConnectControlState();
    }
}

/*!
 * \brief 点击base角度运行到
 */
void MainWindow::on_pushButton_baseAngelRun_clicked()
{

}


/*!
 * \brief 点击neck角度运行到
 */
void MainWindow::on_pushButton_neckAngleRun_clicked()
{

}
