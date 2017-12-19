#ifndef LOGIN_H
#define LOGIN_H

#include <includes.h>
#include <QScrollArea>
#include <QListWidget>
#include <UploadItem.h>
#include <MItem.h>
#include <MFileItem.h>
#include <DataUtil.h>
class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = nullptr);
    QString getUser();
    ~Login();
signals:
    void requestData(MFileInfo);
    void uploadFile(MFileInfo,QFileInfo);
    void updateUI(const QList<MFileInfo>*);
    void downloadFile(MFileInfo,QString);
    void createDir(MFileInfo,QString);
private slots:
    virtual void mousePressEvent(QMouseEvent *event);
    void onUpload(MFileInfo);
    void onUploadFile(MFileInfo,QFileInfo);
    void onDownload(MFileInfo);
    void onDownloadFile(MFileInfo,QString);
    void onCreateDir();
    void onRename(QString,QString);
    void Logon();
    void onRegist();
    void hideAll();
    void refresh();
    void createDirectory();
    void onRemove(QString);
    void onrequestData(MFileInfo);
    void onupdateUI(const QList<MFileInfo>*);
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    void removeAll(QLayoutItem *);
    void hideALL(QLayoutItem *);
private:
    static MFileInfo *userfileinfo;
    static QString *user;
    void MainMenu();
    bool onCheck(const char * type);
    DirectoryItem * diritem;
    QVBoxLayout * left;
    QVBoxLayout * right;
    QHBoxLayout * edit;
    QHBoxLayout * buttom;
    QScrollArea * scrollArea;
    QList<QWidget*> widgets;
    QLayout * mainLayout;
    QLineEdit * edit_pwd;
    QLineEdit * edit_nam;
    QLabel  * label_name;
    QLabel  * label_pwd;
    QPushButton * bt_login;
    QPushButton * bt_regist;
    UpLoadItem * uploaditem;
    QMenu *menu;
    QAction * action1;
    QAction * action2;
    DataUtil util;
};

#endif // LOGIN_H
