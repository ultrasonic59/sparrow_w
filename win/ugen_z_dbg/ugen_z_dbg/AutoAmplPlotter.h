#pragma once
#include "abstactplotter.h"
#include "RespondFrame.h"
#include "OscillogramProperties.h"


class AutoAmplPlotter : public AbstactPlotter
{
protected:

	bool AnyReceived;

	RespondFrame *pOldFrame;

	int *pNew_frame_len;
	int *pOld_frame_len;

	const int pix_per_len;

	const int left_tab;
	const int tact_text_size;

	const int sep_line_W;


	const int defect_height;
	const int defect_base_y;


	const float MaxAmpl;
	const int plot_line_W;

	const int plot_base_y;
	float plot_base1_y;
	float plot_base_step_y;

	float plot_step_x;
	float plot_step_y;


	OscillogramProperties *OscProp;



	inline void OutString(QPainter & painter, const int x_middle, const int y_base, const int x_size, const int y_size, const bool clear, QString &str);
	inline void PlotDefectsLine(QPainter & painter, const float curr_x, const quint16 def_type);
	inline void OutTactLables(QPainter & painter);
	inline void PlotDefects(QPainter & painter);
	inline void PlotBackgroundLines(QPainter & painter);


	quint8 us_on[NUM_CHANS];
	quint8 curr_chan_num;

	inline bool UsChange(void);
	inline void ApplyChanChange(void);

public:

	AutoAmplPlotter(RespondFrame *OldFrame, int *New_frame_len, int *Old_frame_len, OscillogramProperties *newOscProp);
	virtual ~AutoAmplPlotter(void);


	virtual QRect & SetPlotRect(const QRect &);

	void PaintBackground(void);

	void PlotOldRespond(const bool clearing);
	void PlotNewRespond(void);


};

