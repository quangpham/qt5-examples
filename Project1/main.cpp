/***********************************************************
Author: hunght
Date: 19-09-2017
Email: hunght.thaibinh@gmail.com
http://telekmedia.com/qt/amazon-1.json
Load json
Open urls
Save content to id.html
***********************************************************/

#include <QApplication>
#include <QtWidgets>
#include "mainwindow.h"
#include <qtwebenginewidgetsglobal.h>
#include <QtNetwork/QNetworkProxy>

#define SERVER_URL "http://telekmedia.com/qt/amazon-1.json"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QUrl url;
    if (argc > 1)
        url = QUrl::fromUserInput(argv[1]);
    else
        url = QUrl(SERVER_URL);
    MainWindow *browser = new MainWindow(url);
    browser->show();

    QNetworkProxy proxy;

    proxy = QNetworkProxy::HttpProxy;
    proxy.setHostName("46.101.24.27");
    proxy.setPort(3128);
    proxy.setUser("");
    proxy.setPassword("");
    QNetworkProxy::setApplicationProxy(proxy);

    return a.exec();
}
