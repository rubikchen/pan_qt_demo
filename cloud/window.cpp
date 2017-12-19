#include <QPushButton>

#include "flowlayout.h"
#include "window.h"
#include <QLineEdit>
#include <Qt>
#include <MFileItem.h>
Window::Window()
{
    FlowLayout *flowLayout = new FlowLayout;
    flowLayout->addWidget(new QPushButton(tr("Short")));
    flowLayout->addWidget(new QPushButton(tr("Longer")));
    flowLayout->addWidget(new QPushButton(tr("Different text")));
    flowLayout->addWidget(new QPushButton(tr("More text")));
    flowLayout->addWidget(new QPushButton(tr("Even longer button text")));
    /**
image=new QImage("D:/Temp/XX.jpg");
pixmap=newQPixmap();
pixmap->convertFromImage(*image);
label=newQLabel(this);
label->setGeometry(100,0,1000,700);
label->setPixmap(*pixmap);
    */
    setLayout(flowLayout);
    setWindowTitle(tr("Flow Layout"));
}
