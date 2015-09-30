#include "xparser.h"

XParser::XParser()
{

}


bool XParser::doParse(QByteArray src, quint16 *cmd, QByteArray &payload)
{
    if (src[0] != 0x7F || src[1] != 0x55)
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
    if (src[0] != 0x7F || src[1] != 0x55)
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
