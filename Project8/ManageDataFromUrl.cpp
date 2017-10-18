#include "ManageDataFromUrl.h"
#include <unistd.h>
#include <QDebug>
#include <QtWidgets>
#include <QFile>
#include <QTextStream>

ManageDataFromUrl::ManageDataFromUrl(QWebEngineView *view, const QString &link)
{
    m_url = link;
    m_view = view;

    connect(m_view, SIGNAL(loadFinished(bool)), this, SLOT(loadNextURL()));
    connect(this, SIGNAL(getHTML(QString)), this, SLOT(handleHTML(QString)));

    this->readDataFromLocalFile(m_url);
}

void ManageDataFromUrl::readDataFromLocalFile(QString linkLocalFile)
{
    QFile file(linkLocalFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Coudln't open file for reading";
        return;
    }
    QTextStream in(&file);
    QString myText = in.readAll();
    qDebug() << myText;
    this->setDataReceivedFromServer(myText);
}

void ManageDataFromUrl::setDataReceivedFromServer(QString data)
{
//    qDebug() << "Data reviced from URL:" << data;
    m_data = data;
    handleDataReviceFromURL(m_data);
    this->loadUrl();
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
        m_page = new PageElement(jsonArray[i].toObject());
        m_listPage.append(m_page);
    }

    jsonArray = jsonObject["after_actions"].toArray();
    for (int i = 0; i < jsonArray.size(); i++) {
        m_afterAction = new AfterActionElement(jsonArray[i].toObject());
        m_listAfterAction.append(m_afterAction);
    }

}

void ManageDataFromUrl::loadUrl()
{
    m_view->load(m_listPage.at(0)->m_url);
}

void ManageDataFromUrl::loadNextURL()
{
//    m_currentIndex++;
    if (m_currentIndex < m_listPage.length()) {
        if (m_listPage.at(m_currentIndex)->m_action.at(0)->m_typeCommand == "save_html_local") {
            m_view->page()->toHtml([this](const QString& result) mutable {emit getHTML(result);});
        }
         m_view->load(m_listPage.at(m_currentIndex)->m_url);
    }

}
void ManageDataFromUrl::handleHTML(QString sHTML)
{
    if (m_currentIndex < m_listPage.length()) {
        writeContentHTMLtoFile(m_listPage.at(m_currentIndex)->m_action.at(0)->m_fileName, sHTML);
        m_currentIndex++;
    }
    if (m_currentIndex == m_listPage.length()) {
        qDebug() << "hunght abcd" << m_currentIndex;
//        runFunctionAfterAction();
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
    this->funcZipFile(fileName);
}

void ManageDataFromUrl::runFunctionAfterAction()
{
    for (int i = 0; i < m_listAfterAction.length(); i++) {
        if (m_listAfterAction.at(i)->m_type == "zip_files") {
            funcZipFile(m_listAfterAction.at(i)->m_fileName);
        } else {
            funcUploadFile(m_listAfterAction.at(i)->m_file);
        }
    }
}

void ManageDataFromUrl::funcZipFile(QString fileName)
{
    qDebug() << "Start zip file: Create file:" << fileName;
    QString zipFileName = fileName + ".zip";
    QString command = "zip " + zipFileName + " " + fileName;
    system(command.toStdString().c_str());

}

void ManageDataFromUrl::funcUploadFile(QString fileName)
{
    qDebug() << "Start upload file: Need up file:" << fileName;
}
