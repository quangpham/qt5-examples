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

class ManageDataFromUrl : public QObject
{
    Q_OBJECT
public:
    ManageDataFromUrl(QWebEngineView *view, const QUrl& url);
    void SendRequestToServer(const QUrl& url);
    void setDataReceivedFromServer(QString data);
    void handleDataReviceFromURL(QString m_data);
    void writeContentHTMLtoFile(QString fileName, QString data);
    void readContentHTMLfromFile(QString fileName);
    void compressFile(QString inputFileName);
    void uncompressFile(QString fileName);
private:
    /**
     * @brief m_data: This is data receive from SERVER_URL
     */
    QString m_data;
    QString jQuery;
    QStringList m_listFileName;
    QStringList m_listUrl;
    QStringList m_listType;
    QUrl m_url;
    QThread *m_thread;
    QWebEngineView *m_view = new QWebEngineView();
    int curentUrlIndex = 1;
    int currentFileIndex = 0;

signals:
    void getHTML(QString sHTML);
public slots:
    void loadTheFirstUrl();
    void loadNextURl();
    void handleHTML(QString sHTML);
};

#endif // MANAGEDATAFROMURL_H
