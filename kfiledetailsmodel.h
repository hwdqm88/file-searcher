#ifndef KFILEDETAILSMODEL_H
#define KFILEDETAILSMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QFileInfoList>

#include "kfilefilter.h"

/**
 * @brief 文件详细信息的Model层
 */
class KFileDetailsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit KFileDetailsModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief sort 实现排序算法，可以按照不同列排序
     * @param column 排序的列
     * @param order 排序顺序(升序或降序)
     */
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    /**
     * @brief 装在过滤器，从桌面全部文件中筛选出符合条件的文件信息
     * @param filter 过滤器
     */
    void installFilter(const KFileFilter &filter);

    /**
     * @brief fileInfos 返回指定的文件信息列表
     * @param indexes 指定的序号
     * @return
     */
    QFileInfoList fileInfos(const QModelIndexList &indexes);

private:
    /**
     * @brief 标题栏标题列表
     */
    QStringList m_headerList;

    /**
     * @brief model里面保存的文件信息，注意这里的文件信息是筛选之后的，和KFileInfos类中的文件信息可能会不同
     */
    QFileInfoList m_fileInfos;

    /**
     * @brief 记录当前是根据哪一列排序的，用于文件刷新时重新排序
     */
    int m_columnSortedBy;

    /**
     * @brief 记录当前的过滤器，用于文件刷新时重新筛选
     */
    KFileFilter m_fileFilter;

    /**
     * @brief 记录排序的顺序
     */
    Qt::SortOrder m_orderSortedBy;

    /**
     * @brief LCS算法中保存动态规划结果的数组，用固定的空间避免在函数中频繁申请释放
     */
    static QVector<QVector<int>> s_LCSArray;

    /**
     * @brief 此函数用于实现LCS算法
     * @param a 字符串a
     * @param b 字符串b
     * @return 最长公共前缀长度
     */
    static int LCS(const QString &a, const QString &b);
};

#endif // KFILEDETAILSMODEL_H
