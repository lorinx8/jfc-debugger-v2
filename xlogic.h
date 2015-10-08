#ifndef XLOGIC_H
#define XLOGIC_H

#include <QByteArray>

#include "xnetsock.h"

class XLogic : public QObject
{
    Q_OBJECT

public:
    XLogic();

public:
    // 供网络层的回调
    void OnConnected(bool ret);
    void OnDisConnected(bool ret);
    void OnMessage(quint16 cmd, QByteArray &payload);
    // 供网络层的回调结束

public:
    // 界面调用
    // 连接与断开服务器
    void ConnectHost(const QString &ip, quint16 port);
    void ConnectHost();
    void DisConnectHost();
    // 角度
    void BaseAngleRunTo(double angle);
    void NeckAngleRunTo(double angle);
    // 截屏
    void screenShot();
    // 车牌识别
    void plateCheck(int cropX, int cropY, int width, int weight);

    // 两个角度值只接受某个数的整数倍, 此函数将输入的double值调整为最接近baseValue倍数的doubule值
    double adjustAngleValue(double inputValue, float baseValue);

signals:

    // 服务器连接成功后发出的信号
    void s_connected(bool ret);
    // 与服务器成功断开后发出的信号
    void s_disconnected(bool ret);
    // 连接到服务器后发出的信号
    void s_serial_recieved(const QString &serial);
    // 收到摄像头的图片后发出的信号
    void s_camerashot_recieved(QByteArray &data);
    // 接收到车牌识别结果后, 发出的信号
    void s_plate_check_result(int plateCount, const QString &plateResult);

private:
    void OnRecievedSerial(QByteArray &payload);
    void OnRecievedCameraShot(QByteArray &payload);
    void OnRecievedPlateCheckResult(QByteArray &payload);

public:
    void setCurIp(const QString & ip);
    void setCurPort(quint16 port);

private:
    XNetSock *m_pXNetSock;

    // 当前的IP
    QString m_curIp;
    // 当前的端口
    quint16 m_curPort;
    // 当前序列号
    QString m_curSerial;

};

#endif // XLOGIC_H
