#ifndef VEDIO_META_H
#define VEDIO_META_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

}

class vedio_meta
{
public:
    vedio_meta(AVStream* stream,AVFormatContext* format_ctx);
    AVStream* stream;
    AVFormatContext* format_ctx;
};

#endif // VEDIO_META_H
