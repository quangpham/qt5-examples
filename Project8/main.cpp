/***********************************************************
Author: hunght
Date: 21:00 17-10-2017
Email: hunght.thaibinh@gmail.com
Link Local File: amazon-3.json
Read json
Open urls
Runjs
***********************************************************/


/*
    Do hiện tại file amazon-3.json được đặt trong cùng thư mục với file Build của Project8
    Cách chạy project :
    ./Project8 amazon-3.json

*/
#include <QApplication>
#include <QtWidgets>
#include "mainwindow.h"
#include <qtwebenginewidgetsglobal.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    QString link = argv[1];

    MainWindow *browser = new MainWindow(link);
    browser->show();

    qDebug() << "Link File Json" << link;

    return a.exec();
}
