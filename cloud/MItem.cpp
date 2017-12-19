#include "MItem.h"

MItem::MItem(const MFileInfo &info)
{
    fileinfo = new MFileInfo(info);
}

MItem::MItem(const MItem &item)
{
    fileinfo = new MFileInfo(*item.fileinfo);
}

MItem::~MItem()
{

}


void MItem::onShowEdit()
{
    printf("executed\n");
    fileButton->hide();
    fileName->show();
    fileName->setFocus();
    fileName->selectAll();
}

void MItem::onShowButton()
{
    /* setting button get foucsed and */
    fileButton->setFocus();
    fileName->hide();
    if(fileName->text().trimmed().isEmpty()){
        fileName->setText(fileButton->text());
        fileButton->show();
    }else{
        QString from = fileButton->text();
        QString to  =  fileName->text().trimmed();
        fileButton->setText(to);
        fileButton->show();
        emit changeName(from,to);
    }

}

void MItem::onClicked()
{
    printf("mitem clicked\n");
}

void MItem::onRemove()
{
    emit remove(fileinfo->getAbsolutepath());
}

void MItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    if(x>=0&&x<200&&y>=0&&y<=200)
        emit onImageClicked(*fileinfo);
}

void MItem::mousePressEvent(QMouseEvent *event)
{
        int x = event->x();
        int y = event->y();
        if(x>=0&&x<200&&y>=0&&y<=200)
            emit onLoseFocus();
}

QIcon MItem::getIcon()
{
    return QIcon(":/new/png/files.png");
}


void MItem::Init()
{

    /*  set and load picture  */
    imageLabel = new QLabel;
    imageLabel->setScaledContents(true);
    imageLabel->setPixmap(getIcon().pixmap(160,160));
    imageLabel->setFixedSize(150,150);
    imageLabel->setAlignment(Qt::AlignCenter);

    /* set and load edit */
    fileName = new QLineEdit(fileinfo->getFileName());
    fileName->setMaxLength(20);
    fileName->setAlignment(Qt::AlignCenter);
    fileName->setFixedHeight(20);
    fileName->hide();

    /* set and load button */
    fileButton = new QPushButton(fileName->text());
    fileButton->setFixedHeight(20);
    /* set layout */
    layout = new QVBoxLayout;

    /* add items to layout */
    layout->addWidget(imageLabel);
    layout->addWidget(fileName);
    layout->addWidget(fileButton);

    /* add layout manager */
    this->setLayout(layout);
    /* set widget unchangeble width and length*/
    this->setFixedSize(200,200);

}

