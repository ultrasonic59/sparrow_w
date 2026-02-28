#ifndef DIAL_PAR_H
#define DIAL_PAR_H

#include <QCloseEvent>
#include <QtWidgets/QtWidgets>
#include <QPluginLoader>

#include "ui_dial_par.h"
#include "params.h"

namespace Ui {
    class Dialog_par;
}

class DialPar : public QDialog
{
Q_OBJECT
#if 0
private slots:
signals:
#endif
private:
////	cn_info_t	cn_info[NUM_CHANS];		// описание каждого (такто)канала тракта

protected:
	virtual void closeEvent( QCloseEvent * event );

public:
////	    explicit DialPhoto(QWidget *parent = 0);

	Ui::Dialog_par *ui;

	DialPar(QWidget *parent= 0);
	virtual ~DialPar();
////	void  set_chans(cn_info_t *cn_info);
////	void  get_chans(cn_info_t *cn_info);
	void  get_ip_addr(QString  *o_addr);
	void  set_ip_addr(QString addr);

	void  get_on_ed_us(bool  *on_us);
	void  set_on_ed_us(bool  on_us);
	void  get_on_ed_tact(bool  *on_tact);
	void  set_on_ed_tact(bool  on_tact);

	void EnableParams(bool);
///private slots:
///	void on_send_butt();
};

#endif // DIAL_ED_PAR_H
