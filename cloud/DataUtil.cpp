#include "DataUtil.h"

bool DataUtil::closefd()
{
    inited=false;
    ::close(sockfd);
    return true;
}

QList<MFileInfo>* DataUtil::requestFile(const QString & path, const QString & user)
{

    if(!inited && !init() ){
        closefd();
        return NULL;
    }

    /* declare var */
    QJsonObject container;
    QJsonObject obj;
    QJsonDocument doc;


    /* insert data into obj */
    obj.insert("path",path);
    obj.insert("user",user);
    container.insert("directory",obj);
    doc.setObject(container);


    /* send data to service */
    if( send(sockfd, doc.toJson(QJsonDocument::Compact).toStdString().data(), doc.toJson(QJsonDocument::Compact).size(), 0) < 0)   // 将键盘得到的数据发送过去
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return NULL;
    }
    /* receive data from service*/

    memset(buf,0,MAXLENGTH);
    if((rec_len = recv(sockfd, buf, MAXLENGTH,0)) == -1) {  // 如果收到服务器返回的信息为 -1 报错，返回读取到数据的长度说明验证成功
        perror("recv error");
        closefd();
        return NULL;
    }
    buf[rec_len]=0;

    QJsonDocument sdoc = QJsonDocument::fromJson(buf);
    QJsonArray values = sdoc.array();
    QList<MFileInfo> * files= new QList<MFileInfo>;
    for(int i = 0 ; i < values.size() ; i++){
        QJsonObject temp = values.at(i).toObject();
        files->append(MFileInfo(temp.take("path").toString()
                                ,temp.take("name").toString(),temp.take("type").toBool()));
    }
    closefd();
    return files;
}

QList<MFileInfo>* DataUtil::requestFile(const MFileInfo &info, const QString &user)
{
    return requestFile(info.getAbsolutepath(),user);
}

bool DataUtil::download(const QString &path)
{
    if(!inited && !init() ){
        printf("download failed\n");
        closefd();
        return false;
    }
    QJsonObject obj;
    QJsonDocument doc;
    obj.insert("downloadfile",path);
    doc.setObject(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact) ;
    printf("datautil:download=send to %s\n",data.toStdString().data());
    if( send(sockfd,data.toStdString().data(), data.size(), 0) < 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return false;
    }
    printf("send success\n");
    if((rec_len = recv(sockfd, buf, MAXLINE,0))==-1){
        closefd();
        return false;
    }
    buf[rec_len]=0;
    QJsonValue temp = QJsonDocument::fromJson(buf).object().take("result");

    printf("received:%s %d %s\n",buf,rec_len,temp.toString().toLocal8Bit().data());
    if(!temp.toBool()){
        closefd();
        return false;
    }
    closefd();
    return true;
}

bool DataUtil::downloadFile(const QString& filepath,const QString& dirpath){

    if(!inited && !init() ){
        printf("download failed\n");
        closefd();
        return false;
    }
    if(dirpath.isEmpty()){
        send(sockfd,"a",1,0);
        closefd();
        return false;
    }
    FILE * fp = fopen(dirpath.toUtf8().toStdString().data(),"wb+");
    if(fp==NULL){
        printf("datautil:download=downloadFailed\n");
        return false;
    }
    int persize = 1024*4;
    printf("datautilfile:download=start to download %d\n",inited);
    int size = 0,n;
    printf("start sendingt\n");
    while(1){
        if((n=send(sockfd,"1",2,0))<0 ){
            printf("send error: %s\n",strerror(errno));
            fclose(fp);
            closefd();
            return false;
        }
        memset(buf,0,MAXLENGTH);
        if((n= recv(sockfd, buf, MAXLENGTH, 0))<=0){
            printf("recive erro\n");
            fclose(fp);
            closefd();
            return false;
        }
        size += n;
        for(int i=0;i<n;i++)
            fputc(buf[i],fp);
        if( n != persize )
            break;
    }
    fclose(fp);
    closefd();
    return true;
}

bool DataUtil::upload(const MFileInfo & file,const QString&user, const QString &name)
{
    if(!inited && !init() ){
        closefd();
        return false;
    }
    QJsonObject obj;
    QJsonDocument doc;
    obj.insert("uploadfile",user+"/"+file.getAbsolutepath()+"/"+name);
    doc.setObject(obj);
    if( send(sockfd, doc.toJson(QJsonDocument::Compact).toStdString().data(), doc.toJson(QJsonDocument::Compact).size(), 0) < 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return false;
    }
    printf("datautil:upload=send ok\n");
    if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {  // 如果收到服务器返回的信息为 -1 报错，返回读取到数据的长度说明验证成功
        perror("recv error");
        closefd();
        return false;
    }
    buf[rec_len]=0;
    QJsonDocument sdoc = QJsonDocument::fromJson(buf);
    printf("datautil:upload=receivedjson:%s\n",buf);
    if(sdoc.isObject() && sdoc.object().contains("result")){
        QJsonValue value = sdoc.object().take("result");
        printf("return %d\n",value.toBool());
        if(value.toBool()){
            return true;
        }else {
            closefd();
            return false;
        }
    }
    closefd();
    return false;
}

bool DataUtil::uploadFile(const MFileInfo &fileinfo, const QFileInfo &path)
{
    if(!inited && !init() ){
        closefd();
        return false;
    }
    printf("datautil:uploadFile\n");
    FILE * fp = fopen(path.absoluteFilePath().toUtf8().toStdString().data(),"rb+");
    if(NULL==fp){
        closefd();
        return false;
    }
    printf("datautil:uploadfile= path %s\n",
           path.absoluteFilePath().toUtf8().toStdString().data());
    fseek(fp,0,SEEK_END);
    int len = ftell(fp),i=0,persize=1024*4,t;
    fseek(fp,0,SEEK_SET);
    printf("datautil:uploadfile= size %d\n",len);
    while(i<len){
        t=len-i<persize?len-i:persize;
        for(int j=0;j<t;j++,i++){
            buf[j]=char(fgetc(fp));
        }
        buf[t]=0;
        if( send(sockfd,buf,t, 0) < 0)
        {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            closefd();
            fclose(fp);
            return false;
        }
        if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {  // 如果收到服务器返回的信息为 -1 报错，返回读取到数据的长度说明验证成功
            perror("recv error");
            closefd();
            fclose(fp);
            return false;
        }
        memset(buf,0,MAXLINE);

    }
    closefd();
    fclose(fp);
    return true;
}

bool DataUtil::remove(const QString &path)
{
    if(!inited && !init() ){
        closefd();
        return false;
    }
    QJsonObject obj;
    QJsonDocument doc;
    obj.insert("removefile",path);
    doc.setObject(obj);
    QByteArray json = doc.toJson(QJsonDocument::Compact);
    if(send(sockfd,json.toStdString().data(),json.size(),0)<=0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return false;
    }
    int len;
    if((len=recv(sockfd,buf,MAXLENGTH,0))<0){
        printf("receive msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return false;
    }
    buf[len]=0;
    printf("removefile receive %s\n",buf);
    if(doc.fromJson(buf).object().take("result").toInt()){
        printf("remove success\n");
        closefd();
        return true;
    }
    closefd();
    return false;
}

bool DataUtil::rename(const QString &from, const QString &to)
{
    if(!inited && !init() ){
        closefd();
        return false;
    }
    QJsonObject obj;
    QJsonDocument doc;
    QJsonArray array;
    QJsonObject o1;
    QJsonObject o2;
    o1.insert("from",from);
    o2.insert("to",to);
    array.insert(0,o1);
    array.insert(0,o2);
    obj.insert("renamefile",array);
    doc.setObject(obj);
    QByteArray json = doc.toJson(QJsonDocument::Compact);
    if(send(sockfd,json.toStdString().data(),json.size(),0)<=0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return false;
    }
    int len;
    if((len=recv(sockfd,buf,MAXLENGTH,0))<0){
        printf("receive msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return false;
    }
    buf[len]=0;
    printf("rename receive %s\n",buf);
    if(doc.fromJson(buf).object().take("result").toInt()){
        printf("rename success\n");
        closefd();
        return true;
    }
    closefd();
    return false;
}

bool DataUtil::createDir(const MFileInfo &path, const QString & user,const QString &dirname)
{
    if(!inited && !init() ){
        closefd();
        return false;
    }
    QJsonObject obj;
    QJsonDocument doc;
    obj.insert("creatdirectory",user+"/"+path.getAbsolutepath()+"/"+dirname);
    doc.setObject(obj);
    QByteArray json = doc.toJson(QJsonDocument::Compact);
    if(send(sockfd,json.toStdString().data(),json.size(),0)<=0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return false;
    }
    int len;
    if((len=recv(sockfd,buf,MAXLENGTH,0))<0){
        printf("receive msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return false;
    }
    buf[len]=0;
    printf("createDir receive %s\n",buf);
    if(doc.fromJson(buf).object().take("result").toInt()){
        printf("create success\n");
        closefd();
        return true;
    }
    closefd();
    return false;
}

DataUtil::DataUtil()
{
    /* init array */
    sendline = new char[MAXLENGTH];
    buf  = new char[MAXLENGTH];
    inited = init();
}

DataUtil::~DataUtil()
{
    inited = false;
    closefd();
    delete [] sendline;
    delete [] buf;
}

bool DataUtil::login(const QString & name, const QString & password,const QString&type)
{
    if(!inited && !init() ){
        closefd();
        return false;
    }
    QJsonObject objForLog;
    QJsonObject objData;
    QJsonDocument docForlog;
    objData.insert("name",name);
    objData.insert("password",password);
    objForLog.insert(type,objData);
    docForlog.setObject(objForLog);
    if( send(sockfd, docForlog.toJson(QJsonDocument::Compact).toStdString().data(), docForlog.toJson(QJsonDocument::Compact).size(), 0) < 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        closefd();
        return false;
    }
    if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {  // 如果收到服务器返回的信息为 -1 报错，返回读取到数据的长度说明验证成功
        perror("recv error");
        closefd();
        return false;
    }
    buf[rec_len]=0;
    QJsonDocument doc = QJsonDocument::fromJson(buf);
    if(doc.isObject() && doc.object().contains("result")){
        if(doc.object().take("result").toInt()){
            closefd();
            return true;
        }else{
            closefd();
            return false;
        }
    }
    printf("Received : %s\n",buf);
    closefd();
    return false;
}

bool DataUtil::init()
{

    /* create socket (int) */
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){  //sockfd是描述符
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return inited=false;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    /* parse 127.0.0.1 from string to int via AF_INET(ipv4) */
    if( inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0){   //可以在将IP地址在“点分十进制”和“二进制整数”之间转换而且inet_pton
        printf("inet_pton error for %s\n",IP);  			 //和inet_ntop这2个函数能够处理ipv4和ipv6
        closefd();
        return inited=false;
    }

    /* connect to service */
    if( ::connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){  // 连接端口，将描述符和地址内容和大小作为数据 连接服务器
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        closefd();
        return inited=false;
    }
    return inited=true;
}
