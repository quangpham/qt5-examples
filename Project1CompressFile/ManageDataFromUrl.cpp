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

    connect(m_view, SIGNAL(loadFinished(bool)), this, SLOT(loadNextURl()));
    connect(this, SIGNAL(getHTML(QString)), this, SLOT(handleHTML(QString)));

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
        m_listUrl.append(obj["url"].toString());

        QJsonArray jsonArray_action = obj["action"].toArray();
        foreach (const QJsonValue &action_element, jsonArray_action) {
            QString jsonType = action_element.toObject()["type"].toString();
            m_listType.append(jsonType);
             QString jsonNameFile = action_element.toObject()["filename"].toString();
             m_listFileName.append(jsonNameFile);
        }


    }

    for (int i = 0; i < m_listUrl.length(); i++) {
        qDebug() << m_listUrl.at(i);
        qDebug() << m_listType.at(i);
        qDebug() << m_listFileName.at(i);
    }
}

void ManageDataFromUrl::loadTheFirstUrl()
{
    m_view->load(m_listUrl.at(0));
}

void ManageDataFromUrl::loadNextURl()
{
    m_view->page()->toHtml([this](const QString& result) mutable {emit getHTML(result);});
    if (curentUrlIndex < m_listUrl.length()) {
         m_view->load(m_listUrl.at(curentUrlIndex));
         curentUrlIndex++;
    }
}

void ManageDataFromUrl::handleHTML(QString sHTML)
{
    writeContentHTMLtoFile(m_listFileName.at(currentFileIndex), sHTML);
//    compressFile(m_listFileName.at(currentFileIndex));
    zipFile(m_listFileName.at(currentFileIndex), m_listFileName.at(currentFileIndex) + ".zip");
//    readContentHTMLfromFile(m_listFileName.at(currentFileIndex));
    currentFileIndex++;
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

void ManageDataFromUrl::zipFile (QString filename , QString zipfilename){

//    QFile infile(filename);
//    QFile outfile(zipfilename);
//    infile.open(QIODevice::ReadOnly);
//    outfile.open(QIODevice::WriteOnly);
//    QByteArray uncompressedData = infile.readAll();
//    QByteArray compressedData = qCompress(uncompressedData,9);
//    outfile.write(compressedData);
//    infile.close();
//    outfile.close();
    QString command = "zip " + zipfilename + " " + filename;
    qDebug() << "hunght abcd 1234" << command;
    system(command.toStdString().c_str());
}

void ManageDataFromUrl::unZipFile(QString filename, QString zipfilename)
{
    QFile infile(zipfilename);
    QFile outfile(filename);
    infile.open(QIODevice::ReadOnly);
    outfile.open(QIODevice::WriteOnly);
    QByteArray uncompressedData = infile.readAll();
    QByteArray compressedData = qUncompress(uncompressedData);
    outfile.write(compressedData);
    infile.close();
    outfile.close();
}
