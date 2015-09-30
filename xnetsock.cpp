#include <QtNetwork/QHostAddress>
#include <QAbstractSocket>
#include "xnetsock.h"
#include "xparser.h"
#include "xlogic.h"

XNetSock::XNetSock(XLogic *pLogic)
{
    m_pSock = new QTcpSocket();
    m_pLogic = pLogic;
}

void XNetSock::Connect(const QString &ip, quint16 port)
{
    m_pSock->connectToHost(QHostAddress(ip), port);
    bool ret = m_pSock->waitForConnected();
    m_pLogic->OnConnected(ret);

    if (ret != true)
    {
        return;
    }

    ret = m_pSock->waitForReadyRead();
    if (ret == true)
    {
        QByteArray data = m_pSock->readAll();
        OnRead(data);
    }
    // 连接的时候没有拿到序列号等信息，出错，并断开
    else
    {
        DisConnect();
    }
}

void XNetSock::DisConnect()
{
    QAbstractSocket::SocketState state = m_pSock->state();
    if (state == QAbstractSocket::ConnectedState)
    {
        m_pSock->disconnectFromHost();
    }
    m_pLogic->OnDisConnected(true);
}

void XNetSock::OnRead(QByteArray data)
{
    quint16 cmd = 0;
    QByteArray payload;
    XParser::doParse(data, &cmd, payload);
    if (m_pLogic != NULL)
    {
        m_pLogic->OnMessage(cmd, payload);
    }
}
