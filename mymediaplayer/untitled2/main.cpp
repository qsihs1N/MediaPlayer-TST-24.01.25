#include "mainwindow.h"
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>
#include<QMediaPlaylist>
#include <QResource>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font(QStringLiteral("Tahoma"));
    font.setPixelSize(12);
    qApp->setFont(font);
    MainWindow w;

    // 注册资源文件，使用正确的别名
    QResource::registerResource(":/new/prefix1/1.qrc");

    // 使用正确的别名设置应用程序图标
    w.setWindowIcon(QIcon(":/new/prefix1/a"));
    w.show();
    return a.exec();
}
