#ifndef XNETSOCK_H
#define XNETSOCK_H

#include <QtNetwork>
#include <QByteArray>


class XLogic;

class XNetSock
{
public:

    XNetSock(XLogic *pLogic);

    void Connect(const QString &ip, quint16 port);

    void DisConnect();

    void OnRead(QByteArray data);

private:

    QString m_ip;
    quint16 m_port;
    QTcpSocket *m_pSock;
    XLogic *m_pLogic;
};

#endif // XNETSOCK_H
