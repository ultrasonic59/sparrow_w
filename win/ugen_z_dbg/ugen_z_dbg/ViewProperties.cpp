#include "ViewProperties.h"
#include <QColorDialog>
#include <QtGui/QPainter>

ViewProperties::ViewProperties(QWidget *parent, QObject *reseiver, OscillogramProperties *oscProp) :
		QDialog(parent),
		PropInterface(),
		DV2(this, 2),
		OscProp(oscProp),
		OscPropTemp(*OscProp)
{
	PropInterface.setupUi(this);

	PropInterface.Button_col_background->SetColor(OscProp->BackgroundColor);
	PropInterface.Button_col_osc->SetColor(OscProp->OscillogramColor);
	PropInterface.Button_col_cell->SetColor(OscProp->CellColor);
    PropInterface.Button_col_text->SetColor(OscProp->TextColor);
	PropInterface.Button_col_strob->SetColor(OscProp->StrobsColor[0]);
	PropInterface.Button_col_strob2->SetColor(OscProp->StrobsColor[1]);

	PropInterface.Button_col_vrch->SetColor(OscProp->VRChColor);
	PropInterface.Button_col_sepLine->SetColor(OscProp->SepLineColor);
	PropInterface.Button_col_baseDef->SetColor(OscProp->BaseDefColor);
	PropInterface.Button_col_noContact->SetColor(OscProp->NoContactColor);
	PropInterface.Button_col_defect->SetColor(OscProp->DefectColor);


	setWindowFlags(Qt::Drawer);

	PropInterface.lineCellRow->setText(QString::number(OscProp->row_number));
	PropInterface.lineCellColumn->setText(QString::number(OscProp->column_number));

	PropInterface.lineCellRow->setValidator(&DV2);
	PropInterface.lineCellColumn->setValidator(&DV2);

	PropInterface.label_col_vrch->hide();
	PropInterface.Button_col_vrch->hide();

	PropInterface.Layout_col_vrch->setContentsMargins(-1, -1, -1, -1);


	QObject::connect(PropInterface.lineCellColumn, SIGNAL(textChanged(QString)), this, SLOT(SetColumnNumber(QString)));
    QObject::connect(PropInterface.lineCellRow, SIGNAL(textChanged(QString)), this, SLOT(SetRowNumber(QString)));

	QObject::connect(PropInterface.okButton, SIGNAL(clicked()), reseiver, SLOT(NewWindowParameters()));
	QObject::connect(PropInterface.cancelButton, SIGNAL(clicked()), reseiver, SLOT(CloseWindowParameters()));

	QObject::connect(this, SIGNAL(CloseMe()), reseiver, SLOT(CloseWindowParameters()));
	QObject::connect(this, SIGNAL(PlotBackgroundChanged()), reseiver, SLOT(ResetPlot()));

	QObject::connect(PropInterface.Button_col_background, SIGNAL(clicked()), this, SLOT(SetBackgroundColor()));
	QObject::connect(PropInterface.Button_col_osc, SIGNAL(clicked()), this, SLOT(SetOscillogramColor()));
	QObject::connect(PropInterface.Button_col_cell, SIGNAL(clicked()), this, SLOT(SetCellColor()));
	QObject::connect(PropInterface.Button_col_text, SIGNAL(clicked()), this, SLOT(SetTextColor()));
	QObject::connect(PropInterface.Button_col_strob, SIGNAL(clicked()), this, SLOT(SetStrobColor()));
	QObject::connect(PropInterface.Button_col_strob2, SIGNAL(clicked()), this, SLOT(SetStrob2Color()));
	QObject::connect(PropInterface.Button_col_vrch, SIGNAL(clicked()), this, SLOT(SetVRChColor()));
	QObject::connect(PropInterface.Button_col_sepLine, SIGNAL(clicked()), this, SLOT(SetSepLineColor()));
	QObject::connect(PropInterface.Button_col_baseDef, SIGNAL(clicked()), this, SLOT(SetBaseDefColor()));
	QObject::connect(PropInterface.Button_col_noContact, SIGNAL(clicked()), this, SLOT(SetNoContactColor()));
	QObject::connect(PropInterface.Button_col_defect, SIGNAL(clicked()), this, SLOT(SetDefectColor()));


}

ViewProperties::~ViewProperties()
{
}

void ViewProperties::closeEvent( QCloseEvent * event )
{
	QDialog::closeEvent(event);
	CloseMe();
}

void ViewProperties::SetColumnNumber(QString str)
{
	OscProp->column_number = ToInt(str);
	emit PlotBackgroundChanged();
}

void ViewProperties::SetRowNumber(QString str)
{
	OscProp->row_number = ToInt(str);
	emit PlotBackgroundChanged();
}


int ViewProperties::ToInt(QString &str)
{
	if(str == "")
		return 0;
	else
		return str.toInt();
}

void ViewProperties::SetBackgroundColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->BackgroundColor);

	if(tempColor.isValid())
	{
		OscProp->BackgroundColor = tempColor;
		PropInterface.Button_col_background->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}

void ViewProperties::SetOscillogramColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->OscillogramColor);

	if(tempColor.isValid())
	{
		OscProp->OscillogramColor = tempColor;
		PropInterface.Button_col_osc->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}

void ViewProperties::SetCellColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->CellColor);

	if(tempColor.isValid())
	{
		OscProp->CellColor = tempColor;
		PropInterface.Button_col_cell->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}

void ViewProperties::SetTextColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->TextColor);

	if(tempColor.isValid())
	{
		OscProp->TextColor = tempColor;
		PropInterface.Button_col_text->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}

void ViewProperties::SetStrobColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->StrobsColor[0]);

	if(tempColor.isValid())
	{
		OscProp->StrobsColor[0] = tempColor;
		PropInterface.Button_col_strob->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}

void ViewProperties::SetStrob2Color()
{
	QColor tempColor = QColorDialog::getColor(OscProp->StrobsColor[1]);

	if(tempColor.isValid())
	{
		OscProp->StrobsColor[1] = tempColor;
		PropInterface.Button_col_strob2->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}

void ViewProperties::SetVRChColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->VRChColor);

	if(tempColor.isValid())
	{
		OscProp->VRChColor = tempColor;
		PropInterface.Button_col_vrch->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}


void ViewProperties::SetSepLineColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->SepLineColor);

	if(tempColor.isValid())
	{
		OscProp->SepLineColor = tempColor;
		PropInterface.Button_col_sepLine->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}

void ViewProperties::SetBaseDefColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->BaseDefColor);

	if(tempColor.isValid())
	{
		OscProp->BaseDefColor = tempColor;
		PropInterface.Button_col_baseDef->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}

void ViewProperties::SetNoContactColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->NoContactColor);

	if(tempColor.isValid())
	{
		OscProp->NoContactColor = tempColor;
		PropInterface.Button_col_noContact->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}


void ViewProperties::SetDefectColor()
{
	QColor tempColor = QColorDialog::getColor(OscProp->DefectColor);

	if(tempColor.isValid())
	{
		OscProp->DefectColor = tempColor;
		PropInterface.Button_col_defect->SetColor(tempColor);
		emit PlotBackgroundChanged();
	}
}