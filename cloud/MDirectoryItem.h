#ifndef DIRECTORYITEM_H
#define DIRECTORYITEM_H
#include <includes.h>
#include <MItem.h>

class DirectoryItem : public MItem
{
    Q_OBJECT
public:
    DirectoryItem(const MFileInfo&);
    DirectoryItem(const DirectoryItem&);
    QLineEdit * getEdit();
    ~DirectoryItem();
public slots:
    virtual void onClicked();
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void Init();
    virtual QIcon getIcon();
};

#endif // DIRECTORYITEM_H
