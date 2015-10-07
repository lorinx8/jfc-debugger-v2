#include <QMessageBox>
#include <QGraphicsScene>
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_lastCropX = 0;
    m_lastCropY = 0;

    ui->setupUi(this);
    m_pLogic = new XLogic();

    // 控件初始化
    ui->pushButton_baseAngelRun->setEnabled(true);
    ui->pushButton_neckAngleRun->setEnabled(true);
    ui->pushButton_srvConnect->setEnabled(true);
    ui->pushButton_srvDisConnect->setEnabled(false);

    // signal
    // 连接
    connect(m_pLogic, SIGNAL(s_connected(bool)), this, SLOT(on_x_connect(bool)));
    // 断开
    connect(m_pLogic, SIGNAL(s_disconnected(bool)), this, SLOT(on_x_disconnect(bool)));
    // 得到序列号
    connect(m_pLogic, SIGNAL(s_serial_recieved(const QString &)), this, SLOT(on_x_recieved_serial(const QString &)));
    // 收到摄像头图片
    connect(m_pLogic, SIGNAL(s_camerashot_recieved(QByteArray&)), this, SLOT(on_x_recieved_camerashot(QByteArray&)));

    ui->graphicsView->installEventFilter(this);
    ui->graphicsView->setScene(&m_scene);
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
 * \brief 接收到摄像头图片
 * \param data
 */
void MainWindow::on_x_recieved_camerashot(QByteArray &data)
{
    QPixmap pixmap;
    pixmap.loadFromData(data);
    m_scene.addPixmap(pixmap);
    ui->graphicsView->show();
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
    on_doubleSpinBox_baseAngle_editingFinished();
    double value = ui->doubleSpinBox_baseAngle->text().toDouble();
    m_pLogic->BaseAngleRunTo(value);
}

/*!
 * \brief 点击neck角度运行到
 */
void MainWindow::on_pushButton_neckAngleRun_clicked()
{
    on_doubleSpinBox_neckAngle_editingFinished();
    double value = ui->doubleSpinBox_neckAngle->text().toDouble();
    m_pLogic->NeckAngleRunTo(value);
}

// 这个必须是0.375的倍数，若被编辑后，应修改为最接近0.375的倍数值
void MainWindow::on_doubleSpinBox_baseAngle_editingFinished()
{
    double value = ui->doubleSpinBox_baseAngle->text().toDouble();
    double step = ui->doubleSpinBox_baseAngle->singleStep();
    double v = m_pLogic->adjustAngleValue(value, step);
    ui->doubleSpinBox_baseAngle->setValue(v);
}

// 这个必须是0.9的倍数，若被编辑后，应修改为最接近0.9的倍数值
void MainWindow::on_doubleSpinBox_neckAngle_editingFinished()
{
    double value = ui->doubleSpinBox_neckAngle->text().toDouble();
    double step = ui->doubleSpinBox_neckAngle->singleStep();
    double v = m_pLogic->adjustAngleValue(value, step);
    ui->doubleSpinBox_neckAngle->setValue(v);
}

// 点击单次截屏
void MainWindow::on_pushButton_singleScreenShot_clicked()
{
    m_pLogic->screenShot();
}

// 主要用于监视图片空间是鼠标移动等事件
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 图片控件
    if (obj == ui->graphicsView)
    {
        qDebug() << event->type();
        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent* e = (QMouseEvent*)event;
            QString xstr;
            QString ystr;
            xstr = QString("%1").arg(e->x());
            ystr = QString("%1").arg(e->y());
            qDebug() << xstr << ystr;
            ui->lineEdit_mouseX->setText(xstr);
            ui->lineEdit_mouseY->setText(ystr);
        }
        else if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* e = (QMouseEvent*)event;

            int curX = e->x();
            int curY = e->y();
            ui->lineEdit_cropX->setText(QString("%1").arg(e->x() * 2));
            ui->lineEdit_cropY->setText(QString("%1").arg(e->y() * 2));

            if (curX >= m_lastCropX && curY >= m_lastCropY)
            {
                int width = curX - m_lastCropX;
                int weight = curY - m_lastCropY;
                ui->lineEdit_cropWidth->setText(QString("%1").arg(width * 2));
                ui->lineEdit_cropWeight->setText(QString("%1").arg(weight * 2));
            }
            m_lastCropX = curX;
            m_lastCropY = curY;
        }
    }
    return QWidget::eventFilter(obj, event);
}
