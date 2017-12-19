#ifndef DATAUTIL_H
#define DATAUTIL_H

#include <includes.h>
#include <MDirectoryItem.h>
#include <MFileItem.h>
static const int MAXLENGTH=1024*16;
class DataUtil
{
public:
    DataUtil();
    ~DataUtil();
    bool login(const QString&,const QString&,const QString&);
    bool closefd();
    QList<MFileInfo> *requestFile(const QString&,const QString &);
    QList<MFileInfo> *requestFile(const MFileInfo&,const QString &);
    bool download(const QString &);
    bool downloadFile(const QString& filepath,const QString &);
    bool upload(const MFileInfo&, const QString&,const QString &);
    bool uploadFile(const MFileInfo&, const QFileInfo &);
    bool remove(const QString&);
    bool rename(const QString&,const QString&);
    bool createDir(const MFileInfo&,const QString&,const QString&);
private:
    bool init();
    bool inited;
    int sockfd;
    int rec_len;
    char*sendline;
    char*buf;
    struct sockaddr_in  servaddr;
};
/*
int DataUtil::sockfd=-1;
int DataUtil::rec_len=-1;
char*DataUtil::sendline=NULL;
char*DataUtil::buf=NULL;
struct sockaddr_in  DataUtil::servaddr={0};
*/
#endif // DATAUTIL_H
