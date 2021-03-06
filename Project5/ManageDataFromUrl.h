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
    ManageDataFromUrl(QWebEngineView *view, const QUrl& url);
    void SendRequestToServer(const QUrl& url);
    void setDataReceivedFromServer(QString data);
    void handleDataReviceFromURL(QString m_data);
private:
    /**
     * @brief m_data: This is data receive from SERVER_URL
     */
    QString m_data;
    QString jQuery;
    QString m_listUrl;
    QUrl m_url;
    QWebEngineView *m_view = new QWebEngineView();

    PageElement *m_page;

signals:
    void getHTML(QString sHTML);
public slots:
    void loadUrl();
    void runCommand();
};

#endif // MANAGEDATAFROMURL_H
