#include "UploadItem.h"


UpLoadItem::UpLoadItem(const MFileInfo &info):MItem(info)
{
    Init();
}

UpLoadItem::UpLoadItem(const UpLoadItem &item):MItem(item)
{
    Init();
}

void UpLoadItem::setFileInfo(const MFileInfo &info)
{
    fileinfo->setFileName(info.getFileName());
    fileinfo->setFilepath(info.getFilepath());
    fileinfo->setType(info.isDir());
}

UpLoadItem::~UpLoadItem()
{
    delete imageLabel;
    delete layout;
}

void UpLoadItem::onUpload()
{
printf("uploaditem upload path %s\n",fileinfo->getFilepath().toUtf8().toStdString().data());
/*
    QString uploadFilepath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                            "/home");
    printf("%s\n",uploadFilepath.toLocal8Bit().data());
    if(uploadFilepath.isEmpty()){
        return;
    }
*/
}

QIcon UpLoadItem::getIcon()
{
    return QIcon(":/new/png/fileUpload.png");
}

void UpLoadItem::Init()
{
    imageLabel=new QLabel;
    imageLabel->resize(150,150);
    imageLabel->setPixmap(getIcon().pixmap(200,200));
    imageLabel->setScaledContents(true);

    layout = new QVBoxLayout;
    layout->addWidget(imageLabel);
    this->setLayout(layout);
    this->setFixedSize(200,200);
    connect(this,SIGNAL(onImageClicked(MFileInfo)),this,SLOT( onUpload()) );
}

