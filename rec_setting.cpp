#include "rec_setting.h"
#include "ui_rec_setting.h"

rec_setting::rec_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rec_setting)
{
    ui->setupUi(this);
}

rec_setting::~rec_setting()
{
    delete ui;
}
