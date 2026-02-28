#ifndef _PARAM_ED_H_
#define _PARAM_ED_H_
 
///#include <string>
#include <QtWidgets/QtWidgets>
///#include <QtSql/QSqlDatabase>
////#include <QtSql/QtSql>
///#include <QtGui>
#define BYTE_SIZE	1
#define SHORT_SIZE	2
#define WORD_SIZE	4

class param_ed: public QLineEdit
{
    Q_OBJECT
public:
    param_ed(QWidget *parent=0);
 public:
///	virtual void resizeEvent(QResizeEvent * event);
 ///	virtual int heightForWidth(int w) const;

private:
///    QSqlDatabase m_db; //наша БД
	virtual void mouseDoubleClickEvent(QMouseEvent* pe);
	void	keyPressEvent(QKeyEvent * pe);
////virtual void mousePressEvent(QMouseEvent* pe);
private:
	int i_max;
	int i_min;
	int i_inc_step;
	unsigned char	*p_cdata;
	unsigned short	*p_sdata;
	unsigned long	*p_ldata;
	int num_dig;
	int coeff;
	int num_dights;

	int shift;

	bool ena_enter_on_off;
	int data_size;
	bool ena_edit;


///	char *bg;
///	char *col;
///	void set_color(void);
public:
	void set_ena_edit(bool iena_edit);
	void inc_par();
	void dec_par();
	void set_data_size(int idata_size);
	void set_ena_enter_on_off(bool ena);
	void set_min_max(int i_min,int i_max);
	void set_inc_step(int istep);
	void set_num_dig(int inum_dig);
	void set_coeff(int icoeff);
	void set_data(unsigned char *p_data);
	void set_data(unsigned short *p_data);
	void set_data(unsigned long *p_data);
	void set_colors(const char *bg,const char *col);
	void show_par();

	void set_shift(int);


signals:
	void on_off();
	void param_changed();
};
 
#endif // _PARAM_ED_H_