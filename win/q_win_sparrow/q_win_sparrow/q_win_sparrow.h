#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "OscillogramPlotterSetter.h"
#include "OscillogramPlotterSetter2.h"
#include "ui_q_win_sparrow.h"

#include "obj_cmd.h"

#include "dial_debug.h"



class q_win_sparrow : public QMainWindow
{
    Q_OBJECT

protected:
    Ui::q_win_sparrow ui;


	quint16 plot_arr_length;
	QVector<qint16> plot_array;

    OscillogramProperties osc_prop;
    OscillogramPlotterSetter plotter;
	OscillogramPlotterSetter2 plotter_osc;


	quint32 dev_freq;


	DialDebug dial_dbg;



	inline void saveSettings();
	inline void loadSettings();


	inline void RecalculateImpulse();


	inline void ImpulseToPlot();


public:
    q_win_sparrow(QWidget *parent = 0);
    ~q_win_sparrow();



protected slots:

    void OnStartStop();


	void InitPlot();



	void osc_length_changed();

	void Timp_len_changed();
	void Timp_offset_changed();
	void Tcycle_changed();
	void Tdef_changed();

	void kus_changed();
	void beg_osc_changed();
	void step_osc_changed();



	void NumPeriods_changed();


	void Aimp_changed();
	void DevFreq_changed();


	void AttenLev0Clicked();
	void AttenLev1Clicked();


	void GaussClicked();


public slots:

	void SetConnStatus();
	void EndInitConnection();
	void NoConnection();

	void slot_draw_osc();
	void slot_new_ampl();



protected slots:

	void butt_debug();
	void slot_rd_xil_dat(xil_dat_req_t* odat);
	void slot_wr_xil_dat(xil_dat_req_t* idat);
	void slot_rd_dac_dat(dac_spi_req_t* odat);
	void slot_wr_dac_dat(dac_spi_req_t* idat);
	void slot_wr_adc_dat(adc_spi_req_t* idat);
	void slot_rd_adc_dat(adc_spi_req_t* idat);

signals:

	void put_xil_dat_dial(xil_dat_req_t*);
	void put_dac_dat_dial(dac_spi_req_t*);
	void put_adc_dat_dial(adc_spi_req_t*);


public:

	c_obj_cmd dev_obj;


};

#endif // MAINWINDOW_H
