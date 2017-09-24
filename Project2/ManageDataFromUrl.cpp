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

    connect(m_view, SIGNAL(loadFinished(bool)), this, SLOT(runCmd()));

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
        m_urlData.append(obj["url"].toString());

        QJsonArray jsonArray_action = obj["action"].toArray();
        foreach (const QJsonValue &action_element, jsonArray_action) {
            m_typeData = action_element.toObject()["type"].toString();
            m_cmdData = action_element.toObject()["js_cmd"].toString();
        }


    }

    qDebug() << m_urlData;
    qDebug() << m_typeData;
    qDebug() << m_cmdData;
}

void ManageDataFromUrl::loadUrl()
{
    m_view->load(m_urlData);
}

void ManageDataFromUrl::runCmd()
{
   qDebug() << "Start run command";
   if (m_typeData == "runjs")
   m_view->page()->runJavaScript(m_cmdData);
}
