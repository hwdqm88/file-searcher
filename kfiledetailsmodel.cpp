#include "kfiledetailsmodel.h"

#include <QDateTime>
#include <QDebug>
#include <QRegularExpression>
#include <algorithm>
#include <QFileIconProvider>

#include "kfileinfos.h"
#include "kpinyin.h"

QVector<QVector<int>> KFileDetailsModel::s_LCSArray;

KFileDetailsModel::KFileDetailsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_headerList << tr("名称") << tr("路径") << tr("大小") << tr("修改时间");
    m_fileInfos = KFileInfos::instance()->fileInfoList();

    connect(KFileInfos::instance(), &KFileInfos::filesChanged, this, [&]() {
        installFilter(m_fileFilter);
    }, Qt::QueuedConnection);
}

QVariant KFileDetailsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        if (section >= 0 && section < m_headerList.size())
            return m_headerList[section];
    return QAbstractTableModel::headerData(section, orientation, role);
}

int KFileDetailsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_fileInfos.size();

}

int KFileDetailsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_headerList.size();
}

QVariant KFileDetailsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() >= 0 && index.column() < m_headerList.size() &&
                index.row() >= 0 && index.row() < m_fileInfos.size())
        {
            switch (index.column())
            {
            case 0:
                return m_fileInfos[index.row()].fileName();
            case 1:
                return m_fileInfos[index.row()].absoluteFilePath();
            case 2:
                return m_fileInfos[index.row()].isFile() ?
                        QString("%1KB").arg(m_fileInfos[index.row()].size() / 1024) :
                        QString();
            case 3:
                return m_fileInfos[index.row()].lastModified().toString("yyyy-MM-dd hh:mm:ss");
            default:
                break;
            }
        }
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.column() == 0)
            return QFileIconProvider().icon(m_fileInfos[index.row()]);
    }
    return QVariant();
}

void KFileDetailsModel::sort(int column, Qt::SortOrder order)
{
    m_columnSortedBy = column;
    m_orderSortedBy = order;

    // 根据不同的列选择不同的排序函数
    std::function<bool(const QFileInfo &, const QFileInfo &)> compareFunction;
    switch (column) {
    case 0:
        compareFunction = [&](const QFileInfo &_a, const QFileInfo &_b)
        {
            if (order == Qt::AscendingOrder)
                return _a.fileName() < _b.fileName();
            else
                return _a.fileName() > _b.fileName();
        };
        break;
    case 1:
        compareFunction = [&](const QFileInfo &_a, const QFileInfo &_b)
        {
            if (order == Qt::AscendingOrder)
                return _a.absoluteFilePath() < _b.absoluteFilePath();
            else
                return _a.absoluteFilePath() > _b.absoluteFilePath();
        };
        break;
    case 2:
        compareFunction = [&](const QFileInfo &_a, const QFileInfo &_b)
        {
            if (order == Qt::AscendingOrder)
                return _a.size() < _b.size();
            else
                return _a.size() > _b.size();
        };
        break;
    case 3:
        compareFunction = [&](const QFileInfo &_a, const QFileInfo &_b)
        {
            if (order == Qt::AscendingOrder)
                return _a.lastModified() < _b.lastModified();
            else
                return _a.lastModified() > _b.lastModified();
        };
        break;
    default:
        break;
    }
    std::sort(m_fileInfos.begin(), m_fileInfos.end(), compareFunction);
    emit dataChanged(index(0, 0), index(m_fileInfos.count() - 1, m_headerList.count() - 1));
}

void KFileDetailsModel::installFilter(const KFileFilter &filter)
{
    m_fileFilter = filter;

    // 支持正则表达式的情况，采用QRegularExpression匹配
    if (filter.isRegExpEnabled())
    {
        m_fileInfos.clear();
        QRegularExpression regExp(filter.pattern(), QRegularExpression::CaseInsensitiveOption);
        foreach(const QFileInfo &fileInfo, KFileInfos::instance()->fileInfoList())
        {
            if (filter.filterType() == KFileFilter::File && fileInfo.isDir())
                continue;
            else if (filter.filterType() == KFileFilter::Dir && fileInfo.isFile())
                continue;
            QRegularExpressionMatch matchAnswer = regExp.match(fileInfo.fileName());
            if (matchAnswer.hasMatch())
                m_fileInfos.append(fileInfo);
        }
    }

    // 不支持正则表达式的情况，采用LCS算法匹配
    else
    {
        m_fileInfos.clear();
        foreach(const QFileInfo &fileInfo, KFileInfos::instance()->fileInfoList())
        {
            if (filter.filterType() == KFileFilter::File && fileInfo.isDir())
                continue;
            else if (filter.filterType() == KFileFilter::Dir && fileInfo.isFile())
                continue;
            if (LCS(filter.pattern(), fileInfo.fileName()) == filter.pattern().length())
            {
                m_fileInfos.append(fileInfo);
                continue;
            }
            QString convertToPinyin, fileName = fileInfo.fileName();
            for (int i = 0; i < fileName.length(); i++)
            {
                ushort uni = fileName[i].unicode();
                if (uni >= 0x4e00 && uni <= 0x9fa5)
                    convertToPinyin += KPinyin::getNoRepeatPingyinForWord(QString(fileName[i]));
                else
                    convertToPinyin += fileName[i];
            }
            if (LCS(filter.pattern(), convertToPinyin) == filter.pattern().length())
                m_fileInfos.append(fileInfo);
        }
    }
    sort(m_columnSortedBy, m_orderSortedBy);
}

QFileInfoList KFileDetailsModel::fileInfos(const QModelIndexList &indexes)
{
    QFileInfoList result;

    foreach(const QModelIndex &index, indexes)
        if (index.column() == 0)
            result.append(m_fileInfos[index.row()]);

    return result;
}

int KFileDetailsModel::LCS(const QString &a, const QString &b)
{
    if (s_LCSArray.size() < a.length() + 1)
        s_LCSArray.resize(a.length() + 1);
    if (s_LCSArray[0].size() < b.length() + 1)
        s_LCSArray[0].resize(b.length() + 1);
    s_LCSArray[0][0] = 0;
    for (int i = 1; i <= a.length(); i++)
    {
        if (s_LCSArray[i].size() < b.length() + 1)
            s_LCSArray[i].resize(b.length() + 1);
        for (int j = 1; j <= b.length(); j++)
        {
            if (QString::compare(a[i - 1], b[j - 1], Qt::CaseInsensitive) == 0)
                s_LCSArray[i][j] = s_LCSArray[i - 1][j - 1] + 1;
            else
                s_LCSArray[i][j] = std::max(s_LCSArray[i - 1][j], s_LCSArray[i][j - 1]);
        }
    }
    return s_LCSArray[a.length()][b.length()];
}
