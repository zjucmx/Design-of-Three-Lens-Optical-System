#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opt_main.h"
#include "svpng.h"
#include "fonts_table.h"
extern "C"{
    #include "part1.h"
    #include "part2.h"
    #include "part3.h"
    #include "file_read.h"
    #include "aberration_img.h"
}
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QMessageBox>
#include <QTextCharFormat>
struct FACE* head;
struct FACE* temp_face;
int read_check = 0;
int read_file = 0;
int field_img = 0;
unsigned char Fonts[5000] = {
       0x00, 0x00, 0x00, 0x00, 0x00,// ' '
       0x00, 0x00, 0x5F, 0x00, 0x00,// !
       0x00, 0x07, 0x00, 0x07, 0x00,// "
       0x14, 0x7F, 0x14, 0x7F, 0x14,// #
       0x24, 0x2A, 0x07, 0x2A, 0x12,// $
       0x23, 0x13, 0x08, 0x64, 0x62,// %
       0x37, 0x49, 0x55, 0x22, 0x50,// &
       0x00, 0x05, 0x03, 0x00, 0x00,// '
       0x00, 0x1C, 0x22, 0x41, 0x00,// (
       0x00, 0x41, 0x22, 0x1C, 0x00,// )
       0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
       0x08, 0x08, 0x3E, 0x08, 0x08,// +
       0x00, 0x50, 0x30, 0x00, 0x00,// ,
       0x08, 0x08, 0x08, 0x08, 0x08,// -
       0x00, 0x60, 0x60, 0x00, 0x00,// .
       0x20, 0x10, 0x08, 0x04, 0x02,// /
       0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
       0x00, 0x42, 0x7F, 0x40, 0x00,// 1
       0x42, 0x61, 0x51, 0x49, 0x46,// 2
       0x21, 0x41, 0x45, 0x4B, 0x31,// 3
       0x18, 0x14, 0x12, 0x7F, 0x10,// 4
       0x27, 0x45, 0x45, 0x45, 0x39,// 5
       0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
       0x01, 0x71, 0x09, 0x05, 0x03,// 7
       0x36, 0x49, 0x49, 0x49, 0x36,// 8
       0x06, 0x49, 0x49, 0x29, 0x1E,// 9
       0x00, 0x36, 0x36, 0x00, 0x00,// :
       0x00, 0x56, 0x36, 0x00, 0x00,// ;
       0x00, 0x08, 0x14, 0x22, 0x41,// <
       0x14, 0x14, 0x14, 0x14, 0x14,// =
       0x41, 0x22, 0x14, 0x08, 0x00,// >
       0x02, 0x01, 0x51, 0x09, 0x06,// ?
       0x32, 0x49, 0x79, 0x41, 0x3E,// @
       0x7E, 0x11, 0x11, 0x11, 0x7E,// A
       0x7F, 0x49, 0x49, 0x49, 0x36,// B
       0x3E, 0x41, 0x41, 0x41, 0x22,// C
       0x7F, 0x41, 0x41, 0x22, 0x1C,// D
       0x7F, 0x49, 0x49, 0x49, 0x41,// E
       0x7F, 0x09, 0x09, 0x01, 0x01,// F
       0x3E, 0x41, 0x41, 0x51, 0x32,// G
       0x7F, 0x08, 0x08, 0x08, 0x7F,// H
       0x00, 0x41, 0x7F, 0x41, 0x00,// I
       0x20, 0x40, 0x41, 0x3F, 0x01,// J
       0x7F, 0x08, 0x14, 0x22, 0x41,// K
       0x7F, 0x40, 0x40, 0x40, 0x40,// L
       0x7F, 0x02, 0x04, 0x02, 0x7F,// M
       0x7F, 0x04, 0x08, 0x10, 0x7F,// N
       0x3E, 0x41, 0x41, 0x41, 0x3E,// O
       0x7F, 0x09, 0x09, 0x09, 0x06,// P
       0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
       0x7F, 0x09, 0x19, 0x29, 0x46,// R
       0x46, 0x49, 0x49, 0x49, 0x31,// S
       0x01, 0x01, 0x7F, 0x01, 0x01,// T
       0x3F, 0x40, 0x40, 0x40, 0x3F,// U
       0x1F, 0x20, 0x40, 0x20, 0x1F,// V
       0x7F, 0x20, 0x18, 0x20, 0x7F,// W
       0x63, 0x14, 0x08, 0x14, 0x63,// X
       0x03, 0x04, 0x78, 0x04, 0x03,// Y
       0x61, 0x51, 0x49, 0x45, 0x43,// Z
       0x00, 0x00, 0x7F, 0x41, 0x41,// [
       0x02, 0x04, 0x08, 0x10, 0x20,// "\"
       0x41, 0x41, 0x7F, 0x00, 0x00,// ]
       0x04, 0x02, 0x01, 0x02, 0x04,// ^
       0x40, 0x40, 0x40, 0x40, 0x40,// _
       0x00, 0x01, 0x02, 0x04, 0x00,// `
       0x20, 0x54, 0x54, 0x54, 0x78,// a
       0x7F, 0x48, 0x44, 0x44, 0x38,// b
       0x38, 0x44, 0x44, 0x44, 0x20,// c
       0x38, 0x44, 0x44, 0x48, 0x7F,// d
       0x38, 0x54, 0x54, 0x54, 0x18,// e
       0x08, 0x7E, 0x09, 0x01, 0x02,// f
       0x08, 0x14, 0x54, 0x54, 0x3C,// g
       0x7F, 0x08, 0x04, 0x04, 0x78,// h
       0x00, 0x44, 0x7D, 0x40, 0x00,// i
       0x20, 0x40, 0x44, 0x3D, 0x00,// j
       0x00, 0x7F, 0x10, 0x28, 0x44,// k
       0x00, 0x41, 0x7F, 0x40, 0x00,// l
       0x7C, 0x04, 0x18, 0x04, 0x78,// m
       0x7C, 0x08, 0x04, 0x04, 0x78,// n
       0x38, 0x44, 0x44, 0x44, 0x38,// o
       0x7C, 0x14, 0x14, 0x14, 0x08,// p
       0x08, 0x14, 0x14, 0x18, 0x7C,// q
       0x7C, 0x08, 0x04, 0x04, 0x08,// r
       0x48, 0x54, 0x54, 0x54, 0x20,// s
       0x04, 0x3F, 0x44, 0x40, 0x20,// t
       0x3C, 0x40, 0x40, 0x20, 0x7C,// u
       0x1C, 0x20, 0x40, 0x20, 0x1C,// v
       0x3C, 0x40, 0x30, 0x40, 0x3C,// w
       0x44, 0x28, 0x10, 0x28, 0x44,// x
       0x0C, 0x50, 0x50, 0x50, 0x3C,// y
       0x44, 0x64, 0x54, 0x4C, 0x44,// z
       0x00, 0x08, 0x36, 0x41, 0x00,// {
       0x00, 0x00, 0x7F, 0x00, 0x00,// |
       0x00, 0x41, 0x36, 0x08, 0x00,// }
       0x02, 0x01, 0x02, 0x04, 0x02,// ~
       0xff, 0xff, 0xff, 0xff, 0xff,//black block
};
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     //????????????
     connect(ui->pushButton_exit,&QPushButton::clicked,[=](){
        QString fileName = QFileDialog::getSaveFileName(this,tr("????????????"), "",tr("opt_design_result(*.txt)"));
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
           {
               QMessageBox::critical(this, "critical", tr("?????????????????????"),
                                     QMessageBox::Yes, QMessageBox::Yes);
           }
        else{
            QTextStream stream(&file);
            stream<<QString("??????????????????")<<endl;
            stream<<QString(" ")<<endl;

            stream<<QString("??????????????????")<<endl;
            stream<<QString("?????????????????????");
            if(ui->radioButton->isChecked())
                stream<<QString("?????????")<<endl;
            else
                stream<<ui->lineEdit_3->text()<<endl;
            if(ui->radioButton->isChecked())
                stream<<QString("???????????????")<<ui->lineEdit_4->text()<<endl;
            else
                stream<<QString("?????????")<<ui->lineEdit_4->text()<<endl;
            stream<<QString(" ")<<endl;

            stream<<QString("??????????????????")<<endl;
            stream<<QString("???????????????????????????")<<ui->lineEdit->text()<<endl;
            QFile file(".\\opt_system_cal.txt");
            if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {

                qDebug()<<"Can't open the file!"<<endl;

            }
            file.open(QIODevice::ReadOnly|QIODevice::Text);
            QTextStream out(&file);
            for(int i=0;i<4;i++){
                stream<<QString("?????????")<<out.readLine()<<endl;
                stream<<QString("???????????????")<<out.readLine()<<endl;
                stream<<QString("d???????????????")<<out.readLine()<<endl;
                stream<<QString("c???????????????")<<out.readLine()<<endl;
                stream<<QString("f???????????????")<<out.readLine()<<endl;
                stream<<QString("????????????????????????")<<out.readLine()<<endl;
                stream<<QString("???????????????")<<out.readLine()<<endl;
            }
            stream<<QString(" ")<<endl;

            stream<<ui->textEdit_2->toPlainText()<<endl;
            file.close();
        }
    });

     //???????????????????????????
     connect(ui->pushButton_3,&QPushButton::clicked,[=](){
         QString filePath = QFileDialog::getOpenFileName(this,"????????????",".\\opt_system_cal.txt","(*.txt)");
         //qDebug() << filePath;
         //?????????????????????lineEdit???
         read_file = 1;
         if(filePath.isEmpty()){
             QMessageBox::warning(this,"????????????","??????????????????");
             return;
         }

         ui->lineEdit->setText(filePath);

         //???????????????????????????textEdit???
         QFile file(".\\opt_system_cal.txt");;  //?????????????????? utf-8
         //??????????????????
         file.open(QIODevice::ReadOnly); //????????????????????????
         //QByteArray arr;
         //arr = file.readAll();
         //while(!file.atEnd()){
             //arr += file.readLine();
         //}
         //????????????
         //ui->textEdit->setText(arr);
         ui->textEdit->append("????????????????????????");
         ui->textEdit->append(" ");
         for(int i=0;i<4;i++){
             ui->textEdit->append("?????????");
             ui->textEdit->moveCursor(QTextCursor::End);
             ui->textEdit->insertPlainText(file.readLine());
             ui->textEdit->append("???????????????");
             ui->textEdit->moveCursor(QTextCursor::End);
             ui->textEdit->insertPlainText(file.readLine());
             ui->textEdit->append("d???????????????");
             ui->textEdit->moveCursor(QTextCursor::End);
             ui->textEdit->insertPlainText(file.readLine());
             ui->textEdit->append("c???????????????");
             ui->textEdit->moveCursor(QTextCursor::End);
             ui->textEdit->insertPlainText(file.readLine());
             ui->textEdit->append("f???????????????");
             ui->textEdit->moveCursor(QTextCursor::End);
             ui->textEdit->insertPlainText(file.readLine());
             ui->textEdit->append("????????????????????????");
             ui->textEdit->moveCursor(QTextCursor::End);
             ui->textEdit->insertPlainText(file.readLine());
             ui->textEdit->append("???????????????");
             ui->textEdit->moveCursor(QTextCursor::End);
             ui->textEdit->insertPlainText(file.readLine());
         }
     });

     //?????????????????????
      connect(ui->pushButton_5,&QPushButton::clicked,[=](){
          if(!field_img){
              QMessageBox::warning(this,"??????????????????","???????????????????????????");
              return;
          }
         ui->label_8->raise();//??????????????????
         QString file(".\\photo\\sperical.png");
         if(!file.isEmpty())
         {
              QImage* img=new QImage;
              if(! ( img->load(file) ) ) //????????????
              {
                  QMessageBox::information(this,tr("??????????????????"),tr("??????????????????!"));
                  delete img;
              }
          ui->label_8->setPixmap(QPixmap::fromImage(*img));

     }});

    //??????????????????
     connect(ui->pushButton_6,&QPushButton::clicked,[=](){
         if(!field_img){
             QMessageBox::warning(this,"??????????????????","???????????????????????????");
             return;
         }
         ui->label_8->raise();//??????????????????
         QString file(".\\photo\\distortion.png");
         if(!file.isEmpty())
         {
              QImage* img=new QImage;
              if(! ( img->load(file) ) ) //????????????
              {
                  QMessageBox::information(this,tr("??????????????????"),tr("??????????????????!"));
                  delete img;
              }
          ui->label_8->setPixmap(QPixmap::fromImage(*img));
    }});

     //?????????????????????
      connect(ui->pushButton_8,&QPushButton::clicked,[=](){
          if(!field_img){
              QMessageBox::warning(this,"??????????????????","???????????????????????????");
              return;
          }
          ui->label_8->raise();//??????????????????
          QString file(".\\photo\\focla_shift.png");
          if(!file.isEmpty())
          {
               QImage* img=new QImage;
               if(! ( img->load(file) ) ) //????????????
               {
                   QMessageBox::information(this,tr("??????????????????"),tr("??????????????????!"));
                   delete img;
               }
           ui->label_8->setPixmap(QPixmap::fromImage(*img));
     }});

    //????????????????????????
          connect(ui->pushButton_7,&QPushButton::clicked,[=](){
              if(!field_img){
                  QMessageBox::warning(this,"??????????????????","???????????????????????????");
                  return;
              }
              ui->label_8->raise();//??????????????????
              QString file(".\\photo\\field_curve.png");
              if(!file.isEmpty())
              {
                   QImage* img=new QImage;
                   if(! ( img->load(file) ) ) //????????????
                   {
                       QMessageBox::information(this,tr("??????????????????"),tr("??????????????????!"));
                       delete img;
                   }
               ui->label_8->setPixmap(QPixmap::fromImage(*img));
    }});

    //????????????
     connect(ui->pushButton_2,&QPushButton::clicked,[=](){
         struct GAUSS object;
         struct PUPLE puple;

         temp_face = new(struct FACE);
         head = new (struct FACE);
         QFile file(".\\opt_system_cal.txt");
         if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {

             qDebug()<<"Can't open the file!"<<endl;

         }
         file.open(QIODevice::ReadOnly|QIODevice::Text);
         QTextStream out(&file);

         head->index = out.readLine().toInt();
         head->r = out.readLine().toDouble();
         head->n[0] = out.readLine().toDouble();
         head->n[1] = out.readLine().toDouble();
         head->n[2] = out.readLine().toDouble();
         head->d = out.readLine().toDouble();
         head->Vd = out.readLine().toDouble();
         head->next = NULL;


         temp_face = head;
         for(int i=0;i<3;i++){
             temp_face->next = new (struct FACE);
             temp_face->next->index = out.readLine().toInt();
             temp_face->next->r = out.readLine().toDouble();
             temp_face->next->n[0] = out.readLine().toDouble();
             temp_face->next->n[1] = out.readLine().toDouble();
             temp_face->next->n[2] = out.readLine().toDouble();
             temp_face->next->d = out.readLine().toDouble();
             temp_face->next->Vd = out.readLine().toDouble();
             temp_face = temp_face->next;
         }
         temp_face->next = NULL;

         puple.index = head->index;
         puple.a = head->r;
         puple.l = head->d;

         if(ui->radioButton->isChecked()){
             object.l = INFINITY;
         }
         else
             object.l = ui->lineEdit_3->text().toDouble();   //????????????????????????

         //double y = ui->lineEdit_4->text().toDouble(); //?????????????????????????????????
         //object.y = y;
         if(ui->radioButton->isChecked()){
             if((ui->lineEdit_4->text().toDouble()>90)||(ui->lineEdit_4->text().toDouble()<-90)){
                 QMessageBox::warning(this,"??????????????????","??????????????????????????????????????????");
                 return;
             }
             object.y = (ui->lineEdit_4->text().toDouble())/180*3.1415926;
         }
         else
             object.y = ui->lineEdit_4->text().toDouble();

         PlotAberration(object,head,puple);
         field_img = 1;
     });

     //????????????
     connect(ui->pushButton_4,&QPushButton::clicked,[=](){
         if(!read_file){
             QMessageBox::warning(this,"???????????????","??????????????????'opt_system_cal.txt'");
             return;
         }
         temp_face = new(struct FACE);
         head = new (struct FACE);
         QFile file(".\\opt_system_cal.txt");
         if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {

             qDebug()<<"Can't open the file!"<<endl;

         }
         file.open(QIODevice::ReadOnly|QIODevice::Text);
         QTextStream out(&file);

         head->index = out.readLine().toInt();
         head->r = out.readLine().toDouble();
         head->n[0] = out.readLine().toDouble();
         head->n[1] = out.readLine().toDouble();
         head->n[2] = out.readLine().toDouble();
         head->d = out.readLine().toDouble();
         head->Vd = out.readLine().toDouble();
         head->next = NULL;


         temp_face = head;
         for(int i=0;i<3;i++){
             temp_face->next = new (struct FACE);
             temp_face->next->index = out.readLine().toInt();
             temp_face->next->r = out.readLine().toDouble();
             temp_face->next->n[0] = out.readLine().toDouble();
             temp_face->next->n[1] = out.readLine().toDouble();
             temp_face->next->n[2] = out.readLine().toDouble();
             temp_face->next->d = out.readLine().toDouble();
             temp_face->next->Vd = out.readLine().toDouble();
             temp_face = temp_face->next;
         }
         temp_face->next = NULL;
         read_check = 1;
     });  
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    ui->textEdit_2->clear();
    if(!read_check){
        QMessageBox::warning(this,"????????????","?????????????????????????????????????????????");
        return;
    }
    if((!ui->lineEdit_3->text().isEmpty())&&(ui->radioButton->isChecked())){
        QMessageBox::warning(this,"??????????????????","??????????????????????????????????????????");
        return;
    }
    struct FACE* head_F;
    struct FACE* head_C;
    struct PUPLE puple;
    struct GAUSS object;
    struct GAUSS ideal_image[4][3];
    struct GAUSS real_image[4][3];
    struct GAUSS temp_image;
    double L;
    double U;
    double L_;
    double U_;
    double Kr;
    double Kw;
    int reflection_mode;
    int i,j;
    double f_, lf_, lH_, lp_, Dp_;
    double l_, y_, y07_;
    double ya_, yb_, yp_;
    double xs, xt, dx;
    puple.a = head->r;
    puple.l = head->d;
    //double y = ui->lineEdit_4->text().toDouble(); //?????????????????????????????????
    //object.y = y;
    if(ui->radioButton->isChecked()){
        if((ui->lineEdit_4->text().toDouble()>90)||(ui->lineEdit_4->text().toDouble()<-90)){
            QMessageBox::warning(this,"??????????????????","??????????????????????????????????????????");
            return;
        }
        object.y = (ui->lineEdit_4->text().toDouble())/180*3.1415926;
    }
    else
        object.y = ui->lineEdit_4->text().toDouble();

    if(ui->radioButton->isChecked()){
        object.l = INFINITY;
    }
    else
        object.l = ui->lineEdit_3->text().toDouble();   //????????????????????????

    for(i=0;i<4;i++){
        switch(i){
            case 0: Kr = 1; Kw = 1; break;      // ?????????????????????
            case 1: Kr = 0.7; Kw = 1; break;  // 0.7??????????????????
            case 2: Kr = 0; Kw = 1; break;      // ?????????????????????
            case 3: Kr = 0, Kw = 0.7; break;  // ????????????0.7??????
        }
        for ( j=0; j<3; j++)
               {
                   // ??????????????????????????????????????????????????????????????????y???0??????????????????(case 2???case 3????????????????????????)
                   // ??????ideal_image??????????????????????????????????????????????????????ideal_image[i][j].y
                   // ??????????????????case 2???case 3????????????????????????????????????????????????l_, u_, ?????????????????????????????????

                   // ???????????????????????????????????????????????????????????????????????????????????????????????????????????????
                   if( i==0 || i==1 )
                       // ?????????????????????????????????Kr?????????
                       Initialize( object.l, 0, Kr, Kw, puple, &L, &U);
                   else
                       // ?????????????????????????????????Kw?????????
                       Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
                   ideal_image[i][j] = GaussianImage(head->n[0], L, U, object.y, head->next, j);
                   Iteration(head->n[0], L, U, &(real_image[i][j].l), &(real_image[i][j].u),
                             head->next, j);
                   // i=0,1???real_image.y????????????????????????i=2,3???real_image.y??????????????????
                   // ??????????????????ideal_image[0][j].l???????????????
                   if(real_image[i][j].u != 0)
                       real_image[i][j].y = (real_image[i][j].l - ideal_image[0][0].l) * tan(real_image[i][j].u);
                   else
                       real_image[i][j].y = 0;
               }
    }

    // ???????????????f_, lf_, lH_, lp_,
            // ???????????????????????????????????????????????????????????? lf_
            // ?????????????????????Kr = 1, Kw = 1
            // ??????????????????object_l = INFINITY, W = 0.0
            Initialize( INFINITY, 0.0, 1, 1, puple, &L, &U);
            temp_image = GaussianImage( head->n[0], L, U, 0, head->next, 0);
            lf_ = temp_image.l;
            f_ = head->r / temp_image.u;
            lH_ = lf_ - f_;
            ui->textEdit_2->append("??????????????????");
            ui->textEdit_2->append("f': ");
            ui->textEdit_2->moveCursor(QTextCursor::End);
            ui->textEdit_2->insertPlainText(QString::number(f_));
            ui->textEdit_2->append("lf': ");
            ui->textEdit_2->moveCursor(QTextCursor::End);
            ui->textEdit_2->insertPlainText(QString::number(lf_));
            ui->textEdit_2->append("lH': ");
            ui->textEdit_2->moveCursor(QTextCursor::End);
            ui->textEdit_2->insertPlainText(QString::number(lH_));

    // ?????????????????????????????????????????????????????????????????????
            // ????????????????????? Kr = 0, Kw = 1
            // ???????????????object_l = puple.l, ?????????????????????????????????????????????????????????????????? y_
            Initialize( puple.l, 0, 0, 1, puple, &L, &U);
            temp_image = GaussianImage( head->n[0], L, U, puple.a, head->next, 0);
            lp_ = temp_image.l;
            Dp_ = puple.a * 2;
            ui->textEdit_2->append("lp': ");
            ui->textEdit_2->moveCursor(QTextCursor::End);
            ui->textEdit_2->insertPlainText(QString::number(lp_));
            ui->textEdit_2->append("Dp': ");
            ui->textEdit_2->moveCursor(QTextCursor::End);
            ui->textEdit_2->insertPlainText(QString::number(Dp_));

    // ???????????????????????????y_, y07_,
            ui->textEdit_2->append(" ");
            ui->textEdit_2->append("????????????????????????");
            l_ = ideal_image[0][0].l;
            if( fabs(object.l) == INFINITY )
            {   // ?????????????????????????????????????????????y_
                // y_ = tan( ideal_image[2][0].u ) * (ideal_image[2][0].l - l_);
                // y07_ = tan( ideal_image[3][0].u ) * (lp_ - l_);
                y_ = tan( object.y ) * (lH_ - l_);
                y07_ = tan( object.y * 0.7) * (lH_ - l_);
            }
            else
            {
                y_ = ideal_image[0][0].y;
                y07_ = 0.7 * y_;
            }
            ui->textEdit_2->append("y': ");
            ui->textEdit_2->moveCursor(QTextCursor::End);
            ui->textEdit_2->insertPlainText(QString::number(y_));
            ui->textEdit_2->append("l': ");
            ui->textEdit_2->moveCursor(QTextCursor::End);
            ui->textEdit_2->insertPlainText(QString::number(l_));
            //0.7?????????????????????
            ui->textEdit_2->append("y0.7': ");
            ui->textEdit_2->moveCursor(QTextCursor::End);
            ui->textEdit_2->insertPlainText(QString::number(y07_));
            ui->textEdit_2->append("l0.7': ");
            ui->textEdit_2->moveCursor(QTextCursor::End);
            ui->textEdit_2->insertPlainText(QString::number(l_));

    // ???????????????xt_, xs_, dx_,
           Kr = 0; Kw = 1;
           Initialize(object.l, object.y, Kr, Kw, puple, &L, &U);
           cal_part3(U, object.l, L, l_, &xs, &xt, &dx, head->next, 0);
           if(object.y==0){
               ui->textEdit_2->append(" ");
               ui->textEdit_2->append("???????????????");
               ui->textEdit_2->append("xt: ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               xt = 0;
               ui->textEdit_2->insertPlainText(QString::number(xt));
               ui->textEdit_2->append("xs: ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               xs = 0;
               ui->textEdit_2->insertPlainText(QString::number(xs));
               ui->textEdit_2->append("dx: ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               dx = 0;
               ui->textEdit_2->insertPlainText(QString::number(dx));
           }
           else{
                ui->textEdit_2->append(" ");
                ui->textEdit_2->append("???????????????");
                ui->textEdit_2->append("xt: ");
                ui->textEdit_2->moveCursor(QTextCursor::End);
                ui->textEdit_2->insertPlainText(QString::number(xt));
                ui->textEdit_2->append("xs: ");
                ui->textEdit_2->moveCursor(QTextCursor::End);
                ui->textEdit_2->insertPlainText(QString::number(xs));
                ui->textEdit_2->append("dx: ");
                ui->textEdit_2->moveCursor(QTextCursor::End);
                ui->textEdit_2->insertPlainText(QString::number(dx));
           }

    // F???C?????????????????????lC_,lF_,
           ui->textEdit_2->append(" ");
           ui->textEdit_2->append("F/C??????????????????");
           ui->textEdit_2->append("lC': ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(ideal_image[0][1].l));
           ui->textEdit_2->append("lF': ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(ideal_image[0][2].l));

    // ????????????0.7??????d/C/F????????????????????????L_, LC_, LF_, L07_, L07C_, L07F_,
           ui->textEdit_2->append(" ");
           ui->textEdit_2->append("????????????0.7??????d/C/F?????????????????????");
           ui->textEdit_2->append("L': ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[0][0].l));
           ui->textEdit_2->append("LC': ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[0][1].l));
           ui->textEdit_2->append("LF': ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[0][2].l));
           ui->textEdit_2->append("L0.7': ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[1][0].l));
           ui->textEdit_2->append("LC0.7': ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[1][1].l));
           ui->textEdit_2->append("LF0.7': ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[1][2].l));

    // ????????????0.7??????????????? L_-l_, L07_-l_
           ui->textEdit_2->append(" ");
           ui->textEdit_2->append("????????????0.7????????????");
           ui->textEdit_2->append("dl: ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[0][0].l-l_));
           ui->textEdit_2->append("dl0.7: ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[1][0].l-l_));

    // ????????????0.7?????????0?????????????????????LF_-LC_, L07F-L07C, lF_-lC_
           ui->textEdit_2->append(" ");
           ui->textEdit_2->append("????????????0.7???0??????????????????");
           ui->textEdit_2->append("dLch: ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[0][2].l - real_image[0][1].l));
           ui->textEdit_2->append("dLch0.7: ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(real_image[1][2].l - real_image[1][1].l));
           ui->textEdit_2->append("dlch: ");
           ui->textEdit_2->moveCursor(QTextCursor::End);
           ui->textEdit_2->insertPlainText(QString::number(ideal_image[0][2].l - ideal_image[0][1].l));

    // ????????????0.7????????????????????????????????????0.5(ya_+yb_)-yp_, 0.5(ya07_+yb07_)-yp07_
           ui->textEdit_2->append(" ");
           ui->textEdit_2->append("????????????0.7???????????????????????????");
           if (y_ == 0){
               ui->textEdit_2->append("??????????????????");
           }
           else {
               // ????????????????????????????????????????????? ya_, yb_, yp_
               // ????????????????????????????????? Kr = +1, Kw = 1;
               Kr = 1; Kw = 1;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               ya_ = temp_image.y;
               // ????????????????????????????????? Kr = -1, Kw = 1;
               Kr = -1; Kw = 1;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               yb_ = temp_image.y;
               // ????????????????????? Kr = 0, Kw = 1;
               Kr = 0; Kw = 1;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               yp_ = temp_image.y;
               if( y_ == 0)
                   yp_ = 0;
               ui->textEdit_2->append("Kt: ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(0.5*(ya_ + yb_) - yp_));

               // 0.7?????????????????????????????? Kr = +1, Kw = 0.707;
               Kr = 1; Kw = 0.7;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               ya_ = temp_image.y;
               // 0.7?????????????????????????????? Kr = -1, Kw = 0.707;
               Kr = -1; Kw = 0.7;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               yb_ = temp_image.y;
               // 0.7?????????????????? Kr = 0, Kw = 0.707;
               Kr = 0; Kw = 0.7;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               yp_ = temp_image.y;
               if( y_ == 0)
                   yp_ = 0;
               ui->textEdit_2->append("Kt0.7: ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(0.5*(ya_ + yb_) - yp_));
           }

     // ????????????0.7?????????0.7????????????????????????0.5(ya_+yb_)-yp_, 0.5(ya07_+yb07_)-yp07_
           ui->textEdit_2->append(" ");
           ui->textEdit_2->append("????????????0.7?????????0.7???????????????");
           if (y_ == 0)
               ui->textEdit_2->append("??????????????????");
           else
           {
           // ????????????????????????????????????????????? ya_, yb_, yp_
           // ????????????0.7?????????????????? Kr = +0.7, Kw = 1;
               Kr = 0.7; Kw = 1;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               ya_ = temp_image.y;
           // ????????????0.7?????????????????? Kr = -0.7, Kw = 1;
               Kr = -0.7; Kw = 1;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               yb_ = temp_image.y;
           // ????????????????????? Kr = 0, Kw = 1;
               Kr = 0; Kw = 1;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               yp_ = temp_image.y;
               if( y_ == 0)
                   yp_ = 0;
               ui->textEdit_2->append("Kt: ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(0.5*(ya_ + yb_) - yp_));

           // 0.7?????????0.7?????????????????? Kr = +0.7, Kw = 0.7;
               Kr = 0.7; Kw = 0.7;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               ya_ = temp_image.y;
           // 0.7?????????0.7?????????????????? Kr = -0.7, Kw = 0.7;
               Kr = -0.7; Kw = 0.7;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               yb_ = temp_image.y;
           // 0.7?????????????????? Kr = 0, Kw = 0.7;
               Kr = 0; Kw = 0.7;
               Initialize( object.l, object.y, Kr, Kw, puple, &L, &U);
               Iteration( head->n[0], L, U, &(temp_image.l), &(temp_image.u), head->next, 0);
               temp_image.y = (temp_image.l - l_) * tan(temp_image.u);
               yp_ = temp_image.y;
               if( y_ == 0) yp_ = 0;
               ui->textEdit_2->append("Kt0.7: ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(0.5*(ya_ + yb_) - yp_));
           }

          // ????????????0.7??????d/C/F??????????????????Y_, Y07_, YC_, Y07C_, YF_, Y07F_
               ui->textEdit_2->append(" ");
               ui->textEdit_2->append("????????????0.7??????d/F/C???????????????");
               //printf("Y_ = %.10lf\t", real_image[2][0].y);
               ui->textEdit_2->append("Y': ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(real_image[2][0].y));

               //printf("Y07_ = %.10lf\t", real_image[3][0].y);
               ui->textEdit_2->append("Y0.7': ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(real_image[3][0].y));

               //printf("YC_ = %.10lf\t", real_image[2][1].y);
               ui->textEdit_2->append("YC': ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(real_image[2][1].y));

               //printf("Y07C_ = %.10lf\t", real_image[3][1].y);
               ui->textEdit_2->append("Y0.7C': ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(real_image[3][1].y));

               //printf("YF_ = %.10lf\t", real_image[2][2].y);
               ui->textEdit_2->append("YF': ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(real_image[2][2].y));
               //printf("Y07F_ = %.10lf\t", real_image[3][2].y);
               ui->textEdit_2->append("Y0.7F': ");
               ui->textEdit_2->moveCursor(QTextCursor::End);
               ui->textEdit_2->insertPlainText(QString::number(real_image[3][2].y));

        // ????????????0.7??????????????????????????????Y_-y_, Y07-y07_, (Y_-y_)/y_, (Y07_-y07_)/y07
               //printf("\n????????????0.7?????????????????????????????????\n");
               ui->textEdit_2->append(" ");
               ui->textEdit_2->append("????????????0.7?????????????????????????????????");
               if( y_ == 0 )
                   //printf("??????????????????\n");
                   ui->textEdit_2->append("??????????????????");
               else
               {
                   //printf("Y_-y_ = %.10lf\t", fabs(real_image[2][0].y) - fabs(y_));
                   ui->textEdit_2->append("Y'-y': ");
                   ui->textEdit_2->moveCursor(QTextCursor::End);
                   ui->textEdit_2->insertPlainText(QString::number(fabs(real_image[2][0].y) - fabs(y_)));

                   //printf("Y07_-y07_ = %.10lf\t", fabs(real_image[3][0].y) - fabs(y07_));
                   ui->textEdit_2->append("Y0.7'-y0.7': ");
                   ui->textEdit_2->moveCursor(QTextCursor::End);
                   ui->textEdit_2->insertPlainText(QString::number(fabs(real_image[3][0].y) - fabs(y07_)));

                   //printf("(Y_-y_)/y_ = %.10lf\t", (fabs(real_image[2][0].y) - fabs(y_)) / fabs(y_));
                   ui->textEdit_2->append("(Y'-y')/y': ");
                   ui->textEdit_2->moveCursor(QTextCursor::End);
                   ui->textEdit_2->insertPlainText(QString::number((fabs(real_image[2][0].y) - fabs(y_)) / fabs(y_)));

                   //printf("(Y07_-y07_)/y07_ = %.10lf\t",(fabs(real_image[3][0].y) - fabs(y07_)) / fabs(y07_));
                   ui->textEdit_2->append("(Y0.7'-y0.7')/y0.7': ");
                   ui->textEdit_2->moveCursor(QTextCursor::End);
                   ui->textEdit_2->insertPlainText(QString::number((fabs(real_image[3][0].y) - fabs(y07_)) / fabs(y07_)));

        // ????????????0.7?????????????????????YF_-YC_, Y07F_-Y07C_
                   ui->textEdit_2->append(" ");
                   //printf("\n????????????0.7??????????????????\n");
                   ui->textEdit_2->append("????????????0.7??????????????????");
                   //printf("YF_-YC_ = %.10lf\t", real_image[2][2].y - real_image[2][1].y);
                   ui->textEdit_2->append("YF'-YC': ");
                   ui->textEdit_2->moveCursor(QTextCursor::End);
                   ui->textEdit_2->insertPlainText(QString::number(real_image[2][2].y - real_image[2][1].y));

                   //printf("Y07F_-Y07C_ = %.10lf\t", real_image[3][2].y - real_image[3][1].y);
                   ui->textEdit_2->append("Y0.7F'-Y0.7C': ");
                   ui->textEdit_2->moveCursor(QTextCursor::End);
                   ui->textEdit_2->insertPlainText(QString::number(real_image[3][2].y - real_image[3][1].y));
               }

}

