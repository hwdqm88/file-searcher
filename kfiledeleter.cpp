#include "kfiledeleter.h"

#include <QPushButton>

KFileDeleter::KFileDeleter(const QFileInfoList &fileInfoList, QWidget *parent) :
    QObject(parent),
    m_thread(new KFileDeleteThread(fileInfoList, this)),
    m_waitingMessageBox(new QMessageBox(QMessageBox::NoIcon, tr("删除中"), tr("正在删除，请稍后"), QMessageBox::Cancel, parent))
{
    // 绑定信号，实现终止删除
    connect(m_waitingMessageBox, &QMessageBox::buttonClicked, this, [&]() {
        if (m_thread->isRunning())
            m_thread->exit();
    });

    // 绑定信号，实现删除结束之后关闭提示框
    connect(m_thread, &KFileDeleteThread::finished, this, [&]() {
        m_waitingMessageBox->close();
        QMessageBox *successMessageBox = new QMessageBox(QMessageBox::NoIcon, tr("完成"), tr("删除成功"), QMessageBox::Ok, parent);
        successMessageBox->exec();
    });

    m_thread->start();
    m_waitingMessageBox->exec();
}
