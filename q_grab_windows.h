#ifndef Q_GRAB_WINDOWS_H
#define Q_GRAB_WINDOWS_H

#include <QWidget>

class q_grab_windows : public QWidget
{
    Q_OBJECT
public:
    explicit q_grab_windows(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    bool left_pressed = false;
    QPoint startpoint{0,0},endpoint{0,0};
private:
    QRect rect{0,0,0,0};
    QPixmap screen;
signals:

};

#endif // Q_GRAB_WINDOWS_H
