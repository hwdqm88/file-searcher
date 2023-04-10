#include "kfiledeletethread.h"

#include <QDir>

KFileDeleteThread::KFileDeleteThread(QObject *parent) : QThread(parent)
{
}

KFileDeleteThread::KFileDeleteThread(const QFileInfoList &fileInfoList, QObject *parent) :
    QThread(parent),
    m_fileInfoList(fileInfoList)
{
}

void KFileDeleteThread::run()
{
    foreach(const QFileInfo &fileInfo, m_fileInfoList)
    {
        if (fileInfo.isFile())
            QFile::remove(fileInfo.absoluteFilePath());
        else
            QDir(fileInfo.absoluteFilePath()).removeRecursively();
    }
}

void KFileDeleteThread::setFileInfoList(const QFileInfoList &newFileInfoList)
{
    m_fileInfoList = newFileInfoList;
}
