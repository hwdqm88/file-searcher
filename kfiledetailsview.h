#ifndef KFILESEARCHER_KFILEDETAILSWIDGET_H
#define KFILESEARCHER_KFILEDETAILSWIDGET_H

#include <QTableView>
#include <QMouseEvent>
#include <QPoint>

#include "kfiledetailsmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class KFileDetailsWidget; }
QT_END_NAMESPACE

/**
 * @brief 文件详细信息的View层
 */
class KFileDetailsView : public QTableView
{
Q_OBJECT

public:
    explicit KFileDetailsView(QWidget *parent = nullptr);

    ~KFileDetailsView() override;

    /**
     * @brief model 获取绑定的model
     * @return 绑定的model
     */
    KFileDetailsModel *model();

    /**
     * @brief 获取选中的项列表
     * @return 选中项的QModelIndex列表
     */
    QModelIndexList selectedIndexes() const override;

protected:
    /**
     * @brief 重写鼠标事件实现：点击左键选中整行，点击右键弹出菜单
     * @param event 鼠标press事件
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    KFileDetailsModel *m_model;
};


#endif //KFILESEARCHER_KFILEDETAILSWIDGET_H
