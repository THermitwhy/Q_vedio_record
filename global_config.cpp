#include "global_config.h"

 global_config& global_config::getinstance(){
     static global_config instance;
     return instance;
 }

 void global_config::setDir(QString code){
     dir = code;
 }
 QString global_config::getDir(){
    return dir;
}
