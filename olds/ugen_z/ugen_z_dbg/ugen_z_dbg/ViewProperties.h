#ifndef VIEWPROPERTIES_H
#define VIEWPROPERTIES_H

#include <QDialog>
#include "ui_ViewProperies.h"
#include "DigitValidator.h"
#include "OscillogramProperties.h"
#include "QPushButtonColored.h"




class ViewProperties : public QDialog
{
	Q_OBJECT

private slots:
	void SetColumnNumber(QString);
	void SetRowNumber(QString);

	void SetBackgroundColor();
	void SetOscillogramColor();
	void SetCellColor();
	void SetTextColor();
	void SetStrobColor();
	void SetStrob2Color();
	void SetVRChColor();



	void SetSepLineColor();
	void SetBaseDefColor();
	void SetNoContactColor();
	void SetDefectColor();



signals:
	void PlotBackgroundChanged();
	void CloseMe();

public:
	ViewProperties(QWidget *parent, QObject *reseiver, OscillogramProperties *oscProp);
	virtual ~ViewProperties();

protected:
	virtual void closeEvent( QCloseEvent * event );

private:
	Ui::propertiesDialog PropInterface;

	DigitValidator DV2;

	OscillogramProperties *OscProp;

	inline int ToInt(QString &);
	
public:

	OscillogramProperties OscPropTemp;
};

#endif // VIEWPROPERTIES_H
