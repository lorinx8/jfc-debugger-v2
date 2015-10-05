#ifndef XPARSER_H
#define XPARSER_H

#include <QByteArray>

class XParser
{
public:
    XParser();
    static bool doParse(QByteArray src, quint16 *cmd, QByteArray &payload);
    static bool doParseLong(QByteArray src, quint16 *cmd, QByteArray &payload);
    static QByteArray packPayload(char cmd1, char cmd2, QByteArray &payload);
};

#endif // XPARSER_H
