#include "Login.h"
int num=0;
QString* Login::user = NULL;
MFileInfo * Login::userfileinfo = NULL;
Login::Login(QWidget *parent) : QWidget(parent)
{


    edit_pwd  = new QLineEdit();
    edit_pwd->setMaxLength(20);
    //edit_pwd->setEchoMode(QLineEdit::Password);
    edit_nam  = new QLineEdit();
    edit_nam -> setMaxLength(20);
    label_name= new QLabel(tr("nam"));
    label_name->setBuddy(edit_nam);
    label_pwd = new QLabel(tr("pas"));
    label_pwd ->setBuddy(edit_pwd);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    bt_login  = new QPushButton(tr("login"));
    connect(bt_login,SIGNAL( clicked(bool)),this,SLOT(Logon()));
    connect(edit_nam,SIGNAL(returnPressed()),this,SLOT(Logon()));
    connect(edit_pwd,SIGNAL(returnPressed()),this,SLOT(Logon()));
    bt_regist = new QPushButton(tr("regist"));
    connect(bt_regist,SIGNAL( clicked(bool)),this,SLOT( onRegist()));
    left = new QVBoxLayout;
    left->addWidget(label_name);
    left->addWidget(label_pwd);
    right = new QVBoxLayout;
    right->addWidget(edit_nam);
    right->addWidget(edit_pwd);
    edit = new QHBoxLayout;
    edit -> addLayout(left);
    edit -> addLayout(right);
    buttom = new QHBoxLayout;
    buttom -> addWidget(bt_login);
    buttom -> addWidget(bt_regist);
    mainLayout = new FlowLayout;
    mainLayout->addItem(edit);
    mainLayout->addItem(buttom);

    this->setLayout(mainLayout);
    this->setWindowTitle("cloud");
    this->setMinimumSize(200,100);
    this->setMaximumSize(200,100);
    this->setWindowIcon(QIcon(":/new/png/fileIcon.png"));

    menu = new QMenu(this);
    action1 = new QAction(tr("refresh"), menu);
    action2 = new QAction(tr("create Directory"), menu);

    connect(action1,SIGNAL(triggered(bool)),this,SLOT(refresh()));
    connect(action2,SIGNAL(triggered(bool)),this,SLOT(createDirectory()));
    menu->addAction(action1);
    menu->addAction(action2);

    connect(this,SIGNAL(requestData(MFileInfo)),this,SLOT( onrequestData(MFileInfo)) );
    connect(this,SIGNAL(updateUI(const QList<MFileInfo>*)),this,SLOT( onupdateUI(const QList<MFileInfo>*)));
    connect(this,SIGNAL(downloadFile(MFileInfo,QString)),this,SLOT(onDownloadFile(MFileInfo,QString)));
}

QString Login::getUser()
{
    return *user;
}

Login::~Login()
{
    removeAll(mainLayout);
    delete user;
    delete scrollArea;
    delete userfileinfo;
    delete menu;
    delete action1;
    delete action2;
}

void Login::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
        menu->show();
        printf("login right clicked\n");
    }
    else if(event->button() == Qt::LeftButton){
        printf("login left clicked\n");
    }
}

void Login::onUpload(MFileInfo info)
{
    printf("login:onupload=upload clicked\n");
    QString uploadFilepath = QFileDialog::getOpenFileName(this, tr("Open File"),"./");

    if(uploadFilepath.isEmpty())
        return ;
    printf("login:onupload=upload path available\n");
    printf("login:onupload=upload path %s\n",info.getAbsolutepath().toUtf8().toStdString().data());
    QFileInfo qinfo(uploadFilepath);
    if(!util.upload(info,getUser(), qinfo.fileName()))
        return;
    printf("login:onupload=ready to upload\n");
    emit uploadFile(info,qinfo);
}

void Login::onUploadFile(MFileInfo info, QFileInfo qinfo)
{
    printf("login:onuploadfile\n");
    if(util.uploadFile(info,qinfo)){
        emit requestData(info);
    }
    else {
        printf("upload file fail\n");
    }
}

void Login::onDownload(MFileInfo info)
{
    printf("start to download path:%s\n",info.getAbsolutepath().toLocal8Bit().data());

    if(util.download(getUser()+"/"+info.getAbsolutepath())){

        QString dirpath = QFileDialog::getSaveFileName(this, "Save File",
                                                       "./untitled.*");
        emit downloadFile(info,dirpath);
    }

}

void Login::onDownloadFile(MFileInfo info, QString store)
{
    if(util.downloadFile(info.getAbsolutepath(),store))
        QDesktopServices::openUrl(QUrl(store));
}

void Login::onCreateDir()
{
    if(util.createDir(*userfileinfo,getUser(),diritem->getEdit()->text()))
        emit requestData(*userfileinfo);
    else{
        diritem->hide();
        mainLayout->removeWidget(diritem);
        update();
    }
}

void Login::onRename(QString from, QString to)
{
    if(util.rename(getUser()+"/"+from,getUser()+"/"+to))
        emit requestData(*userfileinfo);
}

void Login::Logon()
{
    if(!onCheck("login")){
        return ;
    }
}

void Login::onRegist()
{
    if(!onCheck("regist")){
        return ;
    }
}

void Login::hideAll()
{
    while( mainLayout->count()){
        hideALL(mainLayout->takeAt(0));
    }
}

void Login::refresh()
{
    printf("info %s\n",userfileinfo->getAbsolutepath().toLocal8Bit().toStdString().data());
    emit requestData(*userfileinfo);
}

void Login::createDirectory()
{
    diritem = new DirectoryItem(MFileInfo(userfileinfo->getFileName(),"untitled"));
    connect(diritem->getEdit(),SIGNAL(editingFinished()),this,SLOT(onCreateDir()) );
    mainLayout->addWidget(diritem);
    update();
    diritem->onShowEdit();
}

void Login::onRemove(QString path)
{
    if(util.remove(getUser()+"/"+path)){
        emit requestData(*userfileinfo);
    }
}

void Login::hideALL(QLayoutItem *item)
{
    if(item->layout()){
        QLayout * templayout = item->layout();
        while(templayout->count()){
            hideALL(templayout->takeAt(0));
        }
    }else if(item->widget()) {
        QWidget * tempw = item->widget();
        tempw->hide();
    }
}

void Login::onrequestData(MFileInfo info)
{
    QList<MFileInfo> *finfo =util.requestFile(info,getUser());
    userfileinfo->setFileName("");
    userfileinfo->setFilepath(finfo->at(0).getFilepath());
    userfileinfo->setType(true);
    emit updateUI(finfo);
}

void Login::onupdateUI(const QList<MFileInfo>* mitems)
{
    hideAll();
    if(mitems != NULL){
        for(int i=0;i<mitems->size();i++){
            const MFileInfo item = mitems->at(i);
            MItem *fitem;
            if(item.isDir()){
                fitem = new DirectoryItem(item);
                connect(fitem,SIGNAL(onImageClicked(MFileInfo)),this,SLOT(onrequestData(MFileInfo)));
            }else{
                fitem = new FileItem(item);
                connect(fitem,SIGNAL(onImageClicked(MFileInfo)),this,SLOT(onDownload(MFileInfo)) );
            }
            connect(fitem,SIGNAL(remove(QString)),this,SLOT(onRemove(QString)));
            connect(fitem,SIGNAL(changeName(QString,QString)),this,SLOT(onRename(QString,QString)));
            mainLayout->addWidget(fitem);
        }
    }
    uploaditem->setFileInfo(*userfileinfo);
    uploaditem->show();
    mainLayout->addWidget(uploaditem);
    update();
}

void Login::contextMenuEvent(QContextMenuEvent *event)
{
    printf("menu\n");
    menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
    menu->show();
}

void Login::removeAll(QLayoutItem * item)
{
    if(item->layout()){
        QLayout * templayout = item->layout();
        while(templayout->count()){
            removeAll(templayout->takeAt(0));
        }
        //delete templayout;
    }else if(item->widget()) {
        QWidget * tempw = item->widget();
        delete tempw;
    }
}

void Login::MainMenu()
{
    this->setMinimumSize(440,600);
    this->setMaximumSize(10000,10000);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollArea = new QScrollArea(this->nativeParentWidget());
    scrollArea->setGeometry(0, 0, 200, 200);
    //垂直滚动条不可见，只能通过鼠标滑动
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumSize(450,600);
    scrollArea->show();
    scrollArea->setWindowIcon(QIcon(":/new/png/fileIcon.png"));

}

bool Login::onCheck(const char * type)
{
    if(util.login(edit_nam->text().trimmed(),edit_pwd->text().trimmed(),QString(type))){
        user = new QString(edit_nam->text().trimmed());
        userfileinfo = new MFileInfo("","");
        /* init upload item */
        uploaditem = new UpLoadItem(*userfileinfo);
        connect(uploaditem,SIGNAL(onImageClicked(MFileInfo)),this,SLOT(onUpload(MFileInfo)));
        connect(this,SIGNAL(uploadFile(MFileInfo,QFileInfo)),this,SLOT(onUploadFile(MFileInfo,QFileInfo)) );
        //hideAll();
        MainMenu();
        emit requestData(*userfileinfo);
    }else {
        edit_pwd->setText("");
    }
    /*
    int    sockfd, rec_len;
    char   sendline[MAXLINE]={0};
    char   buf[MAXLINE]={0};
    struct sockaddr_in    servaddr;
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){  //sockfd是描述符
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return false;
    }


    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    if( inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0){   //可以在将IP地址在“点分十进制”和“二进制整数”之间转换而且inet_pton
        printf("inet_pton error for %s\n",IP);  			 //和inet_ntop这2个函数能够处理ipv4和ipv6
        return 0;
    }


    if( ::connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){  // 连接端口，将描述符和地址内容和大小作为数据 连接服务器
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    QJsonObject objData;
    objData.insert("name",edit_nam->text());
    objData.insert("password",edit_pwd->text());
    QJsonObject objForLog;
    objForLog.insert(type,objData);
    QJsonDocument docForlog;
    docForlog.setObject(objForLog);
    memset(sendline,0,MAXLINE);
    strncpy(sendline,docForlog.toJson(QJsonDocument::Compact).data(),MAXLINE);
    printf("send:%s to %d\n",sendline,sockfd);


    if( send(sockfd, sendline, strlen(sendline), 0) < 0)   // 将键盘得到的数据发送过去
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    memset(buf,0,MAXLINE);
    if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {  // 如果收到服务器返回的信息为 -1 报错，返回读取到数据的长度说明验证成功
        perror("recv error");
        return 0;
    }
    QJsonDocument doc = QJsonDocument::fromJson(buf);
    if(doc.isObject() && doc.object().contains("result")){
        if(doc.object().take("result").toInt()){
            user = new QString(edit_nam->text().trimmed());
            userfileinfo = new MFileInfo("","");
            //hideAll();
            printf("log success\n");
            //emit requestData(*userfileinfo);
            //MainMenu();
        }else{
            edit_pwd->setText("");
        }
    }
    printf("Received : %s\n",buf);
    ::close(sockfd);
 */
    return 1;
}
