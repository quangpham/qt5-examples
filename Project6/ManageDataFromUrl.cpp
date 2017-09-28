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

//    connect(m_view, SIGNAL(loadFinished(bool)), this, SLOT(runCommand()));

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
    for (int i = 0; i < jsonArray.size(); i++) {
        PageElement *m_page = new PageElement(jsonArray[i].toObject());
        m_Listpage.append(m_page);
    }

}

void ManageDataFromUrl::loadUrl()
{
    if (m_Listpage.at(0)->m_proxy->m_enalbe) {
        QNetworkProxy proxy;
        if (m_Listpage.at(0)->m_proxy->m_typeProxy == "HTTP") {
            proxy = QNetworkProxy::HttpProxy;
        }
        proxy.setHostName(m_Listpage.at(0)->m_proxy->m_ip);
        proxy.setPort(m_Listpage.at(0)->m_proxy->m_port);
        proxy.setUser(m_Listpage.at(0)->m_proxy->m_user);
        proxy.setPassword(m_Listpage.at(0)->m_proxy->m_password);
        QNetworkProxy::setApplicationProxy(proxy);
    }

    m_view->load(m_Listpage.at(0)->m_url);
}
