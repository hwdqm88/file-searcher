#include "kfiledetailsview.h"

#include <QDebug>
#include <QHeaderView>
#include <QToolTip>
#include <QMenu>
#include <QTime>
#include <QProcess>
#include <QFileDialog>

#include "kfilesaver.h"
#include "kfiledeleter.h"

KFileDetailsView::KFileDetailsView(QWidget *parent) :
    QTableView(parent),
    m_model(new KFileDetailsModel)
{
    verticalHeader()->setVisible(false);
    setModel(m_model);
    setSortingEnabled(true);
    setColumnWidth(0, 250);
    setColumnWidth(1, 270);
    setColumnWidth(2, 80);
    setColumnWidth(3, 150);
    sortByColumn(0, Qt::AscendingOrder);

    setMouseTracking(true);

    // 绑定信号实现提示信息
    connect(this, &KFileDetailsView::entered, this, [&](const QModelIndex &index) {
        QToolTip::showText(QCursor::pos(), m_model->data(index).toString());
    });
}

KFileDetailsView::~KFileDetailsView()
{
    delete m_model;
}

KFileDetailsModel *KFileDetailsView::model()
{
    return m_model;
}

QModelIndexList KFileDetailsView::selectedIndexes() const
{
    return QTableView::selectedIndexes();
}

void KFileDetailsView::mousePressEvent(QMouseEvent *event)
{
    // 点击左键选中整行
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = indexAt(event->pos());
        selectRow(index.row());
    }

    // 点击右键弹出菜单
    else if (event->button() == Qt::RightButton)
    {
        QMenu *menu = new QMenu(this);

        QAction *executeAction = new QAction(tr("打开选中的文件/文件夹"), menu);
        QAction *exportAction = new QAction(tr("将选中文件导出到"), menu);
        QAction *openPathAction = new QAction(tr("打开选中文件所在位置"), menu);
        QAction *deleteAction = new QAction(tr("删除选中文件"), menu);
        menu->addAction(executeAction);
        menu->addAction(exportAction);
        menu->addAction(openPathAction);
        menu->addAction(deleteAction);

        connect(executeAction, &QAction::triggered, this, [&]() {
            if (selectedIndexes().isEmpty())
            {
                QMessageBox::critical(this, tr("失败"), tr("请先左键选中文件或文件夹（按住Ctrl可以选择多个）"));
                return;
            }
            foreach(const QFileInfo &fileInfo, m_model->fileInfos(selectedIndexes()))
                QProcess::startDetached("explorer", QStringList() << fileInfo.fileName(),
                                            fileInfo.dir().absolutePath());
        });
        connect(exportAction, &QAction::triggered, this, [&]() {
            if (selectedIndexes().isEmpty())
            {
                QMessageBox::critical(this, tr("失败"), tr("请先左键选中再导出（按住Ctrl可以选择多个）"));
                return;
            }
            QString destPath = QFileDialog::getExistingDirectory(this);
            KFileSaver saver(m_model->fileInfos(selectedIndexes()), destPath, this);
        });
        connect(openPathAction, &QAction::triggered, this, [&]() {
            if (selectedIndexes().isEmpty())
            {
                QMessageBox::critical(this, tr("失败"), tr("请先左键选中文件或文件夹（按住Ctrl可以选择多个）"));
                return;
            }
            foreach(const QFileInfo &fileInfo, m_model->fileInfos(selectedIndexes()))
                QProcess::startDetached("explorer", QStringList() << tr("/select,") << fileInfo.fileName(),
                                        fileInfo.dir().absolutePath());
        });
        connect(deleteAction, &QAction::triggered, this, [&]() {
            if (selectedIndexes().isEmpty())
            {
                QMessageBox::critical(this, tr("失败"), tr("请先左键选中再删除（按住Ctrl可以选择多个）"));
                return;
            }
            KFileDeleter deleter(m_model->fileInfos(selectedIndexes()), this);
        });

        QPoint point = mapToGlobal(event->pos());
        menu->exec(QPoint(point.x(), point.y() + menu->height()));
    }
}
