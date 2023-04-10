#ifndef KFILEFILTER_H
#define KFILEFILTER_H

#include <QObject>

/**
 * @brief 自定义的过滤器，可以选择是否支持正则表达式
 */
class KFileFilter : public QObject
{
    Q_OBJECT
public:
    enum FilterType { None, Dir, File };

    explicit KFileFilter(QObject *parent = nullptr);

    explicit KFileFilter(const QString &pattern, FilterType filterType = None,
                         bool isRegExpEnabled = false, QObject *parent = nullptr);

    explicit KFileFilter(const KFileFilter &filter);

    KFileFilter &operator=(const KFileFilter &filter);

    /**
     * @brief 返回匹配模式字符串（可以是正则表达式）
     * @return
     */
    const QString &pattern() const;

    /**
     * @brief 返回是否支持正则表达式，如果不支持则采用LCS算法匹配字符串，支持则用QRegularExpression匹配
     * @return
     */
    bool isRegExpEnabled() const;

    /**
     * @brief 返回过滤类型，可以是文件夹或者是文件
     * @return
     */
    FilterType filterType() const;

    void setFilterType(FilterType newFilterType);

    void setPattern(const QString &newPattern);

    void setIsRegExpEnabled(bool newIsRegExpEnabled);

private:
    QString m_pattern;
    FilterType m_filterType;
    bool m_isRegExpEnabled;
};

#endif // KFILEFILTER_H
