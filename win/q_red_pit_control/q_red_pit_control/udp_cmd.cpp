#include "udp_cmd.h"

c_udp_cmd::c_udp_cmd(QObject *parent) : 
		c_base_cmd(parent),
		p_socket(nullptr),
		host_addr(),
		q_host_addr(),
		port(0),
		datagram()
{
	request_period = 50;
}

c_udp_cmd::~c_udp_cmd()
{
}

void c_udp_cmd::set_host(QString addr, int iport)
{
	host_addr=addr;
	q_host_addr.setAddress(host_addr);

	port=iport;
}

void c_udp_cmd::StartConnectrion()
{
	p_socket = new QUdpSocket(this);
	connect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(p_socket, SIGNAL(readyRead()), this, SLOT(udp_ready_dat()));
	p_socket->bind();
}

void c_udp_cmd::StopConnection()
{
	p_socket->close();

	disconnect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	disconnect(p_socket, SIGNAL(readyRead()), this, SLOT(udp_ready_dat()));

	delete p_socket;
}

void c_udp_cmd::process_writing()
{
	p_socket->writeDatagram(reinterpret_cast<char *>(&send_dat), send_dat.GetFullLength(), q_host_addr, port);
}

void c_udp_cmd::socketError(QAbstractSocket::SocketError err)
{
	qDebug()<<"socketError"<< QString("Error: %1").arg(err);
}

void c_udp_cmd::udp_ready_dat()
{
	QHostAddress curr_host_addr;

	while(attached && p_socket->hasPendingDatagrams()) 
	{
		qint64 next_size = p_socket->pendingDatagramSize();
		if(datagram.capacity() < next_size)
			datagram.resize(next_size);

		next_size = p_socket->readDatagram(datagram.data(), next_size, &curr_host_addr);

		if(curr_host_addr == q_host_addr)
			obr_resv(datagram.data(), next_size);
	}
}
