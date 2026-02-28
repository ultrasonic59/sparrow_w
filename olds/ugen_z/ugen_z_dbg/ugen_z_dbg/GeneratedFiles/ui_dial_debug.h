/********************************************************************************
** Form generated from reading UI file 'dial_debug.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIAL_DEBUG_H
#define UI_DIAL_DEBUG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Dial_debug
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEdit_addr;
    QLabel *label;
    QLineEdit *lineEdit_wr_dat;
    QPushButton *pushButt_wr;
    QPushButton *pushButton;
    QLineEdit *lineEdit_count;
    QTextEdit *textEdit_rd_dat;
    QComboBox *comboBox_rej;
    QPushButton *pushButt_send;
    QLineEdit *lineEdit_send;

    void setupUi(QDialog *Dial_debug)
    {
        if (Dial_debug->objectName().isEmpty())
            Dial_debug->setObjectName(QStringLiteral("Dial_debug"));
        Dial_debug->resize(445, 428);
        QFont font;
        font.setPointSize(9);
        Dial_debug->setFont(font);
        buttonBox = new QDialogButtonBox(Dial_debug);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(340, 10, 81, 61));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEdit_addr = new QLineEdit(Dial_debug);
        lineEdit_addr->setObjectName(QStringLiteral("lineEdit_addr"));
        lineEdit_addr->setGeometry(QRect(340, 140, 81, 21));
        QFont font1;
        font1.setPointSize(10);
        lineEdit_addr->setFont(font1);
        label = new QLabel(Dial_debug);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(360, 120, 41, 16));
        lineEdit_wr_dat = new QLineEdit(Dial_debug);
        lineEdit_wr_dat->setObjectName(QStringLiteral("lineEdit_wr_dat"));
        lineEdit_wr_dat->setGeometry(QRect(340, 230, 81, 22));
        pushButt_wr = new QPushButton(Dial_debug);
        pushButt_wr->setObjectName(QStringLiteral("pushButt_wr"));
        pushButt_wr->setGeometry(QRect(340, 260, 81, 28));
        pushButton = new QPushButton(Dial_debug);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(340, 200, 81, 28));
        lineEdit_count = new QLineEdit(Dial_debug);
        lineEdit_count->setObjectName(QStringLiteral("lineEdit_count"));
        lineEdit_count->setGeometry(QRect(340, 171, 81, 21));
        textEdit_rd_dat = new QTextEdit(Dial_debug);
        textEdit_rd_dat->setObjectName(QStringLiteral("textEdit_rd_dat"));
        textEdit_rd_dat->setGeometry(QRect(10, 10, 321, 411));
        textEdit_rd_dat->setFont(font1);
        comboBox_rej = new QComboBox(Dial_debug);
        comboBox_rej->setObjectName(QStringLiteral("comboBox_rej"));
        comboBox_rej->setGeometry(QRect(340, 90, 81, 21));
        comboBox_rej->setFont(font1);
        pushButt_send = new QPushButton(Dial_debug);
        pushButt_send->setObjectName(QStringLiteral("pushButt_send"));
        pushButt_send->setGeometry(QRect(340, 330, 81, 28));
        lineEdit_send = new QLineEdit(Dial_debug);
        lineEdit_send->setObjectName(QStringLiteral("lineEdit_send"));
        lineEdit_send->setGeometry(QRect(340, 300, 91, 22));

        retranslateUi(Dial_debug);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dial_debug, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dial_debug, SLOT(reject()));
        QObject::connect(pushButton, SIGNAL(clicked()), Dial_debug, SLOT(on_butt_rd()));
        QObject::connect(pushButt_wr, SIGNAL(clicked()), Dial_debug, SLOT(on_butt_wr()));
        QObject::connect(pushButt_send, SIGNAL(clicked()), Dial_debug, SLOT(on_butt_send()));

        QMetaObject::connectSlotsByName(Dial_debug);
    } // setupUi

    void retranslateUi(QDialog *Dial_debug)
    {
        Dial_debug->setWindowTitle(QApplication::translate("Dial_debug", "Dial_debug", 0));
        label->setText(QApplication::translate("Dial_debug", "Addr", 0));
        pushButt_wr->setText(QApplication::translate("Dial_debug", "Wr", 0));
        pushButton->setText(QApplication::translate("Dial_debug", "Rd", 0));
        pushButt_send->setText(QApplication::translate("Dial_debug", "send", 0));
    } // retranslateUi

};

namespace Ui {
    class Dial_debug: public Ui_Dial_debug {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIAL_DEBUG_H
