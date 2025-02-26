#include "rec_setting.h"
#include "ui_rec_setting.h"
#include <QFileDialog>
#include "global_config.h"
#include "q_grab_windows.h"
#include <iostream>
rec_setting::rec_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rec_setting)
{
    ui->setupUi(this);
    init_values();
    set_connect();
}
void rec_setting::init_values(){
    ui->label->setText(global_config::getinstance().getDir());
    ui->comboBox->setCurrentText(QString::number(global_config::getinstance().get_frame_rates()));
}
void rec_setting::set_connect(){
    connect(ui->file_button,&QPushButton::clicked,this,[&](){
        QString directory_name = QFileDialog::getExistingDirectory(nullptr, tr("Open Directory"),"C:",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        ui->label->setText(directory_name);

        global_config& conf = global_config::getinstance();
        conf.setDir(directory_name);
    });
    connect(ui->comboBox,&QComboBox::currentTextChanged,this,[&](QString text){
        global_config::getinstance().set_frame_rates(text.toInt());
        int x = global_config::getinstance().get_frame_rates();
        std::cout<<x;

    });
    connect(ui->file_button_4,&QPushButton::clicked,this,[&](){
        if(grab_windows!=nullptr){
            delete grab_windows ;
        }
        grab_windows = new q_grab_windows;
        grab_windows->show();
    });

    //connect()
}
rec_setting::~rec_setting()
{
    delete ui;
}
