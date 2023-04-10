#include "kfileinfos.h"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QQueue>

KFileInfos *KFileInfos::s_fileInfos = nullptr;

KFileInfos::KFileInfos(QObject *parent) :
    QObject{parent}
{
    updateFileInfos();
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, &KFileInfos::updateFileInfos, Qt::QueuedConnection);
    m_timer.start();
}

KFileInfos::~KFileInfos()
{
    m_timer.stop();
}

const KFileInfos *KFileInfos::instance()
{
    if (!s_fileInfos)
        s_fileInfos = new KFileInfos(qApp);  // 设置parent为qApp，加入Qt对象生命周期管理
    return s_fileInfos;
}

const QFileInfoList &KFileInfos::fileInfoList() const
{
    return m_fileInfoList;
}

void KFileInfos::updateFileInfos()
{
    QDir sourceDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    QFileInfoList fileInfos;
    QFileInfoList totalFileInfos;

    QQueue<QDir> queue;
    queue.push_back(sourceDir);

    // 广度优先搜索遍历全部文件
    while (!queue.empty())
    {
        sourceDir = queue.dequeue();
        fileInfos = sourceDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
        totalFileInfos.append(fileInfos);
        foreach (const QFileInfo &fileInfo, fileInfos)
            if (fileInfo.isDir())
                queue.push_back(QDir(fileInfo.absoluteFilePath()));
    }
    if (totalFileInfos == m_fileInfoList)
        return;

    m_fileInfoList = totalFileInfos;
    emit filesChanged();
}
