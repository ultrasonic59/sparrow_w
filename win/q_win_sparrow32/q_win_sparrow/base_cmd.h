#ifndef BASE_CMD_H
#define BASE_CMD_H

#include <QObject>
#include <QMutex>
#include "cmdreq.h"


class c_base_cmd : public QObject
{
Q_OBJECT

protected:

	volatile bool attached;
	int request_period;

public:

	QMutex StopMutex;

	sent_dat_t send_dat;
	resv_dat_t resv_dat;

	volatile bool reseive_wait;
	QMutex ReseiveMutex;


	c_base_cmd(QObject *parent);
	virtual ~c_base_cmd();

	bool IsAttached() const;
	void SetAttached(const bool is_attached);

	int GetRequestPeriod() const;

protected:

	virtual void StartConnectrion() = 0;		// ������ ������� ������
	virtual void StopConnection() = 0;			// ���������� ������� ������

	virtual void process_writing() = 0;			// ������ ������ �� send_dat � ������� ������
	void obr_resv(char *in_buff, quint16 len);	// ��������� ������, ���������� �� ������� ������

public slots:

	void slot_write();

	void slot_start();
	void slot_stop();
	
};

#endif // BASE_CMD_H
