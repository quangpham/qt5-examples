#ifndef MANAGEDATAFROMURL_H
#define MANAGEDATAFROMURL_H

#include <QObject>
#include <QUrl>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <QWebEngineView>
#include <QFile>
#include <QtNetwork/QNetworkProxy>
#include "HandleDataJson.h"

class ManageDataFromUrl : public QObject
{
    Q_OBJECT
public:
    ManageDataFromUrl(QWebEngineView *view, const QString& link);


    void readDataFromLocalFile(QString linkLocalFile);
    void setDataReceivedFromServer(QString data);
    void handleDataReviceFromURL(QString m_data);
    void writeContentHTMLtoFile(QString fileName, QString data);
    void runFunctionAfterAction();
    void funcZipFile(QString fileName);
    void funcUploadFile(QString fileName);
private:
    /**
     * @brief m_data: This is data receive from SERVER_URL
     */
    QString m_data;
    QString jQuery;
//    QString m_listUrl;
    QString m_url;
    QWebEngineView *m_view = new QWebEngineView();

    PageElement *m_page;
    QList<PageElement *> m_listPage;
    AfterActionElement *m_afterAction;
    QList<AfterActionElement *> m_listAfterAction;

    int m_currentIndex = 0;

signals:
    void getHTML(QString sHTML);
public slots:
    void loadUrl();
    void loadNextURL();
    void handleHTML(QString sHTML);
};

#endif // MANAGEDATAFROMURL_H
