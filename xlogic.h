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
    void OnMessage(quint16 cmd, QByteArray payload);
    // 供网络层的回调结束

public:
    // 界面调用
    void ConnectHost(const QString &ip, quint16 port);
    void ConnectHost();
    void DisConnectHost();
    void BaseAngleRunTo(float angle);
    void NeckAngleRunTo(float angle);
signals:

    // 服务器连接成功后发出的信号
    void s_connected(bool ret);
    // 与服务器成功断开后发出的信号
    void s_disconnected(bool ret);
    // 连接到服务器后发出的信号
    void s_serial_recieved(const QString &serial);

private:
    void OnRecievedSerial(QByteArray payload);

public:
    void setCurIp(const QString & ip);
    void setCurPort(quint16 port);

private:
    XNetSock *m_pXNetSock;

    // 当前的IP
    QString m_curIp;
    // 当前的端口
    quint16 m_curPort;

};

#endif // XLOGIC_H