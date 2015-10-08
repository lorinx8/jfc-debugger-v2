#ifndef XNETSOCK_H
#define XNETSOCK_H

#include <QtNetwork>
#include <QByteArray>


class XLogic;

class XNetSock : public QObject
{
    Q_OBJECT

public:

    XNetSock(XLogic *pLogic);

    void Connect(const QString &ip, quint16 port);

    void DisConnect();

    void WriteData(char cmd1, char cmd2, QByteArray payload);

private:

    void DataHandle(QByteArray &data);

private slots:

    void OnRead();

private:
    QString m_ip;
    quint16 m_port;
    QTcpSocket *m_pSock;
    XLogic *m_pLogic;

    QByteArray m_fullData;
};

#endif // XNETSOCK_H
