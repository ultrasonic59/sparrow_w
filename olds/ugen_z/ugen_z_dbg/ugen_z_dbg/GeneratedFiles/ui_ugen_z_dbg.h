/********************************************************************************
** Form generated from reading UI file 'ugen_z_dbg.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UGEN_Z_DBG_H
#define UI_UGEN_Z_DBG_H

#include <PlotterWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <param_ed.h>
#include <widgetwithmouseproc.h>

QT_BEGIN_NAMESPACE

class Ui_ugen_z_dbgUI
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_gens;
    QLabel *label_uss;
    QSplitter *splitter;
    WidgetWithMouseProc *OscPlotter;
    PlotterWidget *AutoPlotter;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QLabel *labelStatus;
    QLabel *label_vtime;
    QPushButton *pushButt_debug;
    QLabel *label_begr;
    QComboBox *comboBox_rej;
    param_ed *ed_pre;
    QLabel *lab_num_par;
    param_ed *ed_lstr;
    param_ed *ed_nus;
    QPushButton *pushButt_save_par;
    param_ed *ed_rng;
    param_ed *ed_chan;
    QLabel *label_endr;
    QPushButton *pushButt_load_par;
    param_ed *ed_por;
    param_ed *ed_bstr;
    QLabel *label_chan;
    QPushButton *pushButton_par;
    QLabel *label_bstr;
    QLabel *label_nu;
    QPushButton *pushButt_wr_par;
    QLabel *label_por;
    param_ed *ed_len_def;
    QPushButton *pushButt__rd_par;
    QPushButton *pushButton_test1;
    param_ed *ed_kus;
    QLabel *label_kus;
    QLabel *label_lstr;
    QPushButton *pushButton_test;
    QLabel *label_noConn;
    param_ed *ed_bstr_sp;
    QLabel *label_lstr_sp;
    QLabel *label_bstr_sp;
    param_ed *ed_lstr_sp;
    QCheckBox *checkBox_spectr;
    param_ed *ed_spectr_const;
    QCheckBox *checkBox_http;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *ugen_z_dbgUI)
    {
        if (ugen_z_dbgUI->objectName().isEmpty())
            ugen_z_dbgUI->setObjectName(QStringLiteral("ugen_z_dbgUI"));
        ugen_z_dbgUI->resize(1174, 799);
        centralWidget = new QWidget(ugen_z_dbgUI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_3 = new QHBoxLayout(centralWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(3, 3, 0, 3);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_gens = new QLabel(centralWidget);
        label_gens->setObjectName(QStringLiteral("label_gens"));
        QFont font;
        font.setPointSize(10);
        label_gens->setFont(font);

        horizontalLayout->addWidget(label_gens);

        label_uss = new QLabel(centralWidget);
        label_uss->setObjectName(QStringLiteral("label_uss"));
        label_uss->setFont(font);

        horizontalLayout->addWidget(label_uss);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout);

        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        OscPlotter = new WidgetWithMouseProc(splitter);
        OscPlotter->setObjectName(QStringLiteral("OscPlotter"));
        splitter->addWidget(OscPlotter);
        AutoPlotter = new PlotterWidget(splitter);
        AutoPlotter->setObjectName(QStringLiteral("AutoPlotter"));
        splitter->addWidget(AutoPlotter);

        verticalLayout_2->addWidget(splitter);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(200, 651));
        labelStatus = new QLabel(widget);
        labelStatus->setObjectName(QStringLiteral("labelStatus"));
        labelStatus->setEnabled(false);
        labelStatus->setGeometry(QRect(60, 600, 141, 41));
        label_vtime = new QLabel(widget);
        label_vtime->setObjectName(QStringLiteral("label_vtime"));
        label_vtime->setGeometry(QRect(10, 270, 51, 21));
        label_vtime->setFont(font);
        pushButt_debug = new QPushButton(widget);
        pushButt_debug->setObjectName(QStringLiteral("pushButt_debug"));
        pushButt_debug->setEnabled(true);
        pushButt_debug->setGeometry(QRect(2, 630, 51, 21));
        label_begr = new QLabel(widget);
        label_begr->setObjectName(QStringLiteral("label_begr"));
        label_begr->setGeometry(QRect(0, 220, 41, 21));
        label_begr->setFont(font);
        comboBox_rej = new QComboBox(widget);
        comboBox_rej->setObjectName(QStringLiteral("comboBox_rej"));
        comboBox_rej->setEnabled(false);
        comboBox_rej->setGeometry(QRect(110, 190, 73, 22));
        comboBox_rej->setFont(font);
        comboBox_rej->setLayoutDirection(Qt::RightToLeft);
        comboBox_rej->setMaxCount(2);
        ed_pre = new param_ed(widget);
        ed_pre->setObjectName(QStringLiteral("ed_pre"));
        ed_pre->setGeometry(QRect(60, 220, 71, 22));
        ed_pre->setFont(font);
        ed_pre->setAlignment(Qt::AlignCenter);
        lab_num_par = new QLabel(widget);
        lab_num_par->setObjectName(QStringLiteral("lab_num_par"));
        lab_num_par->setGeometry(QRect(164, 4, 41, 21));
        QFont font1;
        font1.setPointSize(11);
        lab_num_par->setFont(font1);
        ed_lstr = new param_ed(widget);
        ed_lstr->setObjectName(QStringLiteral("ed_lstr"));
        ed_lstr->setGeometry(QRect(60, 330, 71, 22));
        ed_lstr->setFont(font);
        ed_lstr->setAlignment(Qt::AlignCenter);
        ed_nus = new param_ed(widget);
        ed_nus->setObjectName(QStringLiteral("ed_nus"));
        ed_nus->setEnabled(false);
        ed_nus->setGeometry(QRect(60, 170, 21, 22));
        ed_nus->setFont(font);
        ed_nus->setAlignment(Qt::AlignCenter);
        pushButt_save_par = new QPushButton(widget);
        pushButt_save_par->setObjectName(QStringLiteral("pushButt_save_par"));
        pushButt_save_par->setGeometry(QRect(0, 60, 181, 31));
        pushButt_save_par->setFont(font);
        ed_rng = new param_ed(widget);
        ed_rng->setObjectName(QStringLiteral("ed_rng"));
        ed_rng->setGeometry(QRect(60, 240, 71, 22));
        ed_rng->setFont(font);
        ed_rng->setAlignment(Qt::AlignCenter);
        ed_chan = new param_ed(widget);
        ed_chan->setObjectName(QStringLiteral("ed_chan"));
        ed_chan->setEnabled(false);
        ed_chan->setGeometry(QRect(60, 150, 21, 22));
        ed_chan->setFont(font);
        ed_chan->setAlignment(Qt::AlignCenter);
        label_endr = new QLabel(widget);
        label_endr->setObjectName(QStringLiteral("label_endr"));
        label_endr->setGeometry(QRect(0, 240, 41, 21));
        label_endr->setFont(font);
        pushButt_load_par = new QPushButton(widget);
        pushButt_load_par->setObjectName(QStringLiteral("pushButt_load_par"));
        pushButt_load_par->setGeometry(QRect(0, 30, 181, 31));
        pushButt_load_par->setFont(font);
        ed_por = new param_ed(widget);
        ed_por->setObjectName(QStringLiteral("ed_por"));
        ed_por->setEnabled(false);
        ed_por->setGeometry(QRect(40, 410, 41, 22));
        ed_por->setFont(font);
        ed_por->setAlignment(Qt::AlignCenter);
        ed_bstr = new param_ed(widget);
        ed_bstr->setObjectName(QStringLiteral("ed_bstr"));
        ed_bstr->setGeometry(QRect(60, 300, 71, 22));
        ed_bstr->setFont(font);
        ed_bstr->setAlignment(Qt::AlignCenter);
        label_chan = new QLabel(widget);
        label_chan->setObjectName(QStringLiteral("label_chan"));
        label_chan->setEnabled(false);
        label_chan->setGeometry(QRect(0, 150, 51, 20));
        label_chan->setFont(font);
        pushButton_par = new QPushButton(widget);
        pushButton_par->setObjectName(QStringLiteral("pushButton_par"));
        pushButton_par->setGeometry(QRect(80, 0, 79, 31));
        pushButton_par->setFont(font);
        label_bstr = new QLabel(widget);
        label_bstr->setObjectName(QStringLiteral("label_bstr"));
        label_bstr->setGeometry(QRect(10, 300, 31, 21));
        label_bstr->setFont(font);
        label_nu = new QLabel(widget);
        label_nu->setObjectName(QStringLiteral("label_nu"));
        label_nu->setEnabled(false);
        label_nu->setGeometry(QRect(0, 170, 41, 21));
        label_nu->setFont(font);
        pushButt_wr_par = new QPushButton(widget);
        pushButt_wr_par->setObjectName(QStringLiteral("pushButt_wr_par"));
        pushButt_wr_par->setGeometry(QRect(0, 120, 181, 31));
        pushButt_wr_par->setFont(font);
        label_por = new QLabel(widget);
        label_por->setObjectName(QStringLiteral("label_por"));
        label_por->setEnabled(false);
        label_por->setGeometry(QRect(0, 410, 31, 21));
        label_por->setFont(font);
        ed_len_def = new param_ed(widget);
        ed_len_def->setObjectName(QStringLiteral("ed_len_def"));
        ed_len_def->setGeometry(QRect(60, 270, 71, 22));
        ed_len_def->setFont(font);
        pushButt__rd_par = new QPushButton(widget);
        pushButt__rd_par->setObjectName(QStringLiteral("pushButt__rd_par"));
        pushButt__rd_par->setGeometry(QRect(0, 90, 181, 31));
        pushButt__rd_par->setFont(font);
        pushButton_test1 = new QPushButton(widget);
        pushButton_test1->setObjectName(QStringLiteral("pushButton_test1"));
        pushButton_test1->setEnabled(false);
        pushButton_test1->setGeometry(QRect(0, 600, 51, 21));
        ed_kus = new param_ed(widget);
        ed_kus->setObjectName(QStringLiteral("ed_kus"));
        ed_kus->setEnabled(false);
        ed_kus->setGeometry(QRect(50, 190, 41, 22));
        ed_kus->setFont(font);
        ed_kus->setAlignment(Qt::AlignCenter);
        label_kus = new QLabel(widget);
        label_kus->setObjectName(QStringLiteral("label_kus"));
        label_kus->setEnabled(false);
        label_kus->setGeometry(QRect(10, 190, 31, 16));
        label_kus->setFont(font);
        label_lstr = new QLabel(widget);
        label_lstr->setObjectName(QStringLiteral("label_lstr"));
        label_lstr->setGeometry(QRect(10, 330, 20, 20));
        label_lstr->setFont(font);
        pushButton_test = new QPushButton(widget);
        pushButton_test->setObjectName(QStringLiteral("pushButton_test"));
        pushButton_test->setGeometry(QRect(0, 0, 81, 31));
        pushButton_test->setFont(font);
        label_noConn = new QLabel(widget);
        label_noConn->setObjectName(QStringLiteral("label_noConn"));
        label_noConn->setGeometry(QRect(0, 0, 81, 31));
        QFont font2;
        font2.setPointSize(12);
        label_noConn->setFont(font2);
        label_noConn->setAlignment(Qt::AlignCenter);
        ed_bstr_sp = new param_ed(widget);
        ed_bstr_sp->setObjectName(QStringLiteral("ed_bstr_sp"));
        ed_bstr_sp->setEnabled(false);
        ed_bstr_sp->setGeometry(QRect(30, 460, 61, 22));
        ed_bstr_sp->setFont(font);
        ed_bstr_sp->setAlignment(Qt::AlignCenter);
        label_lstr_sp = new QLabel(widget);
        label_lstr_sp->setObjectName(QStringLiteral("label_lstr_sp"));
        label_lstr_sp->setEnabled(false);
        label_lstr_sp->setGeometry(QRect(100, 460, 20, 20));
        label_lstr_sp->setFont(font);
        label_bstr_sp = new QLabel(widget);
        label_bstr_sp->setObjectName(QStringLiteral("label_bstr_sp"));
        label_bstr_sp->setEnabled(false);
        label_bstr_sp->setGeometry(QRect(0, 460, 31, 21));
        label_bstr_sp->setFont(font);
        ed_lstr_sp = new param_ed(widget);
        ed_lstr_sp->setObjectName(QStringLiteral("ed_lstr_sp"));
        ed_lstr_sp->setEnabled(false);
        ed_lstr_sp->setGeometry(QRect(130, 460, 61, 22));
        ed_lstr_sp->setFont(font);
        ed_lstr_sp->setAlignment(Qt::AlignCenter);
        checkBox_spectr = new QCheckBox(widget);
        checkBox_spectr->setObjectName(QStringLiteral("checkBox_spectr"));
        checkBox_spectr->setEnabled(false);
        checkBox_spectr->setGeometry(QRect(0, 440, 70, 17));
        checkBox_spectr->setFont(font);
        ed_spectr_const = new param_ed(widget);
        ed_spectr_const->setObjectName(QStringLiteral("ed_spectr_const"));
        ed_spectr_const->setEnabled(false);
        ed_spectr_const->setGeometry(QRect(130, 430, 61, 22));
        ed_spectr_const->setFont(font);
        ed_spectr_const->setAlignment(Qt::AlignCenter);
        checkBox_http = new QCheckBox(widget);
        checkBox_http->setObjectName(QStringLiteral("checkBox_http"));
        checkBox_http->setGeometry(QRect(10, 570, 70, 17));

        verticalLayout_3->addWidget(widget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout_3->addLayout(verticalLayout_3);

        ugen_z_dbgUI->setCentralWidget(centralWidget);
        QWidget::setTabOrder(pushButton_test, pushButton_par);
        QWidget::setTabOrder(pushButton_par, pushButt_load_par);
        QWidget::setTabOrder(pushButt_load_par, pushButt_save_par);
        QWidget::setTabOrder(pushButt_save_par, pushButt__rd_par);
        QWidget::setTabOrder(pushButt__rd_par, pushButt_wr_par);
        QWidget::setTabOrder(pushButt_wr_par, ed_chan);
        QWidget::setTabOrder(ed_chan, ed_nus);
        QWidget::setTabOrder(ed_nus, ed_kus);
        QWidget::setTabOrder(ed_kus, comboBox_rej);
        QWidget::setTabOrder(comboBox_rej, ed_pre);
        QWidget::setTabOrder(ed_pre, ed_rng);
        QWidget::setTabOrder(ed_rng, ed_por);
        QWidget::setTabOrder(ed_por, ed_bstr);
        QWidget::setTabOrder(ed_bstr, ed_lstr);
        QWidget::setTabOrder(ed_lstr, ed_len_def);
        QWidget::setTabOrder(ed_len_def, pushButton_test1);
        QWidget::setTabOrder(pushButton_test1, pushButt_debug);

        retranslateUi(ugen_z_dbgUI);

        QMetaObject::connectSlotsByName(ugen_z_dbgUI);
    } // setupUi

    void retranslateUi(QMainWindow *ugen_z_dbgUI)
    {
        ugen_z_dbgUI->setWindowTitle(QApplication::translate("ugen_z_dbgUI", "ugen_z_dbg", 0));
        label_gens->setText(QString());
        label_uss->setText(QString());
        labelStatus->setText(QString());
        label_vtime->setText(QApplication::translate("ugen_z_dbgUI", "Len def", 0));
        pushButt_debug->setText(QApplication::translate("ugen_z_dbgUI", "debug", 0));
        label_begr->setText(QApplication::translate("ugen_z_dbgUI", "Speed", 0));
        comboBox_rej->clear();
        comboBox_rej->insertItems(0, QStringList()
         << QApplication::translate("ugen_z_dbgUI", "\321\200\320\260\320\264\320\270\320\276", 0)
         << QApplication::translate("ugen_z_dbgUI", "\320\262\320\270\320\264\320\265\320\276", 0)
        );
        lab_num_par->setText(QApplication::translate("ugen_z_dbgUI", "1", 0));
        pushButt_save_par->setText(QApplication::translate("ugen_z_dbgUI", "\321\201\320\276\321\205\321\200. \320\262 \321\204\320\260\320\271\320\273", 0));
        label_endr->setText(QApplication::translate("ugen_z_dbgUI", "D pipe", 0));
        pushButt_load_par->setText(QApplication::translate("ugen_z_dbgUI", "\320\267\320\260\320\263\321\200. \320\270\320\267 \321\204\320\260\320\271\320\273\320\260", 0));
        label_chan->setText(QApplication::translate("ugen_z_dbgUI", "\320\232\320\260\320\275\320\260\320\273", 0));
        pushButton_par->setText(QApplication::translate("ugen_z_dbgUI", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", 0));
        label_bstr->setText(QApplication::translate("ugen_z_dbgUI", "\320\235\320\260\321\207", 0));
        label_nu->setText(QApplication::translate("ugen_z_dbgUI", "\320\243\321\201\320\270\320\273", 0));
        pushButt_wr_par->setText(QApplication::translate("ugen_z_dbgUI", "\321\201\320\276\321\205\321\200. \320\262 \320\277\321\200\320\270\320\261\320\276\321\200\320\265", 0));
        label_por->setText(QApplication::translate("ugen_z_dbgUI", "\320\237\320\276\321\200", 0));
        pushButt__rd_par->setText(QApplication::translate("ugen_z_dbgUI", "\320\267\320\260\320\263\321\200. \320\270\320\267 \320\277\321\200\320\270\320\261\320\276\321\200\320\260", 0));
        pushButton_test1->setText(QApplication::translate("ugen_z_dbgUI", "test1", 0));
        label_kus->setText(QApplication::translate("ugen_z_dbgUI", "K \321\203\321\201", 0));
        label_lstr->setText(QApplication::translate("ugen_z_dbgUI", "\320\224\320\273", 0));
        pushButton_test->setText(QApplication::translate("ugen_z_dbgUI", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214", 0));
        label_lstr_sp->setText(QApplication::translate("ugen_z_dbgUI", "\320\224\320\273", 0));
        label_bstr_sp->setText(QApplication::translate("ugen_z_dbgUI", "\320\235\320\260\321\207", 0));
        checkBox_spectr->setText(QApplication::translate("ugen_z_dbgUI", "\320\241\320\277\320\265\320\272\321\202\321\200", 0));
        checkBox_http->setText(QApplication::translate("ugen_z_dbgUI", "http", 0));
    } // retranslateUi

};

namespace Ui {
    class ugen_z_dbgUI: public Ui_ugen_z_dbgUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UGEN_Z_DBG_H
