#include "kfilesaver.h"

#include <QPushButton>

KFileSaver::KFileSaver(const QFileInfoList &fileInfoList, const QString &destDirPath, QWidget *parent) :
    QObject(parent),
    m_thread(new KFileSaveThread(fileInfoList, destDirPath, this)),
    m_waitingMessageBox(new QMessageBox(QMessageBox::NoIcon, tr("导出中"), tr("正在导出，请稍后"), QMessageBox::Cancel, parent))
{
    // 绑定信号，实现终止导出
    connect(m_waitingMessageBox, &QMessageBox::buttonClicked, this, [&]() {
        if (m_thread->isRunning())
            m_thread->exit();
    });

    // 绑定信号，实现保存结束之后关闭提示框
    connect(m_thread, &KFileSaveThread::finished, this, [&]() {
        m_waitingMessageBox->close();
        QMessageBox *successMessageBox = new QMessageBox(QMessageBox::NoIcon, tr("完成"), tr("导出成功"), QMessageBox::Ok, parent);
        successMessageBox->exec();
    });

    m_thread->start();
    m_waitingMessageBox->exec();
}
