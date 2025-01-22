#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include "rec_setting.h"
#include "vedio_record.hpp"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
struct times{
    int time;
    int minute;
    int second;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QPoint inner_pos;
    bool mouse_pressed=false;
    Ui::Widget *ui;
    QTimer time_counter;
    rec_setting *recsetting;
    times time{0, 0, 0};
    record_start *record_vedio = nullptr;
    bool rec_button_flag = false;
    void setButtonIcons(QPushButton *button,QString iconName);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void on_time_out();
    void show_setting_menu();
private slots:
    void resetIcon();

};
#endif // WIDGET_H
