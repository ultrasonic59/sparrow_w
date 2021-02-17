#ifndef CMDREQ_H
#define CMDREQ_H



#define MAX_FRAME_LEN		6000



struct resv_dat_t
{
	quint8 type;
	quint8 cmd;
	quint16 len;
	quint8 buff[MAX_FRAME_LEN];
};

struct sent_dat_t
{
	quint8 type;
	quint8 cmd;
	quint16 len;
	quint8 buff[MAX_FRAME_LEN - 2*sizeof(quint8) - sizeof(quint16)];


	quint16 GetFullLength() const
	{
		return 2*sizeof(quint8) + sizeof(quint16) + len + sizeof(quint16);
	}
};






#endif // CMDREQ_H
