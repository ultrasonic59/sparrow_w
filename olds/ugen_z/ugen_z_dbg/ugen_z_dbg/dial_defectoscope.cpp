#include "dial_defectoscope.h"

DialDefectoscope::DialDefectoscope(QWidget *parent):
    QDialog(parent, Qt::Window),
    ui(new Ui::Defectoscope)

{
	ui->setupUi(this);
///	ui->comboBox_rej->addItem("XIL",XIL_REJ);
///	ui->comboBox_rej->addItem("STM",STM_REJ);
////	ui->comboBox_rej->addItem("MSP",MSP_REJ);
}



void DialDefectoscope::closeEvent( QCloseEvent * event )
{
QDialog::closeEvent(event);
///CloseMe();
}


DialDefectoscope::~DialDefectoscope()
{

}
#if 0
void  DialDebug::on_butt_rd(void)
{
////quint8 tmpb[32];
QByteArray tmp;
alt_dat_req.addr=ui->lineEdit_addr->text().toInt(0,16);
alt_dat_req.nbytes=ui->lineEdit_count->text().toInt(0,16);
if(ui->comboBox_rej->currentIndex()==XIL_REJ)
	emit req_rd_xil(&alt_dat_req);
else if(ui->comboBox_rej->currentIndex()==STM_REJ) 
	emit req_rd_stm(&alt_dat_req);
else if(ui->comboBox_rej->currentIndex()==MSP_REJ) 
	{
	tmp=ui->lineEdit_addr->text().toLatin1();
////		memcpy(tmpb, ui->lineEdit_addr->text().toLatin1().data(), ui->lineEdit_addr->text().size());//sizeof(quint8) * 32);
////		tmpb[ui->lineEdit_addr->text().size()] = 0;
	emit req_rd_msp(tmp);
	}
}
void  DialDebug::on_butt_wr(void)
{
////quint8 tmpb[32];
QByteArray tmp;
alt_dat_req.addr=ui->lineEdit_addr->text().toInt(0,16);
alt_dat_req.nbytes=1;
alt_dat_req.data[0]=ui->lineEdit_wr_dat->text().toInt(0,16);
if(ui->comboBox_rej->currentIndex()==XIL_REJ)
	emit req_wr_xil(&alt_dat_req);
else if(ui->comboBox_rej->currentIndex()==STM_REJ)
	emit req_wr_stm(&alt_dat_req);
else if(ui->comboBox_rej->currentIndex()==MSP_REJ) 
	{
	tmp=ui->lineEdit_addr->text().toLatin1();
///		memcpy(tmpb, ui->lineEdit_addr->text().toLatin1().data(), ui->lineEdit_addr->text().size());//sizeof(quint8) * 32);
///		tmpb[ui->lineEdit_addr->text().size()] = 0;
	emit req_wr_msp(tmp);
	}
}
void  DialDebug::on_butt_send(void)
{
QByteArray tba="CK"; 
////tba.append( (const char*) snd_buff, num_send);

///alt_dat_req.addr=ui->lineEdit_addr->text().toInt(0,16);
////alt_dat_req.nbytes=1;
////alt_dat_req.data[0]=ui->lineEdit_wr_dat->text().toInt(0,16);

signal_send_udp_test_dats(tba);
/*
if(ui->comboBox_rej->currentIndex()==ALT_REJ)
	emit req_wr_alt(&alt_dat_req);
else
	emit req_wr_act(&alt_dat_req);
*/
///qDebug()<<"on_butt_wr";
}

void  DialDebug::req_data_rdy(alt_dat_req_t* p_alt_dat)
{
QString tstr;
int ii;
if(p_alt_dat->nbytes==0)
p_alt_dat->nbytes=1;
else if(p_alt_dat->nbytes>MAX_ALT_DATA_LEN)
	p_alt_dat->nbytes=MAX_ALT_DATA_LEN;
for(ii=0;ii<p_alt_dat->nbytes;ii++)
{
tstr.sprintf("\naddr=%x->%x",p_alt_dat->addr+ii,p_alt_dat->data[ii]);
ui->textEdit_rd_dat->textCursor().insertText(tstr);
}
////qDebug()<<"req_data_rdy";
}
#endif

#if 0
void  DialPar::set_chans(cn_info_t *icn_info)
{
QString tstr;
memcpy(cn_info,icn_info,sizeof(cn_info_t)*NUM_CHANS);
tstr.sprintf("%d",cn_info[0].nus);
ui->line_ed_us0->setText(tstr);
tstr.sprintf("%d",cn_info[0].ntact);
ui->line_tact0->setText(tstr);

tstr.sprintf("%d",cn_info[1].nus);
ui->line_ed_us1->setText(tstr);
tstr.sprintf("%d",cn_info[1].ntact);
ui->line_tact1->setText(tstr);

tstr.sprintf("%d",cn_info[2].nus);
ui->line_ed_us2->setText(tstr);
tstr.sprintf("%d",cn_info[2].ntact);
ui->line_tact2->setText(tstr);

tstr.sprintf("%d",cn_info[3].nus);
ui->line_ed_us3->setText(tstr);
tstr.sprintf("%d",cn_info[3].ntact);
ui->line_tact3->setText(tstr);

tstr.sprintf("%d",cn_info[4].nus);
ui->line_ed_us4->setText(tstr);
tstr.sprintf("%d",cn_info[4].ntact);
ui->line_tact4->setText(tstr);

tstr.sprintf("%d",cn_info[5].nus);
ui->line_ed_us5->setText(tstr);
tstr.sprintf("%d",cn_info[5].ntact);
ui->line_tact5->setText(tstr);

tstr.sprintf("%d",cn_info[6].nus);
ui->line_ed_us6->setText(tstr);
tstr.sprintf("%d",cn_info[6].ntact);
ui->line_tact6->setText(tstr);

tstr.sprintf("%d",cn_info[7].nus);
ui->line_ed_us7->setText(tstr);
tstr.sprintf("%d",cn_info[7].ntact);
ui->line_tact7->setText(tstr);

}
void  DialPar::get_chans(cn_info_t *ocn_info)
{
QString tstr;
cn_info[0].nus=ui->line_ed_us0->text().toInt();
cn_info[0].ntact=ui->line_tact0->text().toInt();
cn_info[1].nus=ui->line_ed_us1->text().toInt();
cn_info[1].ntact=ui->line_tact1->text().toInt();
cn_info[2].nus=ui->line_ed_us2->text().toInt();
cn_info[2].ntact=ui->line_tact2->text().toInt();
cn_info[3].nus=ui->line_ed_us3->text().toInt();
cn_info[3].ntact=ui->line_tact3->text().toInt();
cn_info[4].nus=ui->line_ed_us4->text().toInt();
cn_info[4].ntact=ui->line_tact4->text().toInt();
cn_info[5].nus=ui->line_ed_us5->text().toInt();
cn_info[5].ntact=ui->line_tact5->text().toInt();
cn_info[6].nus=ui->line_ed_us6->text().toInt();
cn_info[6].ntact=ui->line_tact6->text().toInt();
cn_info[7].nus=ui->line_ed_us7->text().toInt();
cn_info[7].ntact=ui->line_tact7->text().toInt();
memcpy(ocn_info,cn_info,sizeof(cn_info_t)*NUM_CHANS);
}
void  DialPar::set_ip_addr(QString addr)
{
ui->lineEdit_ip->setText(addr);
}

void  DialPar::get_ip_addr(QString  *o_addr)
{
*o_addr=ui->lineEdit_ip->text();
}
void  DialPar::get_rele(bool  *on_rele)
{
*on_rele = ui->checkBox_rele->isChecked();
}
void  DialPar::set_rele(bool  on_rele)
{
ui->checkBox_rele->setChecked(on_rele);
}
void  DialPar::get_on_ed_us(bool  *on_us)
{
*on_us = ui->checkBox_us_ed->isChecked();
}
void  DialPar::set_on_ed_us(bool  on_us)
{
ui->checkBox_us_ed->setChecked(on_us);
}
void  DialPar::get_on_ed_tact(bool  *on_tact)
{
*on_tact = ui->checkBox_ed_tact->isChecked();
}
void  DialPar::set_on_ed_tact(bool  on_tact)
{
ui->checkBox_ed_tact->setChecked(on_tact);
}
#endif

#if 0
void  DialPar::on_send_butt()
{
////qDebug()<<"on_send_butt";
qDebug() << "on_send_butt";

}
#endif

