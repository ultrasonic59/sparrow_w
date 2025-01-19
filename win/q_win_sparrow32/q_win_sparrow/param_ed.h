#ifndef _PARAM_ED_H_
#define _PARAM_ED_H_
 
#include <QtWidgets/QtWidgets>


class param_ed: public QLineEdit
{
Q_OBJECT



protected:

	virtual void mouseDoubleClickEvent(QMouseEvent* pe);
	virtual void keyPressEvent(QKeyEvent * pe);

	int i_max;
	int i_min;
	int i_inc_step;
	unsigned char *p_cdata;
	unsigned short *p_sdata;
	unsigned long *p_ldata;

	bool any_elem;

	int num_dig;
	int coeff;
	int num_dights;

	int shift;

	bool ena_enter_on_off;
	bool ena_edit;


	inline void ApplyNewText();

	inline int GetMinimalWidth() const;

private:

	template<typename T> inline void inc_par_val(T *value, const int count);
	template<typename T> inline void dec_par_val(T *value, const int count);


public:

    param_ed(QWidget *parent=0);

	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;


	void set_ena_edit(bool iena_edit);

	void inc_par(const int count = 1);
	void dec_par(const int count = 1);


	void set_ena_enter_on_off(bool ena);
	void set_min_max(int i_min,int i_max);
	void set_inc_step(int istep);
	void set_num_dig(int inum_dig);
	void set_coeff(int icoeff);
	void set_data(unsigned char *p_data);
	void set_data(unsigned short *p_data);
	void set_data(unsigned long *p_data);
	void set_colors(const char *col);
	void show_par();

	void set_shift(int);

	void apply_current();


signals:
	void on_off();
	void param_changed();
};



template<typename T> void param_ed::inc_par_val(T *value, const int count)
{
	T inc_data = *value + i_inc_step*count;

	if((*value + i_inc_step*count) > i_max)
		*value = i_max;
	else if(inc_data <= i_min)
		*value = i_min;
	else 
		*value = inc_data;		
}

template<typename T> void param_ed::dec_par_val(T *value, const int count)
{
	if(*value >= (i_max + i_inc_step*count))
		*value = i_max;
	else if(*value >= (i_min + i_inc_step*count))
		*value -= i_inc_step*count;
	else
		*value = i_min;
}

 
#endif // _PARAM_ED_H_