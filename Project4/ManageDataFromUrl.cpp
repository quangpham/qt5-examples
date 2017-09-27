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
    QList<QString> mlist;
    foreach (const QJsonValue & value, jsonArray) {
        mlist.clear();
        QJsonObject obj = value.toObject();
        mlist.append(obj["url"].toString());
        QJsonArray jsonArray_action = obj["action"].toArray();
        foreach (const QJsonValue &action_element, jsonArray_action) {
            QString jsonType = action_element.toObject()["type"].toString();
            mlist.append(jsonType);
            QString jsonNameFile = action_element.toObject()["js_cmd"].toString();
            mlist.append(jsonNameFile);
            int jsonDelay = action_element.toObject()["delay"].toInt();
            mlist.append(QString::number(jsonDelay));
        }
        m_list.append(mlist);
    }
    for (int a = 0; a < m_list.length(); a++) {
        QList<QString> a_list = m_list.at(a);
        for (int b = 0; b < a_list.length(); b++) {
            qDebug() << a_list.at(b);
        }
    }

//    for (int i = 0; i < m_listType.length(); i++) {
//        qDebug() << m_listType.at(i);
//    }
//    for (int i = 0; i < m_listUrl.length(); i++) {
//        qDebug() << m_listUrl.at(i);
//    }
//    for (int i = 0; i < m_listDelay.length(); i++) {
//        qDebug() << m_listDelay.at(i);
//    }
//    for (int i = 0; i < m_listCmd.length(); i++) {
//        qDebug() << m_listCmd.at(i);
//    }
}

void ManageDataFromUrl::loadTheFirstUrl()
{
//    m_view->load(m_listUrl.at(0));
    m_view->load(m_list.at(0).at(0));
}

void ManageDataFromUrl::loadNextURl()
{
    m_thread->terminate();
    qDebug() << "Start Load the next URL" << m_list.length();
    for (int i = 1; i < m_list.length(); i++) {
        m_view->load(m_list.at(i).at(0));
    }
    curentUrlIndex++;
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
    qDebug() << "Start run Commnad" << curentUrlIndex;
    if (curentUrlIndex < m_list.length()) {
        QList<QString> mlist = m_list.at(curentUrlIndex);
        for (int j = 0; j < mlist.length(); j++) {
            if (mlist.at(j) == "runjs") {
                qDebug() << "Run Command" << mlist.at(j+1);
                m_view->page()->runJavaScript(mlist.at(j+1));
            } else if (mlist.at(j) == "delay") {
                qDebug() << "Start Delay" << mlist.at(j+2);
                m_createDelay->setTimeDelay((mlist.at(j+2)).toInt() / 1000);
                m_thread->start();
            }
        }
    }
}
