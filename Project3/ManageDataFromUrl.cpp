#include "ManageDataFromUrl.h"
#include <unistd.h>
#include <QDebug>
#include <QtWidgets>
#include <QFile>
#include <QTextStream>

ManageDataFromUrl::ManageDataFromUrl(QWebEngineView *view, const QUrl &url)
{
    m_url = url;
    m_view = view;

    connect(m_view, SIGNAL(loadFinished(bool)), this, SLOT(runCommand()));

    this->SendRequestToServer(m_url);
}

void ManageDataFromUrl::SendRequestToServer(const QUrl &url)
{
    QString data;
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
        data = reply->readAll();
        delete reply;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }

    this->setDataReceivedFromServer(data);
}

void ManageDataFromUrl::setDataReceivedFromServer(QString data)
{
//    qDebug() << "Data reviced from URL:" << data;
    m_data = data;
    handleDataReviceFromURL(m_data);
}

void ManageDataFromUrl::handleDataReviceFromURL(QString m_data)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(m_data.toUtf8());
    if (!jsonResponse.isObject()) {
            qDebug() << "Document is not an object";
        }
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray jsonArray = jsonObject["pages"].toArray();
    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
        m_UrlData = obj["url"].toString();

        QJsonArray jsonArray_action = obj["action"].toArray();
        foreach (const QJsonValue &action_element, jsonArray_action) {
            QString jsonType = action_element.toObject()["type"].toString();
            m_listType.append(jsonType);
             QString jsonCmd = action_element.toObject()["js_cmd"].toString();
             m_listCmd.append(jsonCmd);
        }


    }

    for (int i = 0; i < m_listType.length(); i++) {
        qDebug() << m_UrlData;
        qDebug() << m_listType.at(i);
        qDebug() << m_listCmd.at(i);
    }
}

void ManageDataFromUrl::loadTheFirstUrl()
{
    m_view->load(m_UrlData);
}

void ManageDataFromUrl::runCommand()
{
    if (curentUrlIndex == 0) {
        for (int i = 0; i < m_listType.length(); i++) {
            if (m_listType.at(i) == "runjs") {
                m_view->page()->runJavaScript(m_listCmd.at(i));
            }
        }
    }
    curentUrlIndex++;
}
