#include "main_diat.h"
#include "global_config.h"
#include "./ui_main_dia.h"
#include <QMouseEvent>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    thread_pool = new ThreadPool(3);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    time_counter.setInterval(1000);
    //record_vedio = new record_start();
    //record_vedio->init_vedio_info(REC_INFO::vedio_with_audio,"out.mp4",30);
    setConnect();
    setButtonIcons(ui->rec_button,":/icon/icon/record1.png");
    setButtonIcons(ui->set_button,":/icon/icon/setting1.png");
    //setButtonIcons(ui->mic_button,":/icon/icon/stop1.png");
    setButtonIcons(ui->audioc_button,":/icon/icon/audio_free.png");
}
void Widget::setConnect(){
    connect(&time_counter,&QTimer::timeout,this,&Widget::on_time_out);
    connect(this->ui->rec_button,&QPushButton::clicked,this,[&](){
        if(!rec_button_flag){
            startTime = QTime::currentTime();

            record_vedio = new record_start(thread_pool);
            record_vedio->init_vedio_info(rec_audio_flag?REC_INFO::vedio_with_audio:REC_INFO::only_vedio,
                                          global_config::getinstance().getDir()+"/"+QTime::currentTime().toString("hh_mm_ss")+".mp4",30);
            record_vedio->start_vedio_loop();
            time_counter.start();
            rec_button_flag = true;
        }
        else{
            time_counter.stop();
            record_vedio->end_vedio_loop();
            delete record_vedio;
            rec_button_flag = false;
            setButtonIcons(ui->rec_button,":/icon/icon/record1.png");
        }

    });
    connect(this->ui->set_button,&QPushButton::clicked,this,&Widget::show_setting_menu);
    connect(this->ui->audioc_button,&QPushButton::clicked,this,[&](){
        if(rec_audio_flag){
            setButtonIcons(ui->audioc_button,":/icon/icon/audio_free.png");
            rec_audio_flag = false;
        }
        else{
            setButtonIcons(ui->audioc_button,":/icon/icon/audio.png");
            rec_audio_flag = true;
        }
    });
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
    int currentTime = this->startTime.secsTo(QTime::currentTime());
    int hours = currentTime/3600;
    int minutes = (currentTime % 3600) / 60;
    int seconds = currentTime%60;

    QString timeDiffString = QString("%1:%2:%3")
                .arg(hours, 2, 10, QLatin1Char('0')) // 补零使小时至少两位
                .arg(minutes, 2, 10, QLatin1Char('0')) // 补零使分钟至少两位
                .arg(seconds, 2, 10, QLatin1Char('0')); // 补零使秒至少两位
    std::cout<<currentTime<<std::endl;;
    this->ui->lcdNumber->display(timeDiffString);
}


void Widget:: show_setting_menu(){
    recsetting = new rec_setting();
    //this->hide();
    recsetting->show();
}
