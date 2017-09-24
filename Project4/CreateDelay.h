#ifndef CREATEDELAY_H
#define CREATEDELAY_H

#include <QObject>

class CreateDelay : public QObject
{
    Q_OBJECT
public:
    explicit CreateDelay(QObject *parent = nullptr);

signals:
    void requestStopDelay();
public slots:
    void doWork();
};

#endif // CREATEDELAY_H
