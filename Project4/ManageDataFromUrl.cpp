#include "ManageDataFromUrl.h"
#include <unistd.h>
#include <QDebug>
#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <unistd.h>

ManageDataFromUrl::ManageDataFromUrl(QWebEngineView *view, const QUrl &url)
{
    m_url = url;
    m_view = view;
    connect(m_view, SIGNAL(loadFinished(bool)), this, SLOT(runCommand()));
//    connect(this, SIGNAL(startNextUrl()), this, SLOT(loadNextURl()));
    this->SendRequestToServer(m_url);

    m_createDelay = new CreateDelay();
    m_thread = new QThread();
    m_createDelay->moveToThread(m_thread);
    connect(m_thread, SIGNAL(started()), m_createDelay, SLOT(doWork()));
    connect(m_createDelay, SIGNAL(requestStopDelay()), this, SLOT(loadNextURl()));
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
        m_listUrl.append(obj["url"].toString());

        QJsonArray jsonArray_action = obj["action"].toArray();
        foreach (const QJsonValue &action_element, jsonArray_action) {
            QString jsonType = action_element.toObject()["type"].toString();
            m_listType.append(jsonType);
            QString jsonNameFile = action_element.toObject()["js_cmd"].toString();
            m_listCmd.append(jsonNameFile);
            int jsonDelay = action_element.toObject()["delay"].toInt();
            m_listDelay.append(jsonDelay);
        }
    }

    for (int i = 0; i < m_listType.length(); i++) {
        qDebug() << m_listType.at(i);
    }
    for (int i = 0; i < m_listUrl.length(); i++) {
        qDebug() << m_listUrl.at(i);
    }
    for (int i = 0; i < m_listDelay.length(); i++) {
        qDebug() << m_listDelay.at(i);
    }
    for (int i = 0; i < m_listCmd.length(); i++) {
        qDebug() << m_listCmd.at(i);
    }
}

void ManageDataFromUrl::loadTheFirstUrl()
{
    m_view->load(m_listUrl.at(0));
}

void ManageDataFromUrl::loadNextURl()
{
    qDebug() << "Start Load the next URL";
    if (curentUrlIndex <= m_listUrl.length()) {
        qDebug() << "Start here";
         m_view->load(m_listUrl.at(curentUrlIndex - 1));
    }
}

void ManageDataFromUrl::writeContentHTMLtoFile(QString fileName, QString data)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text) ) {
        qDebug() << "Couldn't open file for writing";
        return;
    }

    QTextStream out(&file);
    out << data;
    file.flush();
    file.close();
}

void ManageDataFromUrl::readContentHTMLfromFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Coudln't open file for reading";
        return;
    }
    QTextStream in(&file);
    QString myText = in.readAll();
    qDebug() << myText;
}

void ManageDataFromUrl::runCommand()
{
    qDebug() << "Start run command 1234456676";
    if (curentUrlIndex == 1) {
        qDebug() << "Start run command 1";
        for (int i = 0; i < 4; i++) {
            if (m_listType.at(i) == "runjs") {
                m_view->page()->runJavaScript(m_listCmd.at(i));
            }
        }
//        sleep(5);
        m_thread->start();
        qDebug() << "Start Signal 1";
//        emit startNextUrl();
    }
    if (currentFileIndex == 2) {
        qDebug() << "Start run command 2";
        m_view->page()->runJavaScript(m_listCmd.at(4));
    }
    curentUrlIndex++;
}
