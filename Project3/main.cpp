/***********************************************************
Author: hunght
Date: 19-09-2017
Email: hunght.thaibinh@gmail.com
http://telekmedia.com/qt/facebook-1.json
Login with fb
***********************************************************/

#include <QApplication>
#include <QtWidgets>
#include "mainwindow.h"
#include <qtwebenginewidgetsglobal.h>

#define SERVER_URL "http://telekmedia.com/qt/facebook-1.json"

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
