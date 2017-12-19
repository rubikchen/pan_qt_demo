#ifndef DOWNLOADBUTTONBOX_H
#define DOWNLOADBUTTONBOX_H

#include <QDialogButtonBox>
#include <QWidget>
#include <QList>
class DownloadButtonBox : QDialogButtonBox
{
public:
    DownloadButtonBox();
private:
    QList<void *> *widgets ;

    void setLayout(QLayout *);

};

#endif // DOWNLOADBUTTONBOX_H
