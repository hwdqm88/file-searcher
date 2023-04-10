#include "kfilesavethread.h"


KFileSaveThread::KFileSaveThread(QObject *parent) : QThread(parent)
{
}

KFileSaveThread::KFileSaveThread(const QFileInfoList &fileInfoList, const QString &destDirPath, QObject *parent) :
    QThread(parent),
    m_fileInfoList(fileInfoList),
    m_destDirPath(destDirPath)
{
}

void KFileSaveThread::run()
{
    foreach(const QFileInfo &fileInfo, m_fileInfoList)
    {
        if (fileInfo.isFile())
            QFile::copy(fileInfo.absoluteFilePath(), m_destDirPath + '\\' + fileInfo.fileName());
        else
            copyDirRecursively(QDir(fileInfo.absoluteFilePath()), QDir(m_destDirPath));
    }
}

void KFileSaveThread::copyDirRecursively(const QDir &sourceDir, QDir destDir)
{
    destDir.mkdir(sourceDir.dirName());
    destDir.cd(sourceDir.dirName());
    foreach(const QFileInfo &fileInfo, sourceDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
    {
        if (fileInfo.isFile())
            QFile::copy(fileInfo.absoluteFilePath(), destDir.absolutePath() + '\\' + fileInfo.fileName());
        else
            copyDirRecursively(QDir(fileInfo.absoluteFilePath()), destDir);
    }
}

void KFileSaveThread::setDestDirPath(const QString &newDestDirPath)
{
    m_destDirPath = newDestDirPath;
}

void KFileSaveThread::setFileInfoList(const QFileInfoList &newFileInfoList)
{
    m_fileInfoList = newFileInfoList;
}
