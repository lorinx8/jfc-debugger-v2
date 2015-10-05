#include "xparser.h"

XParser::XParser()
{

}


bool XParser::doParse(QByteArray src, quint16 *cmd, QByteArray &payload)
{
    if (src.data()[0] != 0x7F || src.data()[1] != 0x55)
    {
        return false;
    }
    quint16 type = src[2] << 8 | src[3];
    quint16 c = src[4] << 8 | src[5];
    for (int i = 0; i < c; ++i)
    {
        payload.append(src[6+i]);
    }
    *cmd = type;
    return true;
}

bool XParser::doParseLong(QByteArray src, quint16 *cmd, QByteArray &payload)
{
    if (src.data()[0] != 0x7F || src.data()[1] != 0x55)
    {
        return false;
    }
    quint16 type = src[2] << 8 | src[3];
    quint16 c = src[4] << 8 | src[5];
    for (int i = 0; i < c; ++i)
    {
        payload.append(src[6+i]);
    }
    *cmd = type;
    return true;
}

// 将负载数据打包为可发送的数据包
// header 0x7F 0x55 2bytes
// command 2bytes
// payload length 2bytes
// payload
// cs 1byte
// end 0xBE 0xEF 2bytes
// 除开真正的数据负载,其他数据的字节数目为 9
QByteArray XParser::packPayload(char cmd1, char cmd2, QByteArray &payload)
{
    int n = payload.size() + 9;
    QByteArray ret;
    ret.append(0x7F);
    ret.append(0x55);
    ret.append(cmd1);
    ret.append(cmd2);
    union unlen
    {
        quint16 n;
        char c[2];
    };

    unlen unn;
    unn.n = (short)payload.size();
    ret.append(unn.c[1]);
    ret.append(unn.c[0]);
    ret.append(payload);
    ret.append(0x01);
    ret.append(0xBE);
    ret.append(0xEE);
    return ret;
}
