#ifndef KFILEDELETER_H
#define KFILEDELETER_H

#include <QObject>
#include <QMessageBox>

#include "kfiledeletethread.h"

/**
 * @brief 封装了删除文件的操作
 */
class KFileDeleter : public QObject
{
    Q_OBJECT
public:
    explicit KFileDeleter(const QFileInfoList &fileInfoList, QWidget *parent = nullptr);

private:
    KFileDeleteThread *m_thread;
    QMessageBox *m_waitingMessageBox;
};

#endif // KFILEDELETER_H
