#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QString>
#include <QObject>

class JSONUtils
{
public:
    static QString Wrap(const char *,QJsonValue);
    static QString Wrap(const char *,const QString &);
private:
    JSONUtils();
};

#endif // JSONUTILS_H
