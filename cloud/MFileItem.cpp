#include "MFileItem.h"

FileItem::FileItem(const MFileInfo &info):MItem(info)
{
    Init();
}

FileItem::FileItem(const FileItem &item):MItem(item)
{
    Init();
}

FileItem::~FileItem()
{
    delete fileinfo;
    delete imageLabel;
    delete fileName;
    delete fileButton;
    delete layout;
    delete menu;
    delete action;
}

void FileItem::onClicked()
{
    printf("file clicked\n");
}

void FileItem::contextMenuEvent(QContextMenuEvent *event)
{
    menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
    menu->show();
}

void FileItem::Init()
{
    MItem::Init();
    /*  set menu and action */
    menu = new QMenu(this);
    action = new QAction("delet",menu);
    menu->addAction(action);
    connect(action,SIGNAL(triggered(bool)),this,SLOT(onRemove()));
    connect(fileButton,SIGNAL(clicked(bool)),this,SLOT(onShowEdit()));
    connect(fileName,SIGNAL(editingFinished()),this,SLOT(onShowButton()));
}

QIcon FileItem::getIcon()
{
    QFileIconProvider provider;
    QIcon icon;
    QString strTemplateName = QDir::tempPath() + QDir::separator() +
            QCoreApplication::applicationName() + "_XXXXXX" + fileinfo->getFileName();
    QTemporaryFile tmpFile(strTemplateName);
    tmpFile.setAutoRemove(true);
    if (tmpFile.open())
    {
        tmpFile.close();
        icon = provider.icon(QFileInfo(strTemplateName));
        tmpFile.remove();
    }
    return icon;
}
