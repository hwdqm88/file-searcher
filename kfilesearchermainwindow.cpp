#include "kfilesearchermainwindow.h"
#include "ui_kfilesearchermainwindow.h"

#include <QDebug>
#include <QQueue>
#include <QFileDialog>
#include <QPushButton>

#include "kfilesaver.h"
#include "kfiledeleter.h"
#include "kkeymonitor.h"

KFileSearcherMainWindow::KFileSearcherMainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::KFileSearcherMainWindow),
        m_isRegExpEnable(false)
{
    ui->setupUi(this);

    connect(ui->m_inputLineEdit, &QLineEdit::textEdited,
            this, &KFileSearcherMainWindow::onFilterChanged, Qt::QueuedConnection);
    connect(ui->m_filter_ComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &KFileSearcherMainWindow::onFilterChanged, Qt::QueuedConnection);
    connect(ui->m_detailsView->model(), &KFileDetailsModel::dataChanged, this, [&]() {
        ui->m_statusbar->showMessage(QString(tr("%1个对象")).arg(ui->m_detailsView->model()->rowCount()));
    });
    ui->m_statusbar->showMessage(QString(tr("%1个对象")).arg(ui->m_detailsView->model()->rowCount()));

    // 绑定信号，实现快捷键隐藏和显示
    connect(KKeyMonitor::instance(), &KKeyMonitor::hideShortcutPressed,
            this, &KFileSearcherMainWindow::on_m_hideAction_triggered);
}

KFileSearcherMainWindow::~KFileSearcherMainWindow()
{
    delete ui;
}

void KFileSearcherMainWindow::onFilterChanged()
{
    KFileFilter filter;
    filter.setPattern(ui->m_inputLineEdit->text());
    filter.setIsRegExpEnabled(m_isRegExpEnable);
    switch (ui->m_filter_ComboBox->currentIndex()) {
    case 0:
        filter.setFilterType(KFileFilter::None);
        break;
    case 1:
        filter.setFilterType(KFileFilter::Dir);
        break;
    case 2:
        filter.setFilterType(KFileFilter::File);
        break;
    default:
        break;
    }
    ui->m_detailsView->model()->installFilter(filter);
}

void KFileSearcherMainWindow::on_m_regExpAction_triggered(bool checked)
{
    m_isRegExpEnable = checked;
    onFilterChanged();
}

void KFileSearcherMainWindow::on_m_quitAction_triggered()
{
    close();
}

void KFileSearcherMainWindow::on_m_exportAction_triggered()
{
    if (ui->m_detailsView->selectedIndexes().isEmpty())
    {
        QMessageBox::critical(this, tr("失败"), tr("请先左键选中再导出（按住Ctrl可以选择多个）"));
        return;
    }
    QString destPath = QFileDialog::getExistingDirectory(this);
    KFileSaver saver(ui->m_detailsView->model()->fileInfos(ui->m_detailsView->selectedIndexes()), destPath, this);
}

void KFileSearcherMainWindow::on_m_selectAllAction_triggered()
{
    ui->m_detailsView->selectAll();
}

void KFileSearcherMainWindow::on_m_deleteAction_triggered()
{
    if (ui->m_detailsView->selectedIndexes().isEmpty())
    {
        QMessageBox::critical(this, tr("失败"), tr("请先左键选中再删除（按住Ctrl可以选择多个）"));
        return;
    }
    KFileDeleter deleter(ui->m_detailsView->model()->fileInfos(ui->m_detailsView->selectedIndexes()), this);
}

void KFileSearcherMainWindow::on_m_hideAction_triggered()
{
    if (isHidden())
        show();
    else
        hide();
}
