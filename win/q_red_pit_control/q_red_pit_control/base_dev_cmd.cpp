#include "base_dev_cmd.h"

extern void addfcs16( char *cp, int len );


c_base_dev_cmd::c_base_dev_cmd(QObject *parent): 
		QObject(parent),
		attached(false),
		curr_cmd(nullptr),


		p_send_dat(nullptr),
		p_char_send_dat(nullptr),
		SendMutex(),

		p_resv_dat(nullptr),

		unsuccesfull_conn(0),
		max_unsuccesfull_conn(10),

		el_timer(),

		time_count(0)
{

}

c_base_dev_cmd::~c_base_dev_cmd()
{
	if(attached)
		DetachFromCMD();
}


void c_base_dev_cmd::AttachToCMD(c_base_cmd *base_cmd)
{
	if(attached)
		return;

	unsuccesfull_conn = 0;


	curr_cmd = base_cmd;

	p_send_dat = &curr_cmd->send_dat;
	p_char_send_dat = reinterpret_cast<char *>(&curr_cmd->send_dat);
	p_resv_dat = &curr_cmd->resv_dat;


	curr_cmd->ReseiveMutex.lock();

	connect(this, SIGNAL(signal_write()), curr_cmd, SLOT(slot_write()), Qt::QueuedConnection);
	connect(this, SIGNAL(signal_start()), curr_cmd, SLOT(slot_start()), Qt::QueuedConnection);
	connect(this, SIGNAL(signal_stop()), curr_cmd, SLOT(slot_stop()), Qt::QueuedConnection);


	attached = true;
	curr_cmd->SetAttached(true);

	emit signal_start();
}


void c_base_dev_cmd::DetachFromCMD()
{
	if(!attached)
		return;

	disconnect(this, SIGNAL(signal_write()), curr_cmd, SLOT(slot_write()));
	disconnect(this, SIGNAL(signal_start()), curr_cmd, SLOT(slot_start()));

	curr_cmd->SetAttached(false);
	attached = false;


	if(!SendMutex.tryLock())	// ещё идёт процесс опроса
	{
		curr_cmd->ReseiveMutex.tryLock();	// разблокировка могла произойти в obr_resv
		curr_cmd->ReseiveMutex.unlock();
		SendMutex.lock();
		curr_cmd->ReseiveMutex.unlock();
		SendMutex.unlock();
	}
	else
	{
		SendMutex.unlock();
		curr_cmd->ReseiveMutex.unlock();
	}


	curr_cmd->StopMutex.lock();
	emit signal_stop();
	curr_cmd->StopMutex.lock();
	disconnect(this, SIGNAL(signal_stop()), curr_cmd, SLOT(slot_stop()));
	curr_cmd->StopMutex.unlock();



	p_resv_dat = nullptr;
	p_char_send_dat = nullptr;
	p_send_dat = nullptr;

	curr_cmd = nullptr;
}

void c_base_dev_cmd::gen_send_dat(const quint8 type, const quint8 cmd, const quint8 *in_dat, quint16 len)	// в type - CMD_PUT_DAT или CMD_REQ_DAT
{
	p_send_dat->type = type;
	p_send_dat->cmd = cmd;
	p_send_dat->len = len;
	if(len > 0)
		memcpy(p_send_dat->buff, in_dat, len);
	len += 2*sizeof(quint8) + sizeof(quint16);
	addfcs16(p_char_send_dat, len);	
}

bool c_base_dev_cmd::send_and_wait(const quint8 type, const quint8 cmd, const quint8 *in_dat, quint16 len, quint8 *out_dat)
{
	if(!attached)
		return false;

	if(!SendMutex.tryLock())		// защита от других команд
		return false;

	bool result = false;
	el_timer.restart();

	gen_send_dat(type, cmd, in_dat, len);	// сгенерировать пакет данных

	emit signal_write();
	curr_cmd->reseive_wait = true;

	if(!attached)
	{
		SendMutex.unlock();
		return false;
	}

	if(curr_cmd->ReseiveMutex.tryLock(TIMEOUT_RSV))	// ответ пришёл вовремя
	{
		if(out_dat != nullptr)
		{
			if(attached)
				memcpy(out_dat, p_resv_dat->buff, p_resv_dat->len);
		}
		unsuccesfull_conn = 0;

		if(attached)
			result = true;
	}
	else
		unsuccesfull_conn++;
	
	if(unsuccesfull_conn > max_unsuccesfull_conn)
		emit NoConnection();

	SendMutex.unlock();

	time_count = el_timer.elapsed();

	return result;
}

bool c_base_dev_cmd::send_and_wait(const quint8 type, const quint8 cmd, quint8 *out_dat)
{
	return send_and_wait(type, cmd, nullptr, 0, out_dat);
}

void c_base_dev_cmd::ResetConnCount()
{
	unsuccesfull_conn = 0;
}

int c_base_dev_cmd::GetUnSuccesfullCount() const
{
	return unsuccesfull_conn;
}

bool c_base_dev_cmd::IsAttached() const
{
	return attached;
}


int c_base_dev_cmd::GetRequestPeriod() const
{
	if(!curr_cmd)
		return TIMEOUT_RSV;

	return curr_cmd->GetRequestPeriod();
}




