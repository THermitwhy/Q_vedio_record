#ifndef B_UTILS
#define B_UTILS
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <libyuv.h> // 包含 libyuv 库
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

}

#include "global_config.h"
static int createAVFrame(int width, int height, AVFrame* &frame) {
    if (frame == nullptr) {
        frame = av_frame_alloc();
    }
    //AVFrame* frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate AVFrame\n");
        return -1;
    }

    frame->format = AV_PIX_FMT_YUV420P;
    frame->width = width;
    frame->height = height;
    frame->time_base = { 1,30 };
    int ret = av_image_alloc(frame->data, frame->linesize, width, height, AV_PIX_FMT_YUV420P, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        av_frame_free(&frame);
        return -1;
    }

    return 0;
}
 inline int convertYUVToAVFrame(uint8_t* yuv420, int width, int height, int y_stride, int u_stride, int v_stride, AVFrame* &frame) {
    if (frame == nullptr) {
        createAVFrame(width, height, frame);
    }
    if (!frame) {
        return -1;
    }
        

    // Copy Y plane
    for (int i = 0; i < height; ++i) {
        //memcpy(frame->data[0] + i * frame->linesize[0], yuv420 + (height - 1 - i) * y_stride, width);
        memcpy(frame->data[0] + i * frame->linesize[0], yuv420 + i * y_stride, width);
    }

    // Copy U plane
    for (int i = 0; i < height / 2; ++i) {
        //memcpy(frame->data[1] + i * frame->linesize[1], yuv420 + y_stride * height + (height / 2 - 1 - i) * u_stride, width / 2);
        memcpy(frame->data[1] + i * frame->linesize[1], yuv420 + y_stride * height + i * u_stride, width / 2);
    }

    // Copy V plane
    for (int i = 0; i < height / 2; ++i) {
        //memcpy(frame->data[2] + i * frame->linesize[2], yuv420 + y_stride * height + u_stride * (height / 2) + (height / 2 - 1 - i) * v_stride, width / 2);
        memcpy(frame->data[2] + i * frame->linesize[2], yuv420 + y_stride * height + u_stride * (height / 2) +  i * v_stride, width / 2);
    }

    return 0;
}
 inline void get_screen_size(int &x,int &y) {
     x= GetSystemMetrics(SM_CXSCREEN);
     y = GetSystemMetrics(SM_CYSCREEN);
//     int w = GetSystemMetrics(SM_CXSCREEN);
//     int h = GetSystemMetrics(SM_CYSCREEN);
//     int zoom = GetDpiForWindow(GetDesktopWindow());
//     switch (zoom)
//     {
//     case 96: x = w / (1/1); y = h / (1 / 1);
//         break;
//     case 120: x = w / (1 / 1.25); y = h / (1 / 1.25);
//         break;
//     case 144:x = w / (1 / 1.5); y = h / (1 / 1.5);
//         break;
//     case 192:x = w / (1 / 2); y = h / (1 / 2);
//         break;
//     }
 }
// inline AVCodecParameters* get_vedio_codec_params(){
//    AVCodecParameters* codec_params;
//    codec_params = avcodec_parameters_alloc();
//    codec_params->codec_id = AV_CODEC_ID_H264;
//    codec_params->codec_type = AVMEDIA_TYPE_VIDEO;
//    codec_params->width = global_config::getinstance().get_random().width;
//    codec_params->height = global_config::getinstance().get_random().height;
//    codec_params->format = AV_PIX_FMT_YUV420P;
//    codec_params->bit_rate = 12000000;
//    codec_params->profile = FF_PROFILE_H264_BASELINE;
//    codec_params->level = 41;

//    return codec_params;
// }

//inline  AVCodecContext* get_vedio_codec_context(AVCodecParameters* params){
//    const AVCodec* codec = avcodec_find_encoder(params->codec_id);
//    AVCodecContext* context = avcodec_alloc_context3(codec);
//    if (!context) {
//        //std::cerr << "无法分配编码器上下文" << std::endl;
//        throw std::runtime_error("无法分配编码器上下文");
//    }
//    if (avcodec_parameters_to_context(context, params) < 0) {
//        //std::cerr << "无法将编码器参数复制到编码器上下文" << std::endl;
//        avcodec_free_context(&context);
//        throw std::runtime_error("无法将编码器参数复制到编码器上下文");

//    }
//    return context;
//}
//inline void set_vedio_stream(AVCodecContext* context,AVStream* vedio_stream){
//    if (avcodec_parameters_from_context(vedio_stream->codecpar, context) < 0) {
//        //std::cerr << "无法设置流的编解码器参数" << std::endl;
//        throw std::runtime_error("无法设置流的编解码器参数");
//    }
//}
#endif
