#ifndef HBOXLAYOUT_H
#define HBOXLAYOUT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QList>
#include <Qt>
#include <QLayoutItem>

class MHBoxLayout:public QHBoxLayout{
    Q_OBJECT
private:
    QList<QWidget*> *items;
private slots:
    void hideAll();
    void showAll();
public:
    MHBoxLayout();
    void addWidget(QWidget * item, int stretch, Qt::Alignment alignment);
    ~MHBoxLayout();
};

#endif // HBOXLAYOUT_H
/*
QSlider * mslider;
QSpinBox * mspinbox;
QHBoxLayout * layout;
*/
