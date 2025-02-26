#ifndef REC_SETTING_H
#define REC_SETTING_H

#include <QWidget>
#include "q_grab_windows.h"
namespace Ui {
class rec_setting;
}

class rec_setting : public QWidget
{
    Q_OBJECT

public:
    explicit rec_setting(QWidget *parent = nullptr);
    ~rec_setting();

private:
    void set_connect();
    void init_values();
    Ui::rec_setting *ui;
    q_grab_windows * grab_windows = nullptr;
};

#endif // REC_SETTING_H
