#ifndef XPARSER_H
#define XPARSER_H

#include <QByteArray>

class XParser
{
public:
    XParser();
    static bool doParse(QByteArray src, quint16 *cmd, QByteArray &payload);
    static bool doParseLong(QByteArray src, quint16 *cmd, QByteArray &payload);
};

#endif // XPARSER_H
