#ifndef HANDLEDATAJSON_H
#define HANDLEDATAJSON_H
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
//class Proxy;
class ActionElelement;
class AfterActionElement;
class PageElement
{
public:
    PageElement(QJsonObject json);
    QString m_url;
    QList<ActionElelement*> m_action;
};

class ActionElelement
{
public:
    ActionElelement(QJsonObject json);
    QString m_typeCommand;
    QString m_fileName;
};
class AfterActionElement
{
public:
    AfterActionElement(QJsonObject json);
    QString m_type;
    QString m_fileName;
    QString m_file;
    QStringList m_listFile;
};

#endif // HANDLEDATAJSON_H
