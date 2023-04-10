#include "kfilefilter.h"

#include <QApplication>

KFileFilter::KFileFilter(QObject *parent) :
    QObject(parent),
    m_filterType(None),
    m_isRegExpEnabled(false)
{
}

KFileFilter::KFileFilter(const QString &pattern, FilterType filterType, bool isRegExpEnabled, QObject *parent) :
    QObject(parent),
    m_pattern(pattern),
    m_filterType(filterType),
    m_isRegExpEnabled(isRegExpEnabled)
{
}

KFileFilter::KFileFilter(const KFileFilter &filter) :
    QObject(filter.parent()),
    m_pattern(filter.pattern()),
    m_filterType(filter.filterType()),
    m_isRegExpEnabled(filter.isRegExpEnabled())
{
}

KFileFilter &KFileFilter::operator=(const KFileFilter &filter)
{
    setParent(filter.parent());
    setPattern(filter.pattern());
    setFilterType(filter.filterType());
    m_isRegExpEnabled = filter.isRegExpEnabled();
    return *this;
}

const QString &KFileFilter::pattern() const
{
    return m_pattern;
}

bool KFileFilter::isRegExpEnabled() const
{
    return m_isRegExpEnabled;
}

KFileFilter::FilterType KFileFilter::filterType() const
{
    return m_filterType;
}

void KFileFilter::setFilterType(FilterType newFilterType)
{
    m_filterType = newFilterType;
}

void KFileFilter::setPattern(const QString &newPattern)
{
    m_pattern = newPattern;
}

void KFileFilter::setIsRegExpEnabled(bool newIsRegExpEnabled)
{
    m_isRegExpEnabled = newIsRegExpEnabled;
}
