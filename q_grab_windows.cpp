#include "q_grab_windows.h"
#include <QPalette>
#include <QPixmap>
#include <QGuiApplication>
#include <QApplication>
#include <QScreen>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>


#include "global_config.h"
QRect getRect(QPoint x,QPoint y){
    return QRect{x.x(),x.y(),y.x()-x.x(),y.y()-x.y()};
}
q_grab_windows::q_grab_windows(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->showFullScreen();
    QScreen *screens = QGuiApplication::primaryScreen();
    screen = screens->grabWindow();
    //QPalette pallete = this->palette();
    //QScreen *screen = QGuiApplication::primaryScreen();
    //pallete.setBrush(QPalette::Window,QBrush(screen->grabWindow()));
    //QLabel *lab = new QLabel(this);
    //lab->resize(this->size());
    //lab->setPixmap(screen->grabWindow());
    //lab->setText("fffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
}


void q_grab_windows::paintEvent(QPaintEvent* event){
    QPainter p(this);
    p.drawPixmap(0,0,screen);
    QRect rect = getRect(startpoint,endpoint);
    p.setPen(QPen(Qt::red,5));
    p.drawRect(rect);
}



void q_grab_windows::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        left_pressed = true;
        startpoint = event->pos();
    }
}
void q_grab_windows::mouseReleaseEvent(QMouseEvent* event) {
    if(left_pressed){
        endpoint = event->pos();
        left_pressed = false;
        global_config::getinstance().set_random({getRect(startpoint,endpoint).x(),getRect(startpoint,endpoint).y(),getRect(startpoint,endpoint).width(),getRect(startpoint,endpoint).height()});
        this->close();
    }
}
void q_grab_windows::mouseMoveEvent(QMouseEvent* event) {
    if(left_pressed){
        endpoint = event->pos();
        this->update();
    }
}
