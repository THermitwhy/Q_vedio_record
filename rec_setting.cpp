#include "rec_setting.h"
#include "ui_rec_setting.h"
#include <QFileDialog>
#include "global_config.h"
rec_setting::rec_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rec_setting)
{
    ui->setupUi(this);
    connect(ui->file_button,&QPushButton::clicked,this,[&](){
        QString directory_name = QFileDialog::getExistingDirectory(nullptr, tr("Open Directory"),"C:",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        ui->label->setText(directory_name);

        global_config& conf = global_config::getinstance();
        conf.setDir(directory_name);
    });
}

rec_setting::~rec_setting()
{
    delete ui;
}
