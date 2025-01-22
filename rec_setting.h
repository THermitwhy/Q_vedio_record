#ifndef REC_SETTING_H
#define REC_SETTING_H

#include <QWidget>

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
    Ui::rec_setting *ui;
};

#endif // REC_SETTING_H
