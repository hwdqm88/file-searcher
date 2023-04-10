#include "kkeymonitor.h"

#include <QDebug>
#include <QApplication>

HHOOK KKeyMonitor::s_hook = SetWindowsHookExW(WH_KEYBOARD_LL, keyboardHookProc, moduleFromAddress((PVOID)keyboardHookProc), 0);
DWORD KKeyMonitor::s_lastVkCode;
KKeyMonitor *KKeyMonitor::s_instance = nullptr;

KKeyMonitor::KKeyMonitor(QObject *parent) : QObject(parent)
{
}

KKeyMonitor *KKeyMonitor::instance()
{
    if (!s_instance)
        s_instance = new KKeyMonitor(qApp);
    return s_instance;
}

LRESULT KKeyMonitor::keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *keyInfo = (KBDLLHOOKSTRUCT *)lParam;

    // 判断调用钩子函数的类型
    if (nCode == HC_ACTION)
    {
        // 是按键按下的类型
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            if (keyInfo->vkCode == 32 && s_lastVkCode == 164)
                emit s_instance->hideShortcutPressed();
            s_lastVkCode = keyInfo->vkCode;
        }
    }
    return CallNextHookEx(s_hook, nCode, wParam, lParam);
}

HMODULE KKeyMonitor::moduleFromAddress(PVOID pv)
{
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
        return (HMODULE)mbi.AllocationBase;
    else
        return NULL;
}
