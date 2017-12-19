#include <QApplication>
#include <stdio.h>
#include "DBUtils.h"
#include <mysql/mysql.h>
#include <string.h>
#include <IPInterface.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include<QDebug>
#include<iostream>
#include <QJsonArray>
#include <JSONUtils.h>
#include <pthread.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <QFileDialog>

#define PORT 8000
static const int MAXSIZE = 1024*16;

MYSQL   myconnection;
char buff[MAXSIZE];
int client_fd=-1,server_fd=-1;
int bind_result=-1,acceptsize=-1,listen_result=-1;
struct sockaddr_in clientaddr,serveraddr;
socklen_t clientLen;

bool uploadFile(const QString &filepath);
bool download(const QString &filepath);


int main(int argc,char *argv[])
{
    QApplication a(argc, argv);

    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == server_fd){
        printf("create socket error:%s",strerror(errno));
        return 0;
    }
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family= AF_INET;
    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);

    bind_result = bind(server_fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(0>bind_result){
        printf("bind error:%s",strerror(errno));
        return 0;
    }
    printf("service running:\b");
    /* 设置监听 */
    listen_result = listen(server_fd, 2);
    if (0 > listen_result)
    {
        printf("listen error: %s\n",strerror(errno));
        return -1;
    }
    memset(buff,0,MAXSIZE);
    clientLen=sizeof(clientaddr);
    QByteArray data;
    printf("client fd:%d\n",client_fd);
    while(1){
        client_fd = accept(server_fd,(struct sockaddr*)&clientaddr,&clientLen);
        if(-1==client_fd){
            printf("accept error: %s\n",strerror(errno));
            continue;
        }
        memset(buff,0,sizeof(buff));
        int n = recv(client_fd, buff, sizeof(buff), 0);
        buff[n]=0;
        printf("server received %d\n%s\n",n,buff);
        if( 0 > n ){
            printf("receive error:%s\n",strerror(errno));
            if(errno!=EAGAIN)
                break;
        }else if( 0 == n ){
            printf("client closed\n");
            //::close(client_fd);
            //return 0;
        }else if(n!=sizeof(buff)){
            openSQL(&  myconnection);
            DATA temp = analyze(buff);
            int flag= false;
            switch (temp.type) {
            case LOGIN:
                flag = onLogin(temp.json,&myconnection);
                break;
            case REGIST:
                flag = insertUser(temp.json,&myconnection);
                break;
            case CORRECTPWD:
                flag = updateUser(temp.json,&myconnection);
            case UPLOADFILE:
                if(upLoad(temp.json)){
                    uploadFile(temp.json);
                    goto tobecontinue;
                }else{
                    flag = false;
                }
                break;
            case DIRECOTRY:
                data.clear();
                data.append(getDirectory(temp.json).toUtf8());
                goto out;
                break;
            case DOWNLOADFILE:
                if(download(temp.json))
                    goto tobecontinue;
                break;
            case CREATEDIRECTORY:
                flag = createDir(temp.json);
                break;
            case REMOVEFILE:
                flag = true;
                removeFile(temp.json.toUtf8().toStdString().data());
                break;
            case RENAMEFILE:
                flag = onRename(temp.json);
                break;
            default:
                goto tobecontinue;
            }
            data.clear();
            data.append(JSONUtils::Wrap("result",flag).toUtf8());
out:
            acceptsize = ::send(client_fd,data.toStdString().data(),
                                data.size(),0);
            if (acceptsize < 0)
            {
                printf("send error: %s\n",strerror(errno));
                continue;
            }
tobecontinue:;
        }
    }
    ::close(server_fd);

    return a.exec();
}

bool uploadFile(const QString &uploadPath){

    QString json = JSONUtils::Wrap("result",true);
    if(::send(client_fd,json.toStdString().data(),json.size(),0)<0){
        printf("send error: %s\n",strerror(errno));
        return false;
    }

    printf("main:uploadFile=ready to upload %s\n",uploadPath.toUtf8().toStdString().data());
    FILE * fp = fopen(uploadPath.toUtf8().toStdString().data(),"wb+");
    if(fp==NULL){
        printf("main:uploadFile=openFailed\n");
        return false;
    }
    int persize = 1024*4;
    printf("main:uploadFile=start to upload\n");
    /*


    client_fd = accept(server_fd,(struct sockaddr*)&clientaddr,&clientLen);
    if(-1==client_fd){
        printf("accept error: %s\n",strerror(errno));
        fclose(fp);
        return false;
    }*/
    int size = 0;
    while(1){
        memset(buff,0,sizeof(buff));
        int n = recv(client_fd, buff, sizeof(buff), 0);
        buff[n]=0;
        size += n;
        for(int i=0;i<n;i++)
            fputc(buff[i],fp);
        if(::send(client_fd,"1",2,0)<0){
            printf("send error: %s\n",strerror(errno));
            return false;
        }
        if( n != persize )
            break;
    }
    printf("main:uploadFile=upload end %d\n",size);
    fclose(fp);
    return true;
}

bool download(const QString &filepath){

    QString json = JSONUtils::Wrap("result",QJsonValue(true) );
    if(::send(client_fd,json.toUtf8().toStdString().data(),json.size(),0)<0){
        printf("send error: %s\n",strerror(errno));
        return false;
    }
    printf("return info %s\n",json.toLocal8Bit().data());
    FILE * fp = fopen(filepath.toUtf8().toStdString().data(),"rb+");
    if(NULL == fp){
        printf("main:download=open filed %s\n",filepath.toUtf8().toStdString().data());
        return false;
    }
    printf("main:download=%s.\n",filepath.toUtf8().toStdString().data());
    fseek(fp,0,SEEK_END);
    int size = ftell(fp),persize=1024*4,t,sended=0;
    fseek(fp,0,SEEK_SET);
    printf("size %d\n",size);
    /**/
    client_fd = accept(server_fd,(struct sockaddr*)&clientaddr,&clientLen);
    if(-1==client_fd){
        printf("accept error: %s\n",strerror(errno));
        fclose(fp);
        return false;
    }
    int n;
    printf("main:download=ready\n");
    while(1){
        if(recv(client_fd, buff, sizeof(buff), 0)<=0){
            printf("recv error\n");
            fclose(fp);
            return false;
        }
        t=sended+persize<size?persize:size-sended;
        for(int i=0;i<t;i++)
            buff[i]=fgetc(fp);
        buff[t]=0;
        sended += persize;

        if((n=send(client_fd,buff,t,0))<0){
            printf("send error: %s\n",strerror(errno));
            fclose(fp);
            return false;
        }
        if( t != persize )
            break;
    }
    printf("main:download=upload end %d\n",size);
    fclose(fp);
    return true;
}

void dbtoarray(){
    /*
    // for store data
    QJsonObject simp_ayjson;
    simp_ayjson.insert("name", "LLH");
    simp_ayjson.insert("age", 20);
    simp_ayjson.insert("myAccounts", QString::fromLocal8Bit("123"));

    QJsonDocument document;
    document.setObject(simp_ayjson);
    // data to json
    QByteArray simpbyte_array = document.toJson(QJsonDocument::Compact);
    //json to byte array
    QString simpjson_str(simpbyte_array);
    qDebug()<< simpjson_str;
    */
    QJsonObject data;
    data.insert("name","xiaoming");
    data.insert("id",1);
    QJsonDocument jsondata;
    jsondata.setObject(data);
    bool parsestatu;
    QJsonParseError error;
    QJsonDocument document=QJsonDocument::fromJson(jsondata.toJson(),&error);
    if(error.error == QJsonParseError::NoError){
        std::cout<<document.toJson().toStdString();
    }
}

void query_sql(const char* sql)
{
    MYSQL my_connection; /*这是一个数据库连接*/
    int res; /*执行sql語句后的返回标志*/
    MYSQL_RES *res_ptr; /*指向查询结果的指针*/
    MYSQL_FIELD *field; /*字段结构指针*/
    MYSQL_ROW result_row; /*按行返回的查询信息*/
    int row, column; /*查询返回的行数和列数*/
    int i, j;
    /*初始化mysql连接my_connection*/
    mysql_init(&my_connection);
    /*建立mysql连接*/
    if (NULL != mysql_real_connect(&my_connection,  HOST,  USERNAME,  PASSWORD,
                                   DATABASE, 0, NULL, CLIENT_FOUND_ROWS))  /*连接成功*/
    {
        printf("数据库查询query_sql连接成功！\n");
        /*设置查询编码为gbk，以支持中文*/
        mysql_query(&my_connection, "set names utf-8");
        res = mysql_query(&my_connection, sql);
        if (res)   /*执行失败*/
        {
            printf("Error： mysql_query !\n");
            /*关闭连接*/
            mysql_close(&my_connection);
        }
        else     /*现在就代表执行成功了*/
        {
            /*将查询的結果给res_ptr*/
            res_ptr = mysql_store_result(&my_connection);
            /*如果结果不为空，就把结果print*/
            if (res_ptr)
            {
                /*取得結果的行数和*/
                column = mysql_num_fields(res_ptr);
                row = mysql_num_rows(res_ptr);
                printf("查询到 %d 行 \n", row);
                /*输出結果的字段名*/
                QJsonArray fieldsName;
                for (i = 0; field = mysql_fetch_field(res_ptr); i++){
                    QJsonObject obj;
                    obj.insert("column",field->name);
                    fieldsName.insert(i,obj);
                }
                QJsonDocument document;
                document.setArray(fieldsName);
                //    column name
                //    printf("%10s ", field->name);
                printf("\n");
                /*按行输出結果*/
                QJsonArray datas;
                for (i = 1; i < row+1; i++)
                {
                    result_row = mysql_fetch_row(res_ptr);
                    QJsonObject obj;
                    obj.insert("name",result_row[0]);
                    obj.insert("stunum",result_row[1]);
                    datas.insert(i-1,obj);
                    //for (j = 0; j < column; j++)

                    //    printf("%10s ", result_row[j]);
                    //printf("\n");
                }
                //QJsonDocument docDatas;
                //docDatas.setArray(datas);
                QJsonObject all;
                all.insert("names",fieldsName);
                all.insert("data",datas);
                QJsonDocument alldoc;
                alldoc.setObject(all);
                std::cout<< alldoc.toJson(QJsonDocument::Compact).toStdString();

                std::cout<< "start parse"<<std::endl;
                QByteArray jsonarray = alldoc.toJson(QJsonDocument::Compact);
                QString json(jsonarray);
                std::cout<<json.toStdString()<<std::endl;
                QJsonParseError error;
                QJsonDocument parse=QJsonDocument::fromJson(json.toUtf8(),&error);
                if(error.error == QJsonParseError::NoError){
                    if(parse.isObject()){
                        QJsonObject obj = parse.object();
                        if(obj.contains("names")){
                            QJsonValue value = obj.take("names");
                            if(value.isArray()){
                                QJsonArray names = value.toArray();
                                std::cout<<"name:"<<names.size()<<std::endl;
                                for(int i = 0 ; i < names.size() ; i++){
                                    std::cout<<names.at(i).toObject().take("column").toString().toStdString()
                                            << std::endl;
                                }
                            }
                        }
                        if(obj.contains("data")){
                            QJsonValue value = obj.take("data");
                            if(value.isArray()){
                                QJsonArray names = value.toArray();
                                for(int i = 0 ; i < names.size() ; i++){
                                    std::cout<<names.at(i).toObject().take("name").toString().toStdString()
                                            << names.at(i).toObject().take("stunum").toString().toStdString()<<std::endl;
                                }
                            }
                        }
                    }else {
                        std::cout<<"not an object";
                    }
                }
                else {
                    std::cout<<"parse error:"<<error.error;
                }
            }
            /*不要忘了关闭连接*/
            mysql_close(&my_connection);
        }
    }
    else
    {
        printf("数据库连接失败");
    }
}

