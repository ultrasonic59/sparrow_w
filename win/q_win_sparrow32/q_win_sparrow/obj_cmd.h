#ifndef OBJ_CMD_H
#define OBJ_CMD_H

#include <QObject>
#include "params.h"

#include "udp_cmd.h"

#include "dev_cmd.h"
#include "tune_thr.h"




class c_obj_cmd : public QObject
{
Q_OBJECT

public:
	c_obj_cmd(QObject *parent);
	virtual ~c_obj_cmd();

protected:
	QObject *p_parent;


	QThread *p_thr_udp;

	QThread *p_thr_tune;

	c_udp_cmd *p_udp_cmd;


public:


	QString ip_addr;
	int port;



// device params

	quint32 g_changed_param;
	par_contr_t curr_par_contr;
	QVector<qint16> imp_ampl;

	osc_struct_t osc_struct;
	quint16 ampl_val;



	c_tune_thr *p_tune_thr;



	void SetupDevice();
	void StopDevice();
	void UpdateDevice(const bool update);


	bool IsAttached() const;


	void ApplyImpAmlToPar();


signals:

	void on_off_tune_timer(bool);


};

#endif // OBJ_CMD_H
