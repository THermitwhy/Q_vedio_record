#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QIODevice>
#include <QFile>
#include <libavcodec/avcodec.h>

inline std::map<AVCodecID,std::vector<const AVCodec*>> list_encoders() {
    const AVCodec* codec = nullptr;
    void* iter = nullptr;
    std::map<AVCodecID,std::vector<const AVCodec*>> ret;
    // 遍历所有注册的编解码器
    while ((codec = av_codec_iterate(&iter))) {
        if(av_codec_is_encoder(codec)){  // 检查是否为编码器
            AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
            int rets = avcodec_open2(codec_ctx, codec, nullptr);
            if(rets>=0){
                ret[codec->id].push_back(codec);
            }

        }
    }
    return ret;
}

struct screen_capture_radom{
    int x;
    int y;
    int width;
    int height;
};

class global_config
{
private:
    global_config();
    ~global_config(){};
    //global_config& operator=(const global_config&);
    QString dir="C:";
    int frame_rates = 30;
    screen_capture_radom radom{0,0,1920,1080};
    QJsonObject file_json;
public:
    static global_config& getinstance();
    void setDir(QString code);
    void set_frame_rates(int frame_rates);
    int get_frame_rates();
    void set_random(screen_capture_radom random);
    screen_capture_radom get_random();
    QString getDir();
    void loadOrCreateJson(const QString &filePath);
    void init_data();
};

#endif // GLOBAL_CONFIG_H
