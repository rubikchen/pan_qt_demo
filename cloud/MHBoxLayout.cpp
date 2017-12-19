#include "MHBoxLayout.h"


void MHBoxLayout::addWidget(QWidget * item, int stretch, Qt::Alignment alignment)
{
    QHBoxLayout::addWidget(item);
    items ->append(item);
}

void MHBoxLayout::hideAll()
{
    int i = 0,n = items->size();
    while(i<n){
        items->at(i)->hide();
        i++;
    }
}

void MHBoxLayout::showAll()
{
    int i = 0,n = items->size();
    while(i<n){
        items->at(i)->show();
        i++;
    }
}

MHBoxLayout::MHBoxLayout()
{
    /*
    this->setWindowTitle("layout");
    mslider = new QSlider(Qt::Horizontal);
    mspinbox = new QSpinBox;
    mslider -> setRange(0,100);
    mspinbox -> setRange(0,100);
    QObject::connect(mspinbox,SIGNAL(valueChanged(int)),mslider,SLOT(setValue(int)));
    QObject::connect(mslider,SIGNAL(valueChanged(int)),mspinbox,SLOT(setValue(int)));
    layout = new QHBoxLayout();
    layout->addWidget(mspinbox);
    layout->addWidget(mslider);
    this->setLayout(layout);
    */




}

MHBoxLayout::~MHBoxLayout()
{
    /*
    delete mslider;
    delete layout;
    delete mspinbox;
    */
    QWidget * item;
    while(item=items->at(0))
        delete item;
    delete items;
}
