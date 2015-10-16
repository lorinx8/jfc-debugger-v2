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

void XLogic::OnMessage(quint16 cmd, QByteArray &payload)
{
    switch(cmd)
    {
    // 连接后拿到序列号
    case 0x4000:
    {
        OnRecievedSerial(payload);
        break;
    }
    // 接受到图片以后
    case 0x4001:
    {
        OnRecievedCameraShot(payload);
        break;
    }
    // 接收到车牌返回结果后
    case 0x4002:
    {
        OnRecievedPlateCheckResult(payload);
        break;
    }
    }
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

void XLogic::OnRecievedSerial(QByteArray &payload)
{
    QString serial(payload);
    m_curSerial = serial;
    emit s_serial_recieved(serial);
    return;
}

void XLogic::OnRecievedCameraShot(QByteArray &payload)
{
    // 接收到的结果是 序列号(12字节) + 两个角度值(7+7) + 然后是图片
    // 发送给界面的删掉前面的字节
    payload.remove(0, 26);
    emit s_camerashot_recieved(payload);
    return;
}

void XLogic::OnRecievedPlateCheckResult(QByteArray &payload)
{
    // 接受到的数据是, 设备串号(12)字节, 然后是识别到的车牌个数(1字节), 然后是车牌号码
    int num = (int)payload.at(13);
    int n = payload.size();
    if (num > 0)
    {
        QString palteStr = QString(payload.right(n - 13));
        emit s_plate_check_result(num, palteStr);
    }
    else
    {
        emit s_plate_check_result(0, "");
    }
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

// 数据转换， int型转换为4字节，大端字节序
QByteArray XLogic::int32ToBigendianByteArray(int num)
{
    union unn
    {
        int n;
        char c[4];
    };
    unn unnn;
    unnn.n = num;
    QByteArray ret;
    ret.append(unnn.c[3]);
    ret.append(unnn.c[2]);
    ret.append(unnn.c[1]);
    ret.append(unnn.c[0]);
    return ret;
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

void XLogic::plateCheck(int cropX, int cropY, int width, int weight)
{
    // 通信类型 0003
    // 设备串号 X	 Y W H
    // 12      4 4 4 4
    QByteArray payload;
    payload.append(m_curSerial);
    payload.append(int32ToBigendianByteArray(cropX));
    payload.append(int32ToBigendianByteArray(cropY));
    payload.append(int32ToBigendianByteArray(width));
    payload.append(int32ToBigendianByteArray(weight));

    m_pXNetSock->WriteData(0x00, 0x03, payload);
}

double XLogic::adjustAngleValue(double inputValue, float baseValue)
{
    int c = (int)(inputValue / baseValue);
    double v = c * baseValue;
    return v;
}



