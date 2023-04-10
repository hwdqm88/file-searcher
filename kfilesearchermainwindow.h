#ifndef KFILESEARCHER_KFILESEARCHERMAINWINDOW_H
#define KFILESEARCHER_KFILESEARCHERMAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "kfilesavethread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class KFileSearcherMainWindow; }
QT_END_NAMESPACE

/**
 * @brief 文件搜索器的主窗口类
 */
class KFileSearcherMainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit KFileSearcherMainWindow(QWidget *parent = nullptr);

    ~KFileSearcherMainWindow() override;

private slots:
    void on_m_regExpAction_triggered(bool checked);

    void on_m_quitAction_triggered();

    void on_m_exportAction_triggered();

    void on_m_selectAllAction_triggered();

    void on_m_deleteAction_triggered();

    void on_m_hideAction_triggered();

private:
    Ui::KFileSearcherMainWindow *ui;

    bool m_isRegExpEnable;

    /**
     * @brief 当过滤条件变化时，触发此函数
     */
    void onFilterChanged();
};


#endif //KFILESEARCHER_KFILESEARCHERMAINWINDOW_H
