/********************************************************************************
** Form generated from reading UI file 'dial_load_save_par.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIAL_LOAD_SAVE_PAR_H
#define UI_DIAL_LOAD_SAVE_PAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_load_save_par
{
public:
    QLineEdit *lineEdit_path;
    QLabel *lab_path;
    QLineEdit *lineEdit_num_par;
    QLabel *lab_npar;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *pushButt_dir;

    void setupUi(QDialog *Dialog_load_save_par)
    {
        if (Dialog_load_save_par->objectName().isEmpty())
            Dialog_load_save_par->setObjectName(QStringLiteral("Dialog_load_save_par"));
        Dialog_load_save_par->resize(708, 75);
        lineEdit_path = new QLineEdit(Dialog_load_save_par);
        lineEdit_path->setObjectName(QStringLiteral("lineEdit_path"));
        lineEdit_path->setGeometry(QRect(180, 10, 491, 20));
        lab_path = new QLabel(Dialog_load_save_par);
        lab_path->setObjectName(QStringLiteral("lab_path"));
        lab_path->setGeometry(QRect(140, 10, 31, 20));
        lineEdit_num_par = new QLineEdit(Dialog_load_save_par);
        lineEdit_num_par->setObjectName(QStringLiteral("lineEdit_num_par"));
        lineEdit_num_par->setGeometry(QRect(80, 10, 51, 20));
        lab_npar = new QLabel(Dialog_load_save_par);
        lab_npar->setObjectName(QStringLiteral("lab_npar"));
        lab_npar->setGeometry(QRect(10, 10, 71, 16));
        okButton = new QPushButton(Dialog_load_save_par);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(10, 40, 81, 21));
        cancelButton = new QPushButton(Dialog_load_save_par);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(100, 40, 71, 21));
        pushButt_dir = new QPushButton(Dialog_load_save_par);
        pushButt_dir->setObjectName(QStringLiteral("pushButt_dir"));
        pushButt_dir->setGeometry(QRect(680, 10, 21, 21));

        retranslateUi(Dialog_load_save_par);
        QObject::connect(okButton, SIGNAL(clicked()), Dialog_load_save_par, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Dialog_load_save_par, SLOT(reject()));
        QObject::connect(pushButt_dir, SIGNAL(clicked()), Dialog_load_save_par, SLOT(on_butt_dir()));

        QMetaObject::connectSlotsByName(Dialog_load_save_par);
    } // setupUi

    void retranslateUi(QDialog *Dialog_load_save_par)
    {
        Dialog_load_save_par->setWindowTitle(QApplication::translate("Dialog_load_save_par", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", 0));
        lab_path->setText(QApplication::translate("Dialog_load_save_par", "\320\277\321\203\321\202\321\214:", 0));
        lab_npar->setText(QApplication::translate("Dialog_load_save_par", "\320\275\320\276\320\274\320\265\321\200 \320\277\320\260\321\200.", 0));
        okButton->setText(QApplication::translate("Dialog_load_save_par", "OK", 0));
        cancelButton->setText(QApplication::translate("Dialog_load_save_par", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));
        pushButt_dir->setText(QApplication::translate("Dialog_load_save_par", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog_load_save_par: public Ui_Dialog_load_save_par {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIAL_LOAD_SAVE_PAR_H
