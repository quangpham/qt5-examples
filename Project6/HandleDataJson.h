#ifndef HANDLEDATAJSON_H
#define HANDLEDATAJSON_H
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
class Proxy;
class ActionElelement;
class PageElement
{
public:
    PageElement(QJsonObject json);
    Proxy *m_proxy;
    QString m_url;
    QList<ActionElelement*> m_action;
};

class Proxy
{
public:
    Proxy(QJsonObject json);
    bool m_enalbe;
    QString m_ip;
    QString m_typeProxy;
    int m_port;
    QString m_user;
    QString m_password;
};

class ActionElelement
{
public:
    ActionElelement(QJsonObject json);
    QString m_typeCommand;
    QString m_fileName;
};

#endif // HANDLEDATAJSON_H
