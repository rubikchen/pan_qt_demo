#ifndef UPLOADITEM_H
#define UPLOADITEM_H

#include <MItem.h>
#include <DataUtil.h>
class UpLoadItem : public MItem
{
    Q_OBJECT
public:
    UpLoadItem(const MFileInfo&);
    UpLoadItem(const UpLoadItem&);
    void setFileInfo(const MFileInfo&);
    ~UpLoadItem();
public slots:
    void onUpload();
protected:
    virtual QIcon getIcon();
    virtual void Init();
};

#endif // UPLOADITEM_H
