#ifndef UDP_CMD_H
#define UDP_CMD_H

#include "base_cmd.h"
#include <QtNetwork/QUdpSocket>


class c_udp_cmd : public c_base_cmd
{
Q_OBJECT

protected:

	QUdpSocket *p_socket;
	QString host_addr;
	QHostAddress q_host_addr;
	int port;


	QByteArray datagram;


public:
	c_udp_cmd(QObject *parent);
	virtual ~c_udp_cmd();


	void set_host(QString addr, int iport);


protected:

	virtual void StartConnectrion();
	virtual void StopConnection();

	virtual void process_writing();


protected slots:
	void socketError(QAbstractSocket::SocketError err);
	void udp_ready_dat();



	
};

#endif // UDP_CMD_H
