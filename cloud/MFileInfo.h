#ifndef MFILEINFO_H
#define MFILEINFO_H

#include <QString>
#include <QByteArray>
#include <QDir>

class MFileInfo
{
public:
    MFileInfo();
    MFileInfo(const QString&,const char *);
    MFileInfo(const char *,const QString&);
    MFileInfo(const char *,const char*);
    MFileInfo(const QString&,const QString&);
    MFileInfo(const QString&,const QString&,bool);
    MFileInfo(const MFileInfo&);
    virtual ~MFileInfo();
    virtual QString getFileName();
    virtual bool isDir();
    virtual QString getAbsolutepath();
    virtual QString getFilepath();
    virtual QString getFileName()const;
    virtual bool isDir()const;
    virtual QString getAbsolutepath()const;
    virtual QString getFilepath()const;
    virtual void setFileName(const QString &);
    virtual void setType(bool);
    virtual void setFilepath(const QString &);
protected:
    QString  filename;
    QString  filepath;
    bool type;
};

#endif // MFILEINFO_H
