#include "JSONUtils.h"

JSONUtils::JSONUtils()
{

}

QString JSONUtils::Wrap(const char *name, QJsonValue data)
{
      QJsonObject obj;
      obj.insert(name,data);
      QJsonDocument doc;
      doc.setObject(obj);
      return QString(doc.toJson(QJsonDocument::Compact));
}

QString JSONUtils::Wrap(const char *name, const QString & data)
{
    QJsonObject obj;
    obj.insert(name,data);
    QJsonDocument doc;
    doc.setObject(obj);
    return QString(doc.toJson(QJsonDocument::Compact));
}
