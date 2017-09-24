#include "CreateDelay.h"
#include "unistd.h"
#include <QDebug>

CreateDelay::CreateDelay(QObject *parent) : QObject(parent)
{

}

void CreateDelay::doWork()
{
    qDebug () << "Start Delay";
    for (int i = 0; i < 5; i++) {
        sleep(1);
        qDebug() << i;
    }
    emit requestStopDelay();
}
