#include "main_diat.h"
#include "./ui_main_dia.h"
#include <QMouseEvent>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    time_counter.setInterval(1000);
    record_vedio = new record_start();
    record_vedio->init_vedio_info(REC_INFO::vedio_with_audio,"out.mp4",30);
    connect(&time_counter,&QTimer::timeout,this,&Widget::on_time_out);
    connect(this->ui->rec_button,&QPushButton::clicked,this,[=](){
        if(!rec_button_flag){
            time_counter.start();
            record_vedio->start_vedio_loop();
            rec_button_flag = true;
        }
        else{
            time_counter.stop();
            record_vedio->end_vedio_loop();
            rec_button_flag = false;
            setButtonIcons(ui->rec_button,":/icon/icon/record1.png");
        }

    });
    connect(this->ui->set_button,&QPushButton::clicked,this,&Widget::show_setting_menu);
    setButtonIcons(ui->rec_button,":/icon/icon/record1.png");
    setButtonIcons(ui->set_button,":/icon/icon/setting1.png");
    setButtonIcons(ui->mic_button,":/icon/icon/stop1.png");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resetIcon(){
    //ui->pushButton->setIcon(QIcon(":/icon/icon/stop.png"));
    //ui->pushButton->setIconSize(QSize(ui->pushButton->size().width()-10,ui->pushButton->size().height()-10));
    setButtonIcons(ui->rec_button,":/icon/icon/stop1.png");
    //ui->pushButton->setStyleSheet("image: url(:/icon/icon/stop.png);");
}
void Widget::setButtonIcons(QPushButton *button,QString iconName){
    button->setIcon(QIcon(iconName));
    button->setIconSize((QSize(button->width()-10,button->height()-10)));
}
void Widget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        mouse_pressed = true;
        inner_pos = event->pos();
    }
}
void Widget::mouseMoveEvent(QMouseEvent *event){
    if(mouse_pressed){
        this->move(event->globalPos()-inner_pos);
    }
}
void Widget::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        mouse_pressed = false;
    }
}

void Widget::on_time_out(){
    time.second++;
    if(time.second == 60){
        time.second = 0;
        time.minute++;
    }
    if(time.minute == 60){
        time.minute =0;
        time.time++;

    }
    ui->time->setText(QString::number(time.time));
    ui->minute->setText(QString::number(time.minute));
    ui->second->setText(QString::number(time.second));
}


void Widget:: show_setting_menu(){
    recsetting = new rec_setting();
    this->hide();
    recsetting->show();
}
