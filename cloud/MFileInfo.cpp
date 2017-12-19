#include "MFileInfo.h"

MFileInfo::MFileInfo()
{
}

MFileInfo::MFileInfo(const QString & file, const char * name):type(false)
{
    filepath.append(file);
    filename.append(name);
}

MFileInfo::MFileInfo(const char * file, const QString &name):type(false)
{
    filepath.append(file);
    filename.append(name);
}

MFileInfo::MFileInfo(const char *file, const char *name):type(false)
{
    filename.append(name);
    filepath.append(file);
}

MFileInfo::MFileInfo(const QString &path, const QString &name):type(false)
{
    filepath.append(path);
    filename.append(name);
}

MFileInfo::MFileInfo(const QString & path, const QString &name, bool type):type(type)
{
    filename.append(name);
    filepath.append(path);
}

MFileInfo::MFileInfo(const MFileInfo & file)
{
    filename.append(file.filename);
    filepath.append(file.filepath);
    type = file.type;
}

MFileInfo::~MFileInfo()
{
}

QString MFileInfo::getFileName()
{
    return filename;
}

bool MFileInfo::isDir()
{
    return type;
}

QString MFileInfo::getAbsolutepath()
{
    if(filepath.isEmpty()||filename.isEmpty()){
        return QString(filepath+filename);
    }else{
        return QString(filepath+QDir::separator()+filename);
    }
}

QString MFileInfo::getFilepath()
{
    return filepath;
}

QString MFileInfo::getFileName() const
{
    return filename;
}

bool MFileInfo::isDir()const
{
    return type;
}

QString MFileInfo::getAbsolutepath()const
{
    if(filepath.isEmpty()||filename.isEmpty()){
        return QString(filepath+filename);
    }else{
        return QString(filepath+QDir::separator()+filename);
    }
}

QString MFileInfo::getFilepath()const
{
    return filepath;
}

void MFileInfo::setFileName(const QString &name)
{
    filename.clear();
    filename.append(name);
}

void MFileInfo::setType(bool type)
{
    this->type = type;
}

void MFileInfo::setFilepath(const QString &path)
{
    filepath.clear();
    filepath.append(path);

}
