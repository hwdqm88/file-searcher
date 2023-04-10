#ifndef KFILESAVER_H
#define KFILESAVER_H

#include <QObject>
#include <QMessageBox>

#include "kfilesavethread.h"

/**
 * @brief 封装了文件保存的操作
 */
class KFileSaver : public QObject
{
    Q_OBJECT
public:
    explicit KFileSaver(const QFileInfoList &fileInfoList, const QString &destDirPath, QWidget *parent = nullptr);

private:
    KFileSaveThread *m_thread;
    QMessageBox *m_waitingMessageBox;
};

#endif // KFILESAVER_H
