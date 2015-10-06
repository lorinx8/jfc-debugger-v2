#include "xlogic.h"

XLogic::XLogic()
{
    m_pXNetSock = new XNetSock(this);
    m_curIp = "192.168.10.1";
    m_curPort = 8000;
    m_curSerial = "sz1234567890";
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
    m_curSerial = serial;
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

void XLogic::BaseAngleRunTo(double angle)
{
    // 将angle转换成7字节的字符串, 小数点前后各三位,加上小数点共7位
    QString str = QString("%1").arg((double)angle, 7, 'f', 3, '0');

    // 开始组合数据负载 12字节序列号 + 7字节角度
    QByteArray payload;
    payload.append(m_curSerial).append(str);
    m_pXNetSock->WriteData(0x00, 0x00, payload);
}

void XLogic::NeckAngleRunTo(double angle)
{
    QString str = QString("%1").arg((double)angle, 7, 'f', 3, '0');
    QByteArray payload;
    payload.append(m_curSerial).append(str);
    m_pXNetSock->WriteData(0x00, 0x01, payload);
}

void XLogic::screenShot()
{
    // 截屏仅发送设备串号, 通信类型为0002
    QByteArray payload;
    payload.append(m_curSerial);
    m_pXNetSock->WriteData(0x00, 0x02, payload);
}

double XLogic::adjustAngleValue(double inputValue, float baseValue)
{
    int c = (int)(inputValue / baseValue);
    double v = c * baseValue;
    return v;
}


