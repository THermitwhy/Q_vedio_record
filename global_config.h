#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H
#include <QString>

class global_config
{
private:
    global_config(){};
    ~global_config(){};
    //global_config& operator=(const global_config&);
    QString dir;
public:
    static global_config& getinstance();
    void setDir(QString code);
    QString getDir();
};

#endif // GLOBAL_CONFIG_H
