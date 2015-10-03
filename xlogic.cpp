#include "xlogic.h"

XLogic::XLogic()
{
    m_pXNetSock = new XNetSock(this);
    m_curIp = "192.168.10.1";
    m_curPort = 8000;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 供网络层的回调

void XLogic::OnMessage(quint16 cmd, QByteArray payload)
{
    switch(cmd)
    {
    // 连接后拿到序列号
    case 0x4000:
    {
        OnRecievedSerial(payload);
        break;
    }
    }
}

void XLogic::OnRecievedSerial(QByteArray payload)
{
    QString serial(payload);
    emit s_serial_recieved(serial);
    return;
}

void XLogic::OnConnected(bool ret)
{
    emit s_connected(ret);
}

void XLogic::OnDisConnected(bool ret)
{
    emit s_disconnected(ret);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 界面调用

void XLogic::ConnectHost(const QString &ip, quint16 port)
{
    m_pXNetSock->Connect(ip, port);
}

void XLogic::DisConnectHost()
{
    m_pXNetSock->DisConnect();
}

void XLogic::ConnectHost()
{
    m_pXNetSock->Connect(m_curIp, m_curPort);
}


void XLogic::setCurIp(const QString & ip)
{
    m_curIp = ip;
}

void XLogic::setCurPort(quint16 port)
{
    m_curPort = port;
}


void XLogic::BaseAngleRunTo(float angle)
{

}

void XLogic::NeckAngleRunTo(float angle)
{

}


