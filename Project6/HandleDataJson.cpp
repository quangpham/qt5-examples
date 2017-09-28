#include "HandleDataJson.h"

Proxy::Proxy(QJsonObject json) {
    m_enalbe = json["enable"].toBool() ? json["enable"].toBool(): false;
    m_ip = json["ip"].toString();
    m_typeProxy = json["type"].toString();
    m_port = json["port"].toInt();
    m_user = json["user"].toString();
    m_password = json["password"].toString();
}

ActionElelement::ActionElelement(QJsonObject json) {
    m_typeCommand = json["type"].toString();
    m_fileName = json["js_cmd"].toString();
}

PageElement::PageElement(QJsonObject json) {
    m_proxy = new Proxy(json["proxy"].toObject());
    m_url = json["url"].toString();
    QJsonArray actions = json["action"].toArray();
    for (int i = 0; i < actions.size(); i++) {
        ActionElelement *actElement = new ActionElelement(actions[i].toObject());
        m_action.append(actElement);
    }
    qDebug() << m_url;
    qDebug() << m_proxy->m_enalbe << m_proxy->m_ip << m_proxy->m_password << m_proxy->m_port << m_proxy->m_typeProxy << m_proxy->m_user;
    for (int j = 0; j < m_action.length(); j++) {
        qDebug() << m_action.at(j)->m_fileName;
        qDebug() << m_action.at(j)->m_typeCommand;
    }
}
