#include "HandleDataJson.h"

PageElement::PageElement(QJsonObject json) {
    m_url = json["url"].toString();
    QJsonArray actions = json["action"].toArray();
    for (int i = 0; i < actions.size(); i++) {
        ActionElelement *actElement = new ActionElelement(actions[i].toObject());
        m_action.append(actElement);
    }
    qDebug() << m_url;
    for (int j = 0; j < m_action.length(); j++) {
        qDebug() << m_action.at(j)->m_fileName;
        qDebug() << m_action.at(j)->m_typeCommand;
    }
}

ActionElelement::ActionElelement(QJsonObject json) {
    m_typeCommand = json["type"].toString();
    m_fileName = json["filename"].toString();
}

AfterActionElement::AfterActionElement(QJsonObject json)
{
    m_type = json["type"].toString();
    m_fileName = json["filename"].toString();
    m_file = json["file"].toString();
    QString subFile;
    QJsonArray listFile = json["files"].toArray();
    for (int i = 0; i < listFile.size(); i++) {
        subFile = listFile.at(i).toString();
        m_listFile.append(subFile);
    }
}
