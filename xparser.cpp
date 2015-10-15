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

    union unlen
    {
        quint32 n;
        char c[4];
    };

    unlen uu;
    uu.c[3] = src.at(4);
    uu.c[2] = src.at(5);
    uu.c[1] = src.at(6);
    uu.c[0] = src.at(7);

    for (int i = 0; i < uu.n; ++i)
    {
        payload.append(src[8+i]);
    }
    *cmd = type;
    return true;
}

// 将负载数据打包为可发送的数据包
// header 0x7F 0x55 2bytes    0 1
// command 2bytes             2 3
// payload length 4bytes      4 5 6 7
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
        quint32 n;
        char c[4];
    };

    unlen unn;
    unn.n = payload.size();
    ret.append(unn.c[3]);
    ret.append(unn.c[2]);
    ret.append(unn.c[1]);
    ret.append(unn.c[0]);
    ret.append(payload);
    ret.append(0x01);
    ret.append(0xBE);
    ret.append(0xEF);
    return ret;
}
