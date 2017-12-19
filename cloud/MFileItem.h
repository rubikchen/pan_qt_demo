#ifndef FILEITEM_H
#define FILEITEM_H

#include <includes.h>
#include <MFileInfo.h>
#include <MItem.h>
#include <QTemporaryFile>
#include <QIcon>
class FileItem : public MItem
{
    Q_OBJECT
public:
    FileItem(const MFileInfo&);
    FileItem(const FileItem&);
    ~FileItem();
public slots:
    virtual void onClicked();
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void Init();
    virtual QIcon getIcon();
};

#endif // FILEITEM_H
