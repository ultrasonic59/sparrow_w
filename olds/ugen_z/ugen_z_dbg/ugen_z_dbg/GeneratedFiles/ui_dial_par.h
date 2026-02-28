/********************************************************************************
** Form generated from reading UI file 'dial_par.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIAL_PAR_H
#define UI_DIAL_PAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <param_ed.h>

QT_BEGIN_NAMESPACE

class Ui_Dialog_par
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLineEdit *lineEdit_ip;
    QLabel *label_ip;
    QLabel *lab_chans;
    QLabel *lab_usils;
    QLabel *lab_tects;
    QLabel *lab_0;
    param_ed *line_ed_us0;
    param_ed *line_tact0;
    param_ed *line_ed_us1;
    param_ed *line_tact1;
    QLabel *lab_1;
    QCheckBox *checkBox_us_ed;
    QCheckBox *checkBox_ed_tact;
    QPushButton *pushButton_colors;

    void setupUi(QDialog *Dialog_par)
    {
        if (Dialog_par->objectName().isEmpty())
            Dialog_par->setObjectName(QStringLiteral("Dialog_par"));
        Dialog_par->resize(401, 179);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog_par->sizePolicy().hasHeightForWidth());
        Dialog_par->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(10);
        Dialog_par->setFont(font);
        layoutWidget = new QWidget(Dialog_par);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 130, 351, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacer = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setFont(font);

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setFont(font);

        hboxLayout->addWidget(cancelButton);

        lineEdit_ip = new QLineEdit(Dialog_par);
        lineEdit_ip->setObjectName(QStringLiteral("lineEdit_ip"));
        lineEdit_ip->setGeometry(QRect(90, 10, 211, 21));
        lineEdit_ip->setFont(font);
        label_ip = new QLabel(Dialog_par);
        label_ip->setObjectName(QStringLiteral("label_ip"));
        label_ip->setGeometry(QRect(10, 13, 81, 20));
        label_ip->setFont(font);
        lab_chans = new QLabel(Dialog_par);
        lab_chans->setObjectName(QStringLiteral("lab_chans"));
        lab_chans->setGeometry(QRect(10, 40, 51, 21));
        lab_chans->setFont(font);
        lab_usils = new QLabel(Dialog_par);
        lab_usils->setObjectName(QStringLiteral("lab_usils"));
        lab_usils->setGeometry(QRect(10, 70, 41, 21));
        lab_usils->setFont(font);
        lab_tects = new QLabel(Dialog_par);
        lab_tects->setObjectName(QStringLiteral("lab_tects"));
        lab_tects->setGeometry(QRect(10, 100, 41, 16));
        lab_tects->setFont(font);
        lab_0 = new QLabel(Dialog_par);
        lab_0->setObjectName(QStringLiteral("lab_0"));
        lab_0->setGeometry(QRect(70, 40, 16, 21));
        lab_0->setFont(font);
        line_ed_us0 = new param_ed(Dialog_par);
        line_ed_us0->setObjectName(QStringLiteral("line_ed_us0"));
        line_ed_us0->setGeometry(QRect(60, 70, 31, 22));
        line_ed_us0->setFont(font);
        line_tact0 = new param_ed(Dialog_par);
        line_tact0->setObjectName(QStringLiteral("line_tact0"));
        line_tact0->setGeometry(QRect(60, 100, 31, 22));
        line_tact0->setFont(font);
        line_ed_us1 = new param_ed(Dialog_par);
        line_ed_us1->setObjectName(QStringLiteral("line_ed_us1"));
        line_ed_us1->setGeometry(QRect(100, 70, 31, 22));
        line_ed_us1->setFont(font);
        line_tact1 = new param_ed(Dialog_par);
        line_tact1->setObjectName(QStringLiteral("line_tact1"));
        line_tact1->setGeometry(QRect(100, 100, 31, 22));
        line_tact1->setFont(font);
        lab_1 = new QLabel(Dialog_par);
        lab_1->setObjectName(QStringLiteral("lab_1"));
        lab_1->setGeometry(QRect(110, 40, 16, 21));
        lab_1->setFont(font);
        checkBox_us_ed = new QCheckBox(Dialog_par);
        checkBox_us_ed->setObjectName(QStringLiteral("checkBox_us_ed"));
        checkBox_us_ed->setGeometry(QRect(140, 70, 151, 21));
        checkBox_us_ed->setFont(font);
        checkBox_ed_tact = new QCheckBox(Dialog_par);
        checkBox_ed_tact->setObjectName(QStringLiteral("checkBox_ed_tact"));
        checkBox_ed_tact->setGeometry(QRect(140, 100, 151, 21));
        checkBox_ed_tact->setFont(font);
        pushButton_colors = new QPushButton(Dialog_par);
        pushButton_colors->setObjectName(QStringLiteral("pushButton_colors"));
        pushButton_colors->setGeometry(QRect(310, 10, 81, 23));
        QWidget::setTabOrder(lineEdit_ip, pushButton_colors);
        QWidget::setTabOrder(pushButton_colors, line_ed_us0);
        QWidget::setTabOrder(line_ed_us0, line_tact0);
        QWidget::setTabOrder(line_tact0, line_ed_us1);
        QWidget::setTabOrder(line_ed_us1, line_tact1);
        QWidget::setTabOrder(line_tact1, checkBox_us_ed);
        QWidget::setTabOrder(checkBox_us_ed, checkBox_ed_tact);
        QWidget::setTabOrder(checkBox_ed_tact, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(Dialog_par);
        QObject::connect(okButton, SIGNAL(clicked()), Dialog_par, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Dialog_par, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog_par);
    } // setupUi

    void retranslateUi(QDialog *Dialog_par)
    {
        Dialog_par->setWindowTitle(QApplication::translate("Dialog_par", "Param", 0));
        okButton->setText(QApplication::translate("Dialog_par", "OK", 0));
        cancelButton->setText(QApplication::translate("Dialog_par", "Cancel", 0));
        label_ip->setText(QApplication::translate("Dialog_par", "IP \320\260\320\264\321\200\320\265\321\201:", 0));
        lab_chans->setText(QApplication::translate("Dialog_par", "\320\232\320\260\320\275\320\260\320\273", 0));
        lab_usils->setText(QApplication::translate("Dialog_par", "\320\243\321\201\320\270\320\273", 0));
        lab_tects->setText(QApplication::translate("Dialog_par", "\320\242\320\260\320\272\321\202", 0));
        lab_0->setText(QApplication::translate("Dialog_par", "1", 0));
        lab_1->setText(QApplication::translate("Dialog_par", "2", 0));
        checkBox_us_ed->setText(QApplication::translate("Dialog_par", "\321\200\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265", 0));
        checkBox_ed_tact->setText(QApplication::translate("Dialog_par", "\321\200\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265", 0));
        pushButton_colors->setText(QApplication::translate("Dialog_par", "\320\246\320\262\320\265\321\202\320\260", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog_par: public Ui_Dialog_par {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIAL_PAR_H
