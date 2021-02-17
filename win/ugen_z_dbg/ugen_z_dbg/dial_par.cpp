#include "dial_par.h"

DialPar::DialPar(QWidget *parent):
    QDialog(parent, Qt::Window),
    ui(new Ui::Dialog_par)

{
	ui->setupUi(this);
}



void DialPar::closeEvent( QCloseEvent * event )
{
QDialog::closeEvent(event);
///CloseMe();
}


DialPar::~DialPar()
{

}
/*
void  DialPar::set_chans(cn_info_t *icn_info)
{
QString tstr;
memcpy(cn_info,icn_info,sizeof(cn_info_t)*NUM_CHANS);

for(quint8 i=0;i<NUM_CHANS;i++)
{
	cn_info[i].nus++;
	cn_info[i].ntact++;
}


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
*/
/*
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


for(quint8 i=0;i<NUM_CHANS;i++)
{
	cn_info[i].nus--;
	cn_info[i].ntact--;
}

memcpy(ocn_info,cn_info,sizeof(cn_info_t)*NUM_CHANS);
} 
*/
void  DialPar::set_ip_addr(QString addr)
{
ui->lineEdit_ip->setText(addr);
}

void  DialPar::get_ip_addr(QString  *o_addr)
{
*o_addr=ui->lineEdit_ip->text();
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


void DialPar::EnableParams(bool set)
{
ui->line_ed_us0->setEnabled(set);
ui->line_tact0->setEnabled(set);
ui->line_ed_us1->setEnabled(set);
ui->line_tact1->setEnabled(set);

ui->checkBox_us_ed->setEnabled(set);
ui->checkBox_ed_tact->setEnabled(set);

}

#if 0
void  DialPar::on_send_butt()
{
////qDebug()<<"on_send_butt";
qDebug() << "on_send_butt";

}
#endif

