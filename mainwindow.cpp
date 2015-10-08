#include <QMessageBox>
#include <QGraphicsScene>
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QDir>
#include <QImage>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_ilastCropX = 0;
    m_ilastCropY = 0;
    m_bShotContinuous = false;
    m_pLogic = new XLogic();

    initControl();
    initSignal();
    initGraphView();
    initTableView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGraphView()
{
    QDir dir;
    QString path = dir.currentPath() + "/logo.jpg";
    QImage orgImage;
    QImage scaledImage;
    orgImage.load(path);
    scaledImage = orgImage.scaled(640, 360);
    qDebug() << "load image file" << path;
    ui->graphicsView->installEventFilter(this);
    m_scene.addPixmap(QPixmap::fromImage(scaledImage));
    ui->graphicsView->setScene(&m_scene);
    ui->graphicsView->show();
}

void MainWindow::initSignal()
{
    // signal
    // 连接
    connect(m_pLogic, SIGNAL(s_connected(bool)), this, SLOT(on_x_connect(bool)));
    // 断开
    connect(m_pLogic, SIGNAL(s_disconnected(bool)), this, SLOT(on_x_disconnect(bool)));
    // 得到序列号
    connect(m_pLogic, SIGNAL(s_serial_recieved(const QString &)), this, SLOT(on_x_recieved_serial(const QString &)));
    // 收到摄像头图片
    connect(m_pLogic, SIGNAL(s_camerashot_recieved(QByteArray&)), this, SLOT(on_x_recieved_camerashot(QByteArray&)));
}

void MainWindow::initControl()
{
    // 控件初始化
    ui->pushButton_baseAngelRun->setEnabled(true);
    ui->pushButton_neckAngleRun->setEnabled(true);
    ui->pushButton_srvConnect->setEnabled(true);
    ui->pushButton_srvDisConnect->setEnabled(false);
}

void MainWindow::initTableView()
{
    m_tableModel.setColumnCount(7);

    m_tableModel.setHeaderData(0, Qt::Horizontal, "序号");
    m_tableModel.setHeaderData(1, Qt::Horizontal, "B角度");
    m_tableModel.setHeaderData(2, Qt::Horizontal, "N角度");

    m_tableModel.setHeaderData(3, Qt::Horizontal, "剪裁X");
    m_tableModel.setHeaderData(4, Qt::Horizontal, "剪裁Y");
    m_tableModel.setHeaderData(5, Qt::Horizontal, "剪裁宽");
    m_tableModel.setHeaderData(6, Qt::Horizontal, "剪裁高");
    ui->tableView->setModel(&m_tableModel);

    //设置列宽不可变
    for (int i = 0; i < 7; ++i)
    {
        ui->tableView->setColumnWidth(i, 80);
    }
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

    if (m_bShotContinuous == true)
    {
        m_pLogic->screenShot();
    }
}

void MainWindow::on_x_plate_check_result(int plateCount, const QString &plateResult)
{
    // 车牌识别结果
    if (plateCount == 0)
    {
        ui->label_plateResult->setText("未识别到车牌");
    }
    else
    {
        ui->label_plateResult->setText(plateResult);
    }
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
    if (m_bShotContinuous == true)
    {
        return;
    }
    m_pLogic->screenShot();
}

// 单击连续截屏
void MainWindow::on_pushButton_continuousScreenShot_clicked()
{
    // 连续截屏的方式为, 上一次截图完成, 显示完图片后, 则立刻开始下一次截屏
    if (m_bShotContinuous == true)
    {
        m_bShotContinuous = false;
        ui->pushButton_continuousScreenShot->setText("连续截图");
    }
    else
    {
        m_bShotContinuous = true;
        ui->pushButton_continuousScreenShot->setText("停止截图");
        m_pLogic->screenShot();
    }
}

// 单击车牌识别
void MainWindow::on_pushButton_plateCheck_clicked()
{
    int cropX = ui->lineEdit_cropX->text().toInt();
    int cropY = ui->lineEdit_cropY->text().toInt();
    int cropWidth = ui->lineEdit_cropWidth->text().toInt();
    int cropWeight = ui->lineEdit_cropWeight->text().toInt();
    m_pLogic->plateCheck(cropX, cropY, cropWidth, cropWeight);
}

// 单击写入当前设备某角度值
void MainWindow::on_pushButton_saveCurAngle_clicked()
{
    // 得到两个角度值
    double baseAngle = ui->doubleSpinBox_baseAngle->text().toDouble();
    double neckAngle = ui->doubleSpinBox_neckAngle->text().toDouble();

    // 得到截屏数据
    int cropX = ui->lineEdit_cropX->text().toInt();
    int cropY = ui->lineEdit_cropY->text().toInt();
    int cropWidth = ui->lineEdit_cropWidth->text().toInt();
    int cropWeight = ui->lineEdit_cropWeight->text().toInt();

    AngleItem item(baseAngle, neckAngle, cropX, cropY, cropWidth, cropWeight);

    if (m_angelData.contains(m_pLogic->m_curSerial))
    {
        QList<AngleItem> ls = m_angelData.value(m_pLogic->m_curSerial);
        ls.append(item);
    }
    else
    {
        QList<AngleItem> ls;
        ls.append(item);
        m_angelData.insert(m_pLogic->m_curSerial, ls);
    }
}

// 单击写入当前设备值到文件
void MainWindow::on_pushButton_saveCurDevice_clicked()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 主要用于监视图片空间是鼠标移动等事件
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 图片控件
    if (obj == ui->graphicsView)
    {
        // qDebug() << event->type();
        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent* e = (QMouseEvent*)event;
            QString xstr;
            QString ystr;
            xstr = QString("%1").arg(e->x());
            ystr = QString("%1").arg(e->y());
            // qDebug() << xstr << ystr;
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

            if (curX >= m_ilastCropX && curY >= m_ilastCropY)
            {
                int width = curX - m_ilastCropX;
                int weight = curY - m_ilastCropY;
                ui->lineEdit_cropWidth->setText(QString("%1").arg(width * 2));
                ui->lineEdit_cropWeight->setText(QString("%1").arg(weight * 2));
            }
            m_ilastCropX = curX;
            m_ilastCropY = curY;
        }
    }
    return QWidget::eventFilter(obj, event);
}

