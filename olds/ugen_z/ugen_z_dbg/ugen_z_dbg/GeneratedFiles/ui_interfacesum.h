/********************************************************************************
** Form generated from reading UI file 'interfacesum.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACESUM_H
#define UI_INTERFACESUM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InterfaceSum
{
public:
    QWidget *centralWidget;
    QScrollBar *verticalScrollBar;
    QScrollBar *verticalScrollBar_2;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QMainWindow *InterfaceSum)
    {
        if (InterfaceSum->objectName().isEmpty())
            InterfaceSum->setObjectName(QStringLiteral("InterfaceSum"));
        InterfaceSum->resize(800, 570);
        centralWidget = new QWidget(InterfaceSum);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalScrollBar = new QScrollBar(centralWidget);
        verticalScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
        verticalScrollBar->setGeometry(QRect(0, 40, 31, 461));
        verticalScrollBar->setOrientation(Qt::Vertical);
        verticalScrollBar_2 = new QScrollBar(centralWidget);
        verticalScrollBar_2->setObjectName(QStringLiteral("verticalScrollBar_2"));
        verticalScrollBar_2->setGeometry(QRect(769, 29, 31, 471));
        verticalScrollBar_2->setOrientation(Qt::Vertical);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(5, 0, 791, 31));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 510, 121, 61));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(124, 512, 111, 61));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(240, 512, 101, 61));
        InterfaceSum->setCentralWidget(centralWidget);

        retranslateUi(InterfaceSum);

        QMetaObject::connectSlotsByName(InterfaceSum);
    } // setupUi

    void retranslateUi(QMainWindow *InterfaceSum)
    {
        InterfaceSum->setWindowTitle(QApplication::translate("InterfaceSum", "InterfaceSum", 0));
        label->setText(QApplication::translate("InterfaceSum", "TextLabel", 0));
        pushButton->setText(QApplication::translate("InterfaceSum", "PushButton", 0));
        pushButton_2->setText(QApplication::translate("InterfaceSum", "PushButton", 0));
        pushButton_3->setText(QApplication::translate("InterfaceSum", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class InterfaceSum: public Ui_InterfaceSum {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACESUM_H
