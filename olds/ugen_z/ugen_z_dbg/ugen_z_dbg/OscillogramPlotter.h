#pragma once

#include "respond.h"
#include "RespondTrans.h"
#include "OscillogramProperties.h"
#include "positiontesterplotter.h"
#define LEN_OSC 256			///???
#define MAX_OSC 255			

#define STEP_KOEF 0.05f		
#define TEN_F  10.0f

class OscillogramPlotter : public PositionTesterPlotter
{
Q_OBJECT

protected:

	bool AnyReceived;

	int width;
	int height;

	float x_b;
	float y_b;

	float x_step;
	float y_step;

	const int x_sign_step;
	const int y_sign_step;
	const int RedrawAdjustX;
	const int RedrawAdjustY;

	const int frame_width;

	const int ArrowDim;
	const int ArrowIndent;
	const int ArrowInterval;

	const int StrobWidth;
	const int StrobSetHeight;

	const float VRChPart;


	RespondTrans *Translator;
	OscillogramProperties *OscProp;

	Respond *pOldRespond;



	inline void DrawGrid(QPainter &);
	inline void PlotRespond(QPainter & painter, const Respond &, bool clean);
	inline void OutFrameText(QPainter & painter, const QString & string, const int x_rightMid, const int y_rightMid, const int size, const int legth, const bool redraw);

	inline void PlotVerticalSignature(QPainter & painter, const bool FirstEl);
	inline void PlotHorizontalSignature(QPainter & painter, const Respond &respond, const bool clean, const bool FirstEl);

	inline void PlotStrob(QPainter & painter, const float koeff, float s_begin, float s_end, const float porog1, QColor color, const bool selected, bool clean);
	inline void PlotStrobArrow(QPainter & painter, const float x0, const float y0, const bool left);
	inline void PlotStrobSetEnd(QPainter & painter, const float x0, const float y0);

	inline void PlotVRCh(QPainter & painter, const Respond &respond, const bool MKSMode, const float koeff, const float cos_al, const float OscBegin, bool clean);
	inline float GetVRChPorog(const Respond &respond, const quint8 i, const quint8 max_val) const;


	virtual void PlotTempContent(QPainter & painter);

	inline bool IsInsidePlot(const float x, const float y);
	inline bool IsInsideBeginOsc(const float x, const float y);
	inline bool IsInsideEndOsc(const float x, const float y);

	inline bool IsInsideStrobVisible(const float koeff, const float s_begin, const float s_end);

	inline bool IsInsideStrobLeft(const float koeff, const float s_begin, const float s_end, const float s_porog, const float x, const float y);
	inline bool IsInsideStrobMiggle(const float koeff, const float s_begin, const float s_end, const float s_porog, const float x, const float y);
	inline bool IsInsideStrobRight(const float koeff, const float s_begin, const float s_end, const float s_porog, const float x, const float y);


	virtual void DragStarted();
	virtual void DragEnded();


	enum cursor_area {
		Outside = 0,

		InBeginOsc = 1,
		InEndOsc = 2,

		InStrobLeft = 3,
		InStrobMiggle = 4,
		InStrobRight = 5


	}  curr_area;

	int curr_strob;


	Respond RefferenceRespond;



	inline int CalculateCurrOscBeg();
	inline int CalculateCurrOscEnd();


	inline int CalculateCurrStrobLen();


public:


	OscillogramPlotter(	RespondTrans *, 
						OscillogramProperties *, 
						Respond *);

	virtual ~OscillogramPlotter(void);

	virtual QRect & SetPlotRect(const QRect &);
	void ResetPlot();

	void PrintScaleLimit(int limit);

	void PaintBackground(void);
	void PlotRespond(const Respond &New, const Respond &Old);

	virtual void TestNewPosition(const QPointF &rel_pos);


signals:

	void BegOscIntermedChange(int new_beg);
	void BegOscFinalChange();
	void EndOscIntermedChange(int new_end);
	void EndOscFinalChange();
	void SetStrobIntermedPos(int strob, int begin, int porog);
	void SetStrobFinalPos();
	void SetStrobIntermedLeft(int strob, int begin, int len);
	void SetStrobFinalLeft();
	void SetStrobIntermedRight(int strob, int len);
	void SetStrobFinalRight();


};