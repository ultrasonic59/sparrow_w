#ifndef BASE_DEV_CMD_H
#define BASE_DEV_CMD_H

#include "base_cmd.h"
#include <QElapsedTimer>


#define TIMEOUT_RSV		100


class c_base_dev_cmd : public QObject
{
Q_OBJECT

protected:

	volatile bool attached;

	c_base_cmd *curr_cmd;


	sent_dat_t *p_send_dat;
	char *p_char_send_dat;
	QMutex SendMutex;


	resv_dat_t *p_resv_dat;


	int unsuccesfull_conn;	
	int max_unsuccesfull_conn;

	QElapsedTimer el_timer;


	inline void gen_send_dat(const quint8 type, const quint8 cmd, const quint8 *in_dat, quint16 len);	// генерирование отправляемых данных
	bool send_and_wait(const quint8 type, const quint8 cmd, const quint8 *in_dat, quint16 len, quint8 *out_dat = nullptr);
	bool send_and_wait(const quint8 type, const quint8 cmd, quint8 *out_dat);


public:
	c_base_dev_cmd(QObject *parent);
	virtual ~c_base_dev_cmd();


	void AttachToCMD(c_base_cmd *base_cmd);
	void DetachFromCMD();

	void ResetConnCount();
	int GetUnSuccesfullCount() const;

	bool IsAttached() const;

	int time_count;


	int GetRequestPeriod() const;


signals:

	void signal_write();
	void NoConnection();


	void signal_start();
	void signal_stop();

	
};

#endif // BASE_DEV_CMD_H
