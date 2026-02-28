/********************************************************************************
** Form generated from reading UI file 'ViewProperies.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWPROPERIES_H
#define UI_VIEWPROPERIES_H

#include <QPushButtonColored.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_propertiesDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_cell;
    QHBoxLayout *Layout_cell;
    QLabel *label_cell_x;
    QLineEdit *lineCellColumn;
    QLabel *label_cell_y;
    QLineEdit *lineCellRow;
    QHBoxLayout *Layout_col_background;
    QLabel *lab_col_background;
    QPushButtonColored *Button_col_background;
    QHBoxLayout *Layout_col_ocs;
    QLabel *label_col_os;
    QPushButtonColored *Button_col_osc;
    QHBoxLayout *Layout_col_cell;
    QLabel *label_col_cell;
    QPushButtonColored *Button_col_cell;
    QHBoxLayout *Layout_col_text;
    QLabel *label_col_text;
    QPushButtonColored *Button_col_text;
    QHBoxLayout *Layout_col_strob;
    QLabel *label_col_strob;
    QPushButtonColored *Button_col_strob;
    QHBoxLayout *Layout_col_strob2;
    QLabel *label_col_strob2;
    QPushButtonColored *Button_col_strob2;
    QHBoxLayout *Layout_col_vrch;
    QLabel *label_col_vrch;
    QPushButtonColored *Button_col_vrch;
    QHBoxLayout *Layout_col_sepLine;
    QLabel *label_col_sepLine;
    QPushButtonColored *Button_col_sepLine;
    QHBoxLayout *Layout_col_baseDef;
    QLabel *label_col_baseDef;
    QPushButtonColored *Button_col_baseDef;
    QHBoxLayout *Layout_col_noContact;
    QLabel *label_col_noContact;
    QPushButtonColored *Button_col_noContact;
    QHBoxLayout *Layout_col_defect;
    QLabel *label_col_defect;
    QPushButtonColored *Button_col_defect;
    QHBoxLayout *Layout_buttons;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *propertiesDialog)
    {
        if (propertiesDialog->objectName().isEmpty())
            propertiesDialog->setObjectName(QStringLiteral("propertiesDialog"));
        propertiesDialog->resize(260, 435);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(propertiesDialog->sizePolicy().hasHeightForWidth());
        propertiesDialog->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(propertiesDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 7, -1, -1);
        label_cell = new QLabel(propertiesDialog);
        label_cell->setObjectName(QStringLiteral("label_cell"));
        label_cell->setMaximumSize(QSize(16777215, 29));
        QFont font;
        font.setPointSize(10);
        label_cell->setFont(font);
        label_cell->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_cell);

        Layout_cell = new QHBoxLayout();
        Layout_cell->setObjectName(QStringLiteral("Layout_cell"));
        label_cell_x = new QLabel(propertiesDialog);
        label_cell_x->setObjectName(QStringLiteral("label_cell_x"));
        label_cell_x->setFont(font);
        label_cell_x->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        Layout_cell->addWidget(label_cell_x);

        lineCellColumn = new QLineEdit(propertiesDialog);
        lineCellColumn->setObjectName(QStringLiteral("lineCellColumn"));
        lineCellColumn->setFont(font);
        lineCellColumn->setAlignment(Qt::AlignCenter);

        Layout_cell->addWidget(lineCellColumn);

        label_cell_y = new QLabel(propertiesDialog);
        label_cell_y->setObjectName(QStringLiteral("label_cell_y"));

        Layout_cell->addWidget(label_cell_y);

        lineCellRow = new QLineEdit(propertiesDialog);
        lineCellRow->setObjectName(QStringLiteral("lineCellRow"));
        lineCellRow->setFont(font);
        lineCellRow->setAlignment(Qt::AlignCenter);

        Layout_cell->addWidget(lineCellRow);


        verticalLayout->addLayout(Layout_cell);

        Layout_col_background = new QHBoxLayout();
        Layout_col_background->setObjectName(QStringLiteral("Layout_col_background"));
        lab_col_background = new QLabel(propertiesDialog);
        lab_col_background->setObjectName(QStringLiteral("lab_col_background"));
        lab_col_background->setFont(font);
        lab_col_background->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        Layout_col_background->addWidget(lab_col_background);

        Button_col_background = new QPushButtonColored(propertiesDialog);
        Button_col_background->setObjectName(QStringLiteral("Button_col_background"));

        Layout_col_background->addWidget(Button_col_background);


        verticalLayout->addLayout(Layout_col_background);

        Layout_col_ocs = new QHBoxLayout();
        Layout_col_ocs->setObjectName(QStringLiteral("Layout_col_ocs"));
        label_col_os = new QLabel(propertiesDialog);
        label_col_os->setObjectName(QStringLiteral("label_col_os"));
        label_col_os->setFont(font);

        Layout_col_ocs->addWidget(label_col_os);

        Button_col_osc = new QPushButtonColored(propertiesDialog);
        Button_col_osc->setObjectName(QStringLiteral("Button_col_osc"));

        Layout_col_ocs->addWidget(Button_col_osc);


        verticalLayout->addLayout(Layout_col_ocs);

        Layout_col_cell = new QHBoxLayout();
        Layout_col_cell->setObjectName(QStringLiteral("Layout_col_cell"));
        label_col_cell = new QLabel(propertiesDialog);
        label_col_cell->setObjectName(QStringLiteral("label_col_cell"));
        label_col_cell->setFont(font);

        Layout_col_cell->addWidget(label_col_cell);

        Button_col_cell = new QPushButtonColored(propertiesDialog);
        Button_col_cell->setObjectName(QStringLiteral("Button_col_cell"));

        Layout_col_cell->addWidget(Button_col_cell);


        verticalLayout->addLayout(Layout_col_cell);

        Layout_col_text = new QHBoxLayout();
        Layout_col_text->setObjectName(QStringLiteral("Layout_col_text"));
        label_col_text = new QLabel(propertiesDialog);
        label_col_text->setObjectName(QStringLiteral("label_col_text"));
        label_col_text->setFont(font);

        Layout_col_text->addWidget(label_col_text);

        Button_col_text = new QPushButtonColored(propertiesDialog);
        Button_col_text->setObjectName(QStringLiteral("Button_col_text"));

        Layout_col_text->addWidget(Button_col_text);


        verticalLayout->addLayout(Layout_col_text);

        Layout_col_strob = new QHBoxLayout();
        Layout_col_strob->setObjectName(QStringLiteral("Layout_col_strob"));
        label_col_strob = new QLabel(propertiesDialog);
        label_col_strob->setObjectName(QStringLiteral("label_col_strob"));
        label_col_strob->setFont(font);

        Layout_col_strob->addWidget(label_col_strob);

        Button_col_strob = new QPushButtonColored(propertiesDialog);
        Button_col_strob->setObjectName(QStringLiteral("Button_col_strob"));

        Layout_col_strob->addWidget(Button_col_strob);


        verticalLayout->addLayout(Layout_col_strob);

        Layout_col_strob2 = new QHBoxLayout();
        Layout_col_strob2->setObjectName(QStringLiteral("Layout_col_strob2"));
        label_col_strob2 = new QLabel(propertiesDialog);
        label_col_strob2->setObjectName(QStringLiteral("label_col_strob2"));
        label_col_strob2->setFont(font);

        Layout_col_strob2->addWidget(label_col_strob2);

        Button_col_strob2 = new QPushButtonColored(propertiesDialog);
        Button_col_strob2->setObjectName(QStringLiteral("Button_col_strob2"));

        Layout_col_strob2->addWidget(Button_col_strob2);


        verticalLayout->addLayout(Layout_col_strob2);

        Layout_col_vrch = new QHBoxLayout();
        Layout_col_vrch->setObjectName(QStringLiteral("Layout_col_vrch"));
        label_col_vrch = new QLabel(propertiesDialog);
        label_col_vrch->setObjectName(QStringLiteral("label_col_vrch"));
        label_col_vrch->setFont(font);

        Layout_col_vrch->addWidget(label_col_vrch);

        Button_col_vrch = new QPushButtonColored(propertiesDialog);
        Button_col_vrch->setObjectName(QStringLiteral("Button_col_vrch"));

        Layout_col_vrch->addWidget(Button_col_vrch);


        verticalLayout->addLayout(Layout_col_vrch);

        Layout_col_sepLine = new QHBoxLayout();
        Layout_col_sepLine->setObjectName(QStringLiteral("Layout_col_sepLine"));
        label_col_sepLine = new QLabel(propertiesDialog);
        label_col_sepLine->setObjectName(QStringLiteral("label_col_sepLine"));
        label_col_sepLine->setFont(font);

        Layout_col_sepLine->addWidget(label_col_sepLine);

        Button_col_sepLine = new QPushButtonColored(propertiesDialog);
        Button_col_sepLine->setObjectName(QStringLiteral("Button_col_sepLine"));

        Layout_col_sepLine->addWidget(Button_col_sepLine);


        verticalLayout->addLayout(Layout_col_sepLine);

        Layout_col_baseDef = new QHBoxLayout();
        Layout_col_baseDef->setObjectName(QStringLiteral("Layout_col_baseDef"));
        label_col_baseDef = new QLabel(propertiesDialog);
        label_col_baseDef->setObjectName(QStringLiteral("label_col_baseDef"));
        label_col_baseDef->setFont(font);

        Layout_col_baseDef->addWidget(label_col_baseDef);

        Button_col_baseDef = new QPushButtonColored(propertiesDialog);
        Button_col_baseDef->setObjectName(QStringLiteral("Button_col_baseDef"));

        Layout_col_baseDef->addWidget(Button_col_baseDef);


        verticalLayout->addLayout(Layout_col_baseDef);

        Layout_col_noContact = new QHBoxLayout();
        Layout_col_noContact->setObjectName(QStringLiteral("Layout_col_noContact"));
        label_col_noContact = new QLabel(propertiesDialog);
        label_col_noContact->setObjectName(QStringLiteral("label_col_noContact"));
        label_col_noContact->setFont(font);

        Layout_col_noContact->addWidget(label_col_noContact);

        Button_col_noContact = new QPushButtonColored(propertiesDialog);
        Button_col_noContact->setObjectName(QStringLiteral("Button_col_noContact"));

        Layout_col_noContact->addWidget(Button_col_noContact);


        verticalLayout->addLayout(Layout_col_noContact);

        Layout_col_defect = new QHBoxLayout();
        Layout_col_defect->setObjectName(QStringLiteral("Layout_col_defect"));
        label_col_defect = new QLabel(propertiesDialog);
        label_col_defect->setObjectName(QStringLiteral("label_col_defect"));
        label_col_defect->setFont(font);

        Layout_col_defect->addWidget(label_col_defect);

        Button_col_defect = new QPushButtonColored(propertiesDialog);
        Button_col_defect->setObjectName(QStringLiteral("Button_col_defect"));

        Layout_col_defect->addWidget(Button_col_defect);


        verticalLayout->addLayout(Layout_col_defect);

        Layout_buttons = new QHBoxLayout();
        Layout_buttons->setObjectName(QStringLiteral("Layout_buttons"));
        okButton = new QPushButton(propertiesDialog);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setFont(font);

        Layout_buttons->addWidget(okButton);

        cancelButton = new QPushButton(propertiesDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setFont(font);

        Layout_buttons->addWidget(cancelButton);


        verticalLayout->addLayout(Layout_buttons);


        retranslateUi(propertiesDialog);

        QMetaObject::connectSlotsByName(propertiesDialog);
    } // setupUi

    void retranslateUi(QDialog *propertiesDialog)
    {
        propertiesDialog->setWindowTitle(QApplication::translate("propertiesDialog", "\320\246\320\262\320\265\321\202\320\276\320\262\320\260\321\217 \321\201\321\205\320\265\320\274\320\260", 0));
        label_cell->setText(QApplication::translate("propertiesDialog", "\320\241\320\265\321\202\320\272\320\260", 0));
        label_cell_x->setText(QApplication::translate("propertiesDialog", "x", 0));
        lineCellColumn->setText(QApplication::translate("propertiesDialog", "10", 0));
        label_cell_y->setText(QApplication::translate("propertiesDialog", "y", 0));
        lineCellRow->setText(QApplication::translate("propertiesDialog", "6", 0));
        lab_col_background->setText(QApplication::translate("propertiesDialog", "\320\244\320\276\320\275", 0));
        label_col_os->setText(QApplication::translate("propertiesDialog", "\320\236\321\201\321\206\320\270\320\273\320\273\320\276\320\263\321\200\320\260\320\274\320\274\320\260", 0));
        label_col_cell->setText(QApplication::translate("propertiesDialog", "\320\241\320\265\321\202\320\272\320\260", 0));
        label_col_text->setText(QApplication::translate("propertiesDialog", "\320\242\320\265\320\272\321\201\321\202", 0));
        label_col_strob->setText(QApplication::translate("propertiesDialog", "\320\241\321\202\321\200\320\276\320\261 1", 0));
        label_col_strob2->setText(QApplication::translate("propertiesDialog", "\320\241\321\202\321\200\320\276\320\261 2", 0));
        label_col_vrch->setText(QApplication::translate("propertiesDialog", "\320\222\320\240\320\247", 0));
        label_col_sepLine->setText(QApplication::translate("propertiesDialog", "\320\240\320\260\320\267\320\264\320\265\320\273\320\270\321\202\320\265\320\273\321\214", 0));
        label_col_baseDef->setText(QApplication::translate("propertiesDialog", "\320\235\320\276\321\200\320\274\320\260", 0));
        label_col_noContact->setText(QApplication::translate("propertiesDialog", "\320\235\320\265\321\202 \320\272\320\276\320\275\321\202\320\260\320\272\321\202\320\260", 0));
        label_col_defect->setText(QApplication::translate("propertiesDialog", "\320\224\320\265\321\204\320\265\320\272\321\202", 0));
        okButton->setText(QApplication::translate("propertiesDialog", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", 0));
        cancelButton->setText(QApplication::translate("propertiesDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));
    } // retranslateUi

};

namespace Ui {
    class propertiesDialog: public Ui_propertiesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWPROPERIES_H
