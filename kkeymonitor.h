#ifndef KKEYMONITOR_H
#define KKEYMONITOR_H

#include <QObject>

#include <windows.h>

/**
 * @brief 自定义键盘监视类，用于挂载windows系统钩子，以达到界面隐藏失去聚焦之后还可以监听键盘事件的作用
 */
class KKeyMonitor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 监听类采用单例模式
     * @return 监听类实例化对象指针
     */
    static KKeyMonitor *instance();

signals:
    void hideShortcutPressed();

private:
    // 系统钩子变量，定义为静态成员才能被下面的两个钩子函数访问
    static HHOOK s_hook;

    // 以下两个函数是实现系统钩子必须的，必须定义为静态成员函数（普通成员函数不能转化为函数指针）
    static LRESULT keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HMODULE moduleFromAddress(PVOID pv);

    // 记录上一个按键按的是什么
    static DWORD s_lastVkCode;

    static KKeyMonitor *s_instance;
    explicit KKeyMonitor(QObject *parent = nullptr);
};

#endif // KKEYMONITOR_H
