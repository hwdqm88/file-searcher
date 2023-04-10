#ifndef KFILEDELETETHREAD_H
#define KFILEDELETETHREAD_H

#include <QThread>
#include <QFileInfo>

/**
 * @brief 将文件删除单独放在一个线程中，防止保存文件阻塞GUI线程导致界面卡住
 */
class KFileDeleteThread : public QThread
{
    Q_OBJECT
public:
    explicit KFileDeleteThread(QObject *parent = nullptr);

    explicit KFileDeleteThread(const QFileInfoList &fileInfoList, QObject *parent = nullptr);

    virtual void run() override;

    void setFileInfoList(const QFileInfoList &newFileInfoList);

private:
    QFileInfoList m_fileInfoList;
};

#endif // KFILEDELETETHREAD_H
