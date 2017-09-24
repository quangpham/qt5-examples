/***********************************************************
Author: hunght
Date: 24-09-2017
Email: hunght.thaibinh@gmail.com
http://telekmedia.com/qt/amazon-2.json
Load json
Open url
Run JS
***********************************************************/

#include <QApplication>
#include <QtWidgets>
#include "mainwindow.h"
#include <qtwebenginewidgetsglobal.h>

#define SERVER_URL "http://telekmedia.com/qt/amazon-2.json"

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
