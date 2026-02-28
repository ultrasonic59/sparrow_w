#ifndef PARAM_ED_ENDR_H
#define PARAM_ED_ENDR_H

#include <QtWidgets/QtWidgets>

class param_ed_endr : public QLineEdit
{
Q_OBJECT	

public:
	param_ed_endr(QWidget *parent);


private:
	void	keyPressEvent(QKeyEvent * pe);

private:
	int i_max;
	int i_min;
	int i_inc_step;

	unsigned char *p_cdata;

	int num_dig;
	int coeff;
	int num_dights;


	int Result;


public:
	void set_ena_edit(bool iena_edit);

	void inc_par();
	void dec_par();

	void set_min_max(int i_min,int i_max);
	void set_inc_step(int istep);
	void set_num_dig(int inum_dig);

	void set_coeff(int icoeff);
	void set_data(unsigned char *p_data);

	void show_par(int val);

	int GetResult();


signals:
	void param_changed();
	void result_changed();
	
};

#endif // PARAM_ED_ENDR_H
