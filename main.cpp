#include <QApplication>

#include "kfilesearchermainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    KFileSearcherMainWindow w;
    QMessageBox messageBox(QMessageBox::NoIcon, QObject::tr("桌面文件搜索器"),
                           QObject::tr("启动后默认隐藏，按Alt+空格唤出。"), QMessageBox::Ok, &w);
    messageBox.exec();
    return QApplication::exec();
}
