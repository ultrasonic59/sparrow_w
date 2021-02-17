#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "OscillogramPlotterSetter.h"
#include "ui_q_red_pit_control.h"

#include "obj_cmd.h"



class q_red_pit_control : public QMainWindow
{
    Q_OBJECT

protected:
    Ui::q_red_pit_control ui;


	quint16 plot_arr_length;
	QVector<qint16> plot_array;

    OscillogramProperties osc_prop;
    OscillogramPlotterSetter plotter;


	quint32 dev_freq;


	inline void saveSettings();
	inline void loadSettings();


	inline void RecalculateImpulse();


	inline void ImpulseToPlot();


public:
    q_red_pit_control(QWidget *parent = 0);
    ~q_red_pit_control();



protected slots:

    void OnStartStop();


	void InitPlot();



	void osc_length_changed();

	void Timp_len_changed();
	void Timp_offset_changed();
	void Tcycle_changed();
	void Tdef_changed();
	void pre_changed();

	void NumPeriods_changed();


	void Aimp_changed();
	void DevFreq_changed();


	void GaussClicked();


public slots:

	void SetConnStatus();
	void EndInitConnection();
	void NoConnection();

public:

	c_obj_cmd device_CMD;


};

#endif // MAINWINDOW_H
