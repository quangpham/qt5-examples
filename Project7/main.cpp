/***********************************************************
Author: hunght
Date: 28-09-2017
Email: hunght.thaibinh@gmail.com
https://quangpham.github.io/qt5-examples/tasks/proxy-1.json
Load json
Check proxy
Open urls
Runjs
***********************************************************/

#include <QApplication>
#include <QtWidgets>
#include "mainwindow.h"
#include <qtwebenginewidgetsglobal.h>

#define SERVER_URL "https://quangpham.github.io/qt5-examples/tasks/amazon-3.json"

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

    return a.exec();
}
