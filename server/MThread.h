#ifndef MTHREAD_H
#define MTHREAD_H

#include <QThread>
#include <QMutex>
#include <QString>
#include <QDir>


class MThread : public QThread
{
    Q_OBJECT
public:
    MThread();
    MThread(QString );
protected:
    virtual void run();
private:
    QMutex * mutex;
    QString filePath;
};

#endif // MTHREAD_H
