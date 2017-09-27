#include "CreateDelay.h"
#include "unistd.h"
#include <QDebug>

CreateDelay::CreateDelay(QObject *parent) : QObject(parent)
{

}

void CreateDelay::setTimeDelay(int timeDelay)
{
    m_timeDelay = timeDelay;
}

void CreateDelay::doWork()
{
    qDebug () << "Start Delay ----------->";
    for (int i = 0; i < m_timeDelay; i++) {
        sleep(1);
        qDebug() << i;
    }
    emit requestStopDelay();
}
