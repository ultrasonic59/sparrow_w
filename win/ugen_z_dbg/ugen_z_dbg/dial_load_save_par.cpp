#include "dial_load_save_par.h"

DialLoadSavePar::DialLoadSavePar(QWidget *parent):
    QDialog(parent, Qt::Window),
    ui(new Ui::Dialog_load_save_par)

{
	ui->setupUi(this);
DirName="";
}



void DialLoadSavePar::closeEvent( QCloseEvent * event )
{
QDialog::closeEvent(event);
///CloseMe();
}


DialLoadSavePar::~DialLoadSavePar()
{

}
void  DialLoadSavePar::set_path(QString path)
{
DirName=path;
ui->lineEdit_path->setText(DirName);
}

void  DialLoadSavePar::get_path(QString  *o_path)
{
*o_path=ui->lineEdit_path->text();
///*o_addr=ui->lineEdit_ip->text();
}
void  DialLoadSavePar::set_num_par(int  num_par)
{
QString tstr;
tstr.sprintf("%d",num_par);
ui->lineEdit_num_par->setText(tstr);
}
void  DialLoadSavePar::get_num_par(int  *num_par)
{
*num_par=ui->lineEdit_num_par->text().toInt();
///ui->checkBox_rele->setChecked(*on_rele);
}
void  DialLoadSavePar::set_butt_text(QString text)
{
ui->okButton->setText(text);
///ui->checkBox_rele->setChecked(*on_rele);
}
void DialLoadSavePar::on_butt_dir() 
{ 
////QString DirName;
DirName=QFileDialog::getExistingDirectory(0,tr("\320\240\320\260\320\261\320\276\321\207\320\260\321\217 \320\264\320\270\321\200\320\265\320\272\321\202\320\276\321\200\320\270\321\217"),
                     DirName);
ui->lineEdit_path->setText(DirName);
}
