#include "MDirectoryItem.h"

DirectoryItem::DirectoryItem(const MFileInfo &info):MItem(info)
{
    Init();
}

DirectoryItem::DirectoryItem(const DirectoryItem &item):MItem(item)
{
    Init();
}

QLineEdit *DirectoryItem::getEdit()
{
    return fileName;
}

DirectoryItem::~DirectoryItem()
{
    delete fileinfo;
    delete imageLabel;
    delete fileName;
    delete fileButton;
    delete layout;
    delete menu;
    delete action;
}

void DirectoryItem::onClicked()
{
    printf("diretory %s\n",fileinfo->getAbsolutepath().toUtf8().toStdString().data());

}

void DirectoryItem::contextMenuEvent(QContextMenuEvent *event)
{
    if(fileinfo->getFileName().compare("..")!=0&&fileinfo->getFileName().compare(".")!=0){
        menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
        menu->show();
    }
}
void DirectoryItem::Init()
{
    MItem::Init();
    /*  set menu and action */
    menu = new QMenu(this);
    action = new QAction("delet",menu);
    menu->addAction(action);
    /* connect */
    if(fileinfo->getFileName().compare("..")==0||fileinfo->getFileName().compare(".")==0){
        fileButton->show();
        fileButton->setCheckable(false);
    }else{
        connect(action,SIGNAL(triggered(bool)),this,SLOT(onRemove()));
        connect(fileButton,SIGNAL(clicked(bool)),this,SLOT(onShowEdit()));
        connect(fileName,SIGNAL(editingFinished()),this,SLOT(onShowButton()));
    }
}

QIcon DirectoryItem::getIcon()
{
    return QFileIconProvider().icon(QFileInfo(QDir::tempPath()));
}
