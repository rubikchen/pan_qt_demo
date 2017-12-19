#ifndef MITEM_H
#define MITEM_H

#include<includes.h>
#include <MFileInfo.h>


class MItem : public QWidget
{
    Q_OBJECT
public:
    /* constructors and destructor*/
    MItem(const MFileInfo&);
    MItem(const MItem&);
    virtual ~MItem();

    /* getter*/
    MFileInfo getFileInfo();

signals:
    void onImageClicked(const MFileInfo&);
    void onLoseFocus();
    void changeName(QString,QString);
    void remove(QString);
public slots:
    virtual void onShowEdit();
    virtual void onShowButton();
    virtual void onClicked();
    virtual void onRemove();
protected:
    virtual QIcon getIcon();
    virtual void Init();
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    //virtual void focusOutEvent(QFocusEvent *e);
    MFileInfo *fileinfo;
    QLabel  * imageLabel;
    QLineEdit * fileName;
    QPushButton * fileButton;
    QVBoxLayout * layout;
    QMenu * menu;
    QAction * action;

};

#endif // MITEM_H
