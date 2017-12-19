#ifndef DBUTILS_H
#define DBUTILS_H
#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>
#include <IPInterface.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QString>
#include <QDebug>
#include <QJsonArray>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
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
#include <mysql/mysql.h>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileIconProvider>
#include <QBuffer>
#include <QFileDialog>
#include <JSONUtils.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

const char * login = "login";
const char * regist= "regist";
const char * correctpwd="correctpwd";
const char * creatdirectory ="creatdirectory";
const char * uploadfile ="uploadfile";
const char * downloadfile ="downloadfile";
const char * directory = "directory";
const char * removefile = "removefile";
const char * renameFile = "renamefile";

typedef enum RequestType{
    LOGIN,
    REGIST,
    CREATEDIRECTORY,
    CORRECTPWD,
    UPLOADFILE,
    DOWNLOADFILE,
    DIRECOTRY,
    REMOVEFILE,
    RENAMEFILE,
    ERROR
}RequestType;

typedef struct DATA{
    RequestType type;
    QString json;
}DATA,*PDATA;

bool openSQL(MYSQL *my_connection);
bool userExist(const QString &buff,MYSQL my_connection);
bool onLogin(const QString &buff,MYSQL* my_connection);
bool insertUser(const QString & buff,MYSQL *my_connection);
bool updateUser(const QString & buff,MYSQL *my_connection);
bool query(const char *sql,const MYSQL_BIND*params);
bool createDir(const QString & path);
void removeFile(const char * path);
bool onRename(const QString &json);

DATA analyze(const char * buff);

DATA analyze(const char * buff){
    DATA temp;
    temp.type = ERROR;
    QJsonParseError error;
    QJsonDocument json=QJsonDocument::fromJson(buff,&error);
    json.toJson(QJsonDocument::Compact);
    if(error.error == QJsonParseError::NoError){
        if(json.isObject()){
            QJsonObject obj = json.object();
            if(obj.contains(login)){
                QJsonValue value = obj.take(login);
                temp.type = LOGIN;
                QJsonDocument doc;
                doc.setObject(value.toObject());
                temp.json.clear();
                temp.json.append(doc.toJson(QJsonDocument::Compact));
                return temp;
            }
            if(obj.contains(regist)){
                QJsonValue value = obj.take(regist);
                temp.type = REGIST;
                QJsonDocument doc;
                doc.setObject(value.toObject());
                temp.json.clear();
                temp.json.append(doc.toJson(QJsonDocument::Compact));
                return temp;
            }
            if(obj.contains(correctpwd)){
                QJsonValue value = obj.take(correctpwd);
                temp.type = CORRECTPWD;
                QJsonDocument doc;
                doc.setObject(value.toObject());
                temp.json.clear();
                temp.json.append(doc.toJson(QJsonDocument::Compact));
                return temp;
            }
            if(obj.contains(renameFile)){
                temp.type = RENAMEFILE;
                temp.json.clear();
                temp.json.append(buff);
                return temp;
            }
            if(obj.contains(creatdirectory)){
                QJsonValue value = obj.take(creatdirectory);
                temp.type = CREATEDIRECTORY;
                temp.json.clear();
                temp.json.append("/home/rubikchen/"+value.toString());
                temp.json.replace("//","/");
                return temp;
            }
            if(obj.contains(uploadfile)){
                QJsonValue value = obj.take(uploadfile);
                temp.type = UPLOADFILE;
                temp.json.clear();
                temp.json.append("/home/rubikchen/"+value.toString());
                temp.json.replace("//","/");
                return temp;
            }
            if(obj.contains(removefile)){
                QJsonValue value = obj.take(removefile);
                temp.type = REMOVEFILE;
                temp.json.clear();
                temp.json.append("/home/rubikchen/"+value.toString());
                temp.json.replace("//","/");
                return temp;
            }
            if(obj.contains(downloadfile)){
                QJsonValue value = obj.take(downloadfile);
                temp.type = DOWNLOADFILE;
                temp.json.clear();
                temp.json.append("/home/rubikchen/"+value.toString());
                temp.json.replace("//","/");
                return temp;
            }
            if(obj.contains(directory)){
                QJsonValue value = obj.take(directory);
                QJsonObject infos =  value.toObject();
                QString user = infos.take("user").toString();
                QString path = infos.take("path").toString();
                temp.json.clear();
                temp.json.append(QString("/home/rubikchen/") +
                                 user+"/"+path);
                if(path.length())
                    temp.json.append("/");
                temp.type = DIRECOTRY;
                return temp;
            }
            printf("wrong0\n");
            return temp;
        }else {
            printf("wrong1\n");
            return temp;
        }
    }else {
        printf("wrong2\n");
        return temp;
    }
}


bool query(const char *sql,MYSQL_BIND*params,MYSQL* my_connection){
    MYSQL_STMT * preparestatment = mysql_stmt_init(my_connection);
    int prepare_result = mysql_stmt_prepare(preparestatment, sql,
                                            strlen(sql));
    if( 0 == prepare_result ){
        mysql_stmt_bind_param(preparestatment, params);
        mysql_stmt_execute(preparestatment);
        mysql_stmt_store_result(preparestatment);
        int rows = mysql_stmt_affected_rows(preparestatment);
        mysql_stmt_close(preparestatment);
        return rows>0;
    }else {
        printf("prepare error:%s",strerror(errno));
        return false;
    }
}

bool userExist(const QString &buff,MYSQL* my_connection)
{

    char *sql = "select * from stu where name=?;";
    MYSQL_BIND params[2];
    char pwd[21];
    char name[21];
    strncpy(name,buff.toLatin1().data(),20);
    memset(params, 0, sizeof(params));
    params[0].buffer_type = MYSQL_TYPE_STRING;
    params[0].buffer = name;
    params[0].buffer_length = strlen(name);
    params[0].is_null=0;
    params[1].buffer_type = MYSQL_TYPE_STRING;
    params[1].buffer = pwd;
    params[1].buffer_length = sizeof(pwd);
    params[1].is_null=0;
    return query(sql,params,my_connection);
}

bool onLogin(const QString &buff,MYSQL* my_connection)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(buff.toLatin1(),&error);
    if(error.error != QJsonParseError::NoError){
        printf("error:%s",strerror(errno));
        return false;
    }
    QJsonObject obj = doc.object();
    char *sql = "select * from stu where name=? and pwd=?;";
    MYSQL_BIND params[2];
    char pwd[21];
    char name[21];
    strncpy(name,obj.take("name").toString().toLatin1().data(),20);
    strncpy(pwd,obj.take("password").toString().toLatin1().data(),20);
    memset(params, 0, sizeof(params));
    params[0].buffer_type = MYSQL_TYPE_STRING;
    params[0].buffer = name;
    params[0].buffer_length = strlen(name);
    params[0].is_null=0;
    params[1].buffer_type = MYSQL_TYPE_STRING;
    params[1].buffer = pwd;
    params[1].buffer_length = strlen(pwd);
    params[1].is_null=0;
    return query(sql,params,my_connection);
}

bool insertUser(const QString & buff,MYSQL *my_connection)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(buff.toLatin1(),&error);
    if(error.error != QJsonParseError::NoError){
        printf("error:%s",strerror(errno));
        return false;
    }
    QJsonObject obj = doc.object();
    QString tempname = obj.take("name").toString();
    QString temppwd = obj.take("password").toString();
    char *sql = "insert into stu(name,pwd)values(?,?);";
    if(userExist(tempname,my_connection)){
        return false;
    }
    MYSQL_BIND params[2];
    char pwd[21]={0};
    char name[21]={0};
    strcpy(name,tempname.toLatin1().data());
    strcpy(pwd,temppwd.toLatin1().data());
    printf("name %s pwd %s\n",name,pwd);
    memset(params, 0, sizeof(params));
    params[0].buffer_type = MYSQL_TYPE_STRING;
    params[0].buffer = name;
    params[0].buffer_length = strlen(name);
    params[0].is_null=0;
    params[1].buffer_type = MYSQL_TYPE_STRING;
    params[1].buffer = pwd;
    params[1].buffer_length = strlen(pwd);
    params[1].is_null=0;
    bool flag = query(sql,params,my_connection);
    if(flag){
        mkdir(QString(QString("/home/rubikchen/")+QString(name)).toUtf8().toStdString().data(),0777);
    }
    return flag;
}

bool updateUser(const QString &buff,MYSQL *my_connection)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(buff.toLatin1(),&error);
    if(error.error != QJsonParseError::NoError){
        printf("error:%s",strerror(errno));
        return false;
    }
    QJsonObject obj = doc.object();
    QString tempname = obj.take("name").toString();
    QString temppwd = obj.take("password").toString();
    char *sql = "update stu set pwd=? where name=?;";
    if(!userExist(tempname,my_connection)){
        return false;
    }
    MYSQL_BIND params[2];
    char pwd[21]={0};
    char name[21]={0};
    strcpy(name,tempname.toLatin1().data());
    strcpy(pwd,temppwd.toLatin1().data());
    memset(params, 0, sizeof(params));
    params[0].buffer_type = MYSQL_TYPE_STRING;
    params[0].buffer = pwd;
    params[0].buffer_length = strlen(pwd);
    params[0].is_null=0;
    params[1].buffer_type = MYSQL_TYPE_STRING;
    params[1].buffer = name;
    params[1].buffer_length = strlen(name);
    params[1].is_null=0;
    return query(sql,params,my_connection);
}

bool upLoad(const QString &filePath){
    /*
    mkdir(path.toLatin1().data(),0777);
    fp = fopen(filePath.toLatin1().data(),"wb+");
    */

    printf("DBUtils:upload=filepath is %s .\n",filePath.toUtf8().toStdString().data());

    FILE * fp = fopen(filePath.toUtf8().toStdString().data(),"r");
    if(NULL != fp){
        printf("DBUtils:upload=filepath no avaiable .\n");
        return false;
    }
    return true;
}


QString getDirectory(const QString& path){
    QFileInfo file(path);
    QFileInfoList infoList ;
    QString realpath = file.absolutePath();
    if(realpath.split("/").size()>4){
        infoList= QDir(realpath).
                entryInfoList( QDir::AllEntries | QDir::NoDot,QDir::DirsFirst|QDir::Type);
    }else{
        infoList= QDir(realpath).
                entryInfoList(QDir::NoDotDot | QDir::AllEntries,QDir::DirsFirst|QDir::Type);

    }
    printf("dbutils getdirecotry %d\n",infoList.size());
    QJsonArray jsonarray;
    QJsonDocument doc;
    if(infoList.size()>0){
        int j,k;
        QString str = infoList.at(0).filePath();
        for(k=j=0;j<4;k++)
            if(str.at(k)==QDir::separator())
                j++;
        for(int i  = 0 ;i < infoList.size() ; i++){
            QJsonObject obj;
            obj.insert("path",infoList.at(i).path().remove(0,k));
            obj.insert("name",infoList.at(i).fileName());
            obj.insert("type",infoList.at(i).isDir());
            jsonarray.insert(i,obj);
        }
    }
    doc.setArray(jsonarray);
    return QString(doc.toJson(QJsonDocument::Compact));
}


bool openSQL(MYSQL *my_connection)
{
    /* my_connection 这是一个数据库连接*/
    /*初始化mysql连接my_connection*/
    mysql_init(my_connection);
    if (NULL != mysql_real_connect(my_connection,   HOST,   USERNAME,   PASSWORD,
                                   DATABASE, 0, NULL, CLIENT_FOUND_ROWS))  /*连接成功*/
    {
        printf("数据库查询query_sql连接成功！\n");
        /*设置查询编码为gbk，以支持中文*/
        //mysql_query(my_connection, "set names utf-8");
        return true;
    }
    return false;
}

bool createDir(const QString & path){
    return mkdir(path.toUtf8().toStdString().data(),0777)!=-1;
}
void removeFile(const char * path){
    dirent * dirt;
    DIR * dir;
    if((dir= opendir(path) ) != NULL){
        while((dirt=readdir(dir))){
            if(strcmp(dirt->d_name,".")&strcmp(dirt->d_name,"..")){
                removeFile(QString(QString(path)+"/"+QString(dirt->d_name)).toUtf8().toStdString().data());
            }
        }
    }
    remove(path);
    closedir(dir);
}

bool onRename(const QString &json){
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonArray array = doc.object().take(renameFile).toArray();
    QString from ="/home/rubikchen/"+array.at(1).toObject().take("from").toString();
    QString to ="/home/rubikchen/"+array.at(0).toObject().take("to").toString();
    return rename(from.replace("//","/").toUtf8().toStdString().data(),
                  to.replace("//","/").toUtf8().toStdString().data())!=-1;
}

#endif // DBUTILS_H
