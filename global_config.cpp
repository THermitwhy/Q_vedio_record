#include "global_config.h"
#include "utils.hpp"


global_config::global_config(){
    loadOrCreateJson("config.json");
    init_data();
}


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
void global_config::set_frame_rates(int frame_rates){
    this->frame_rates = frame_rates;
}

int global_config::get_frame_rates(){
    return this->frame_rates;
}

void global_config::set_random(screen_capture_radom random){
    this->radom = random;
}

screen_capture_radom global_config::get_random(){
    return this->radom;
}

 void global_config::loadOrCreateJson(const QString &filePath) {

     // 检查文件是否存在
     QFile file(filePath);
     if (file.exists()) {
         // 如果文件存在，打开并读取内容
         if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
             QByteArray fileData = file.readAll();
             file.close();

             // 解析JSON数据
             QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
             if (!jsonDoc.isNull() && jsonDoc.isObject()) {
                 file_json = jsonDoc.object();
             } else {
                 qWarning() << "Failed to parse JSON from file:" << filePath;
             }
         } else {
             qWarning() << "Failed to open file for reading:" << filePath;
         }
     } else {
         // 如果文件不存在，创建一个新的JSON对象
         file_json = QJsonObject();
         file_json["file_location"] = "G:"; // 你可以根据需要初始化一些默认值
         file_json["frame_rates"] = 30;
         auto capture_random = QJsonObject();
        capture_random["x"] = 0;
        capture_random["y"] = 0;
        int w,h;
        get_screen_size(w,h);
        capture_random["width"] = w;
        capture_random["height"] = h;
        file_json["capture_random"] = capture_random;
         // 将新的JSON对象写入文件
         QJsonDocument jsonDoc(file_json);
         if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
             file.write(jsonDoc.toJson());
             file.close();
         } else {
             qWarning() << "Failed to create and write to file:" << filePath;
         }
     }

     init_data();

 }


 void global_config::init_data(){
     QJsonObject random = file_json["capture_random"].toObject();
     this->setDir(file_json["file_location"].toString());
     this->set_frame_rates(file_json["frame_rates"].toInt());
     this->set_random({random["x"].toInt(),random["y"].toInt(),random["width"].toInt(),random["height"].toInt()});
 }
