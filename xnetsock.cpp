#include <QtNetwork/QHostAddress>
#include <QAbstractSocket>
#include "xnetsock.h"
#include "xparser.h"
#include "xlogic.h"

XNetSock::XNetSock(XLogic *pLogic)
{
    m_pSock = new QTcpSocket();
    m_pLogic = pLogic;
    m_fullData.clear();
}

void XNetSock::Connect(const QString &ip, quint16 port)
{
    m_pSock->connectToHost(QHostAddress(ip), port);
    bool ret = m_pSock->waitForConnected();
    m_pLogic->OnConnected(ret);
    if (ret != true)
    {
        QString error = m_pSock->errorString();
        qDebug() << error;
        return;
    }

    ret = m_pSock->waitForReadyRead();
    if (ret == true)
    {
        OnRead();
        // 连接readReady信号
        connect(m_pSock, SIGNAL(readyRead()), this, SLOT(OnRead()));
    }
    // 连接的时候没有拿到序列号等信息，出错，并断开
    else
    {
        DisConnect();
    }
}

void XNetSock::DisConnect()
{
    disconnect(m_pSock, SIGNAL(readyRead()), this, SLOT(OnRead()));
    QAbstractSocket::SocketState state = m_pSock->state();
    if (state == QAbstractSocket::ConnectedState)
    {
        m_pSock->disconnectFromHost();
    }
    m_pLogic->OnDisConnected(true);
}

void XNetSock::WriteData(char cmd1, char cmd2, QByteArray payload)
{
    QByteArray pack = XParser::packPayload(cmd1, cmd2, payload);
    QByteArray temp = pack.toHex().toUpper();
    qDebug() << temp;
    int ret = m_pSock->write(pack);
    if (ret == -1)
    {
        qDebug() << m_pSock->errorString();
    }
}

void XNetSock::DataHandle(QByteArray &data)
{
    quint16 cmd = 0;
    QByteArray payload;
    XParser::doParse(data, &cmd, payload);
    if (m_pLogic != NULL)
    {
        m_pLogic->OnMessage(cmd, payload);
    }
}

void XNetSock::OnRead()
{
    QByteArray data = m_pSock->readAll();

    // 在此处需要进行连接包的处理,以便传递给DataHandle(data)函数的都是一个完整的数据包
    // header 0x7F 0x55 2bytes   0 1
    // command 2bytes            2 3
    // payload length 4bytes     4 5 6 7
    // payload
    // cs 1byte
    // end 0xBE 0xEF 2bytes

    // 如果是一个完整的包, 最少的字节长度
    int const minSize = 11;

    int n = data.size();

    char h1 = 0x7F;
    char h2 = 0x55;
    char e1 = 0xBE;
    char e2 = 0xEF;
    // 是完整的一个包
    if (n >= minSize && data.at(0) == h1 && data.at(1) == h2 && data.at(n-2) == e1 && data.at(n-2) == e2)
    {
        DataHandle(data);
        return;
    }

    // 如果不是完整包
    // 1 如果是某个数据包的第一部分数据
    if (n >=2 && data.at(0) == h1 && data.at(1) == h2)
    {
        m_fullData.clear();
        m_fullData.append(data);
    }
    // 2. 如果是结尾包(注意, 由于实际使用的场景, 不存在上一个数据包的最后一部分和新数据包的第一部分相连的情况, 所以就简单处理了)
    else if (n >=2 && data.at(n-2) == e1 && data.at(n-2) == e2)
    {
        m_fullData.append(data);
        DataHandle(m_fullData);
        m_fullData.clear();
    }
    // 如果是中间部分的包, 就直接添加进去
    else
    {
        m_fullData.append(data);
    }
}
