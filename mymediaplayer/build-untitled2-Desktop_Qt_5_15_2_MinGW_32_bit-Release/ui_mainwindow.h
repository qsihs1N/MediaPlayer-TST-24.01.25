/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QWidget *widget_3;
    QPushButton *save;
    QPushButton *del;
    QPushButton *grab;
    QComboBox *comboBox_2;
    QPushButton *load;
    QComboBox *comboBox_3;
    QComboBox *comboBox;
    QListWidget *list;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_now;
    QLabel *label_2;
    QLabel *label_all;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *open;
    QSpacerItem *horizontalSpacer;
    QPushButton *play;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pause;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *stop;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_3;
    QSlider *horizontalSlider_2;
    QLabel *label_list;
    QLabel *label;
    QSlider *horizontalSlider;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1546, 845);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget_3 = new QWidget(centralwidget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        save = new QPushButton(widget_3);
        save->setObjectName(QString::fromUtf8("save"));
        save->setGeometry(QRect(0, 0, 50, 24));
        save->setMaximumSize(QSize(60, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\215\216\346\226\207\347\220\245\347\217\200"));
        font.setBold(false);
        font.setItalic(false);
        save->setFont(font);
        save->setStyleSheet(QString::fromUtf8("font: 12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));
        del = new QPushButton(widget_3);
        del->setObjectName(QString::fromUtf8("del"));
        del->setGeometry(QRect(70, 0, 50, 24));
        del->setMaximumSize(QSize(60, 16777215));
        del->setFont(font);
        del->setStyleSheet(QString::fromUtf8("font: 12px\"\345\215\216\346\226\207\347\220\245\347\217\200\";"));
        grab = new QPushButton(widget_3);
        grab->setObjectName(QString::fromUtf8("grab"));
        grab->setGeometry(QRect(0, 30, 50, 24));
        grab->setMaximumSize(QSize(60, 16777215));
        grab->setFont(font);
        grab->setStyleSheet(QString::fromUtf8("font: 12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));
        comboBox_2 = new QComboBox(widget_3);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(70, 30, 60, 24));
        comboBox_2->setMinimumSize(QSize(60, 0));
        comboBox_2->setMaximumSize(QSize(60, 16777215));
        comboBox_2->setFont(font);
        comboBox_2->setStyleSheet(QString::fromUtf8("font: 12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));
        load = new QPushButton(widget_3);
        load->setObjectName(QString::fromUtf8("load"));
        load->setGeometry(QRect(140, 0, 50, 24));
        load->setFont(font);
        load->setStyleSheet(QString::fromUtf8("font: 12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));
        comboBox_3 = new QComboBox(widget_3);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));
        comboBox_3->setGeometry(QRect(140, 30, 60, 24));
        comboBox_3->setMaximumSize(QSize(60, 16777215));
        comboBox_3->setFont(font);
        comboBox_3->setStyleSheet(QString::fromUtf8("font:12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));

        gridLayout->addWidget(widget_3, 6, 4, 3, 1);

        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(70, 24));
        comboBox->setMaximumSize(QSize(50, 20));
        comboBox->setFont(font);
        comboBox->setStyleSheet(QString::fromUtf8("font: 12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));

        gridLayout->addWidget(comboBox, 8, 3, 1, 1);

        list = new QListWidget(centralwidget);
        list->setObjectName(QString::fromUtf8("list"));
        list->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(list, 4, 4, 1, 1);

        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMinimumSize(QSize(120, 0));
        widget_2->setMaximumSize(QSize(100, 30));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_now = new QLabel(widget_2);
        label_now->setObjectName(QString::fromUtf8("label_now"));
        label_now->setMinimumSize(QSize(60, 20));
        label_now->setMaximumSize(QSize(40, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font1.setBold(false);
        font1.setItalic(false);
        label_now->setFont(font1);
        label_now->setStyleSheet(QString::fromUtf8("font: 12px \"Microsoft YaHei UI\";"));

        horizontalLayout_3->addWidget(label_now);

        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(10, 20));

        horizontalLayout_3->addWidget(label_2);

        label_all = new QLabel(widget_2);
        label_all->setObjectName(QString::fromUtf8("label_all"));
        label_all->setMinimumSize(QSize(60, 20));
        label_all->setMaximumSize(QSize(40, 40));
        label_all->setFont(font1);
        label_all->setStyleSheet(QString::fromUtf8("font: 12px \"Microsoft YaHei UI\";"));

        horizontalLayout_3->addWidget(label_all);


        gridLayout->addWidget(widget_2, 6, 3, 1, 1);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMaximumSize(QSize(10000, 40));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        open = new QPushButton(widget);
        open->setObjectName(QString::fromUtf8("open"));
        open->setMinimumSize(QSize(80, 30));
        open->setMaximumSize(QSize(16666666, 16777215));
        open->setFont(font);
        open->setStyleSheet(QString::fromUtf8("font: 12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));

        horizontalLayout->addWidget(open);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        play = new QPushButton(widget);
        play->setObjectName(QString::fromUtf8("play"));
        play->setMinimumSize(QSize(80, 30));
        play->setFont(font);
        play->setStyleSheet(QString::fromUtf8("font: 12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));

        horizontalLayout->addWidget(play);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pause = new QPushButton(widget);
        pause->setObjectName(QString::fromUtf8("pause"));
        pause->setMinimumSize(QSize(80, 30));
        pause->setFont(font);
        pause->setStyleSheet(QString::fromUtf8("font: 12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));

        horizontalLayout->addWidget(pause);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        stop = new QPushButton(widget);
        stop->setObjectName(QString::fromUtf8("stop"));
        stop->setMinimumSize(QSize(80, 30));
        stop->setFont(font);
        stop->setStyleSheet(QString::fromUtf8("font: 12px \"\345\215\216\346\226\207\347\220\245\347\217\200\";"));

        horizontalLayout->addWidget(stop);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(100, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 12px \"Microsoft YaHei UI\";"));

        horizontalLayout->addWidget(label_3);

        horizontalSlider_2 = new QSlider(widget);
        horizontalSlider_2->setObjectName(QString::fromUtf8("horizontalSlider_2"));
        horizontalSlider_2->setMaximumSize(QSize(100, 16777215));
        horizontalSlider_2->setCursor(QCursor(Qt::OpenHandCursor));
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider_2);


        gridLayout->addWidget(widget, 8, 1, 1, 1);

        label_list = new QLabel(centralwidget);
        label_list->setObjectName(QString::fromUtf8("label_list"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\215\216\346\226\207\344\270\255\345\256\213"));
        font2.setBold(false);
        font2.setItalic(false);
        label_list->setFont(font2);
        label_list->setStyleSheet(QString::fromUtf8("font: 15px \"\345\215\216\346\226\207\344\270\255\345\256\213\";"));

        gridLayout->addWidget(label_list, 1, 4, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 1, 5, 3);

        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setCursor(QCursor(Qt::OpenHandCursor));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 6, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1546, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        save->setText(QString());
        del->setText(QString());
        grab->setText(QString());
        load->setText(QString());
        label_now->setText(QCoreApplication::translate("MainWindow", "00:00", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "/", nullptr));
        label_all->setText(QCoreApplication::translate("MainWindow", "00:00", nullptr));
        open->setText(QString());
        play->setText(QString());
        pause->setText(QString());
        stop->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "\351\237\263\351\207\217", nullptr));
        label_list->setText(QCoreApplication::translate("MainWindow", "\346\222\255\346\224\276\345\210\227\350\241\250", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
