#ifndef KFILESAVETHREAD_H
#define KFILESAVETHREAD_H

#include <QThread>
#include <QDir>
#include <QFileInfo>

/**
 * @brief 将文件保存单独放在一个线程中，防止保存文件阻塞GUI线程导致界面卡住
 */
class KFileSaveThread : public QThread
{
    Q_OBJECT
public:
    KFileSaveThread(QObject *parent = nullptr);

    KFileSaveThread(const QFileInfoList &fileInfoList, const QString &destDirPath, QObject *parent = nullptr);

    virtual void run() override;

    void setFileInfoList(const QFileInfoList &newFileInfoList);

    void setDestDirPath(const QString &newDestDirPath);

private:
    /**
     * @brief copyDirRecursively 复制文件夹，通过递归的方式吧内部所有内容也一起复制
     * @param sourceDir 被复制的文件夹
     * @param destDir 目标文件夹
     */
    void copyDirRecursively(const QDir &sourceDir, QDir destDir);

    QFileInfoList m_fileInfoList;
    QString m_destDirPath;
};

#endif // KFILESAVETHREAD_H
