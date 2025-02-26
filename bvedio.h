#pragma once
#ifndef B_VEDIO
#define B_VEDIO
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <vector>
#include "utils.hpp"
#include "dx_capture.hpp"
#include "q_write_queue.hpp"
#include <atomic>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

}

#include "vedio_meta.h"
class bvedio:public vedio_meta {
public:
    bvedio(int fps,std::atomic<bool>& stop_flag, AVStream* stream, AVFormatContext* format_ctx,SafePacketQueue &queue);
	~bvedio();
	HWND hWnd ;
	//capture_windows_instance *captureinstance;
	ScreenCapture *sc;
	int vedio_fps;
	std::atomic<bool>& stop_flag;
	//编码器上下文
	AVCodecContext* codec_ctx;
    const AVCodec* codec;

	//音频
	// 编解码器参数
    AVCodecParameters* codec_params;
	// 视频流
    //AVStream* video_stream;
	// 输出格式
    //AVFormatContext* format_ctx = nullptr;
	// 文件头
	AVDictionary* opt = nullptr;
	//即时帧
	AVFrame* current_frame = nullptr;
    SafePacketQueue &queue;

	int count = 0;

    int y_stride;// = params.width;
    int u_stride;// = (params.width + 1) / 2;
    int v_stride;// = (params.width + 1) / 2;
    std::vector<BYTE> yuv_data;
	void initHeader();
    //void initRect();
	int loop();
	void recordLoop();
	void captureFrame(int frame_count);
	//void release
	void endLoop();

    // 直接在头文件里定义的函数

     void get_vedio_codec_params(){
       //AVCodecParameters* codec_params;
       codec_params = avcodec_parameters_alloc();
       codec_params->codec_id = AV_CODEC_ID_H264;
       codec_params->codec_type = AVMEDIA_TYPE_VIDEO;
       codec_params->width = global_config::getinstance().get_random().width;
       codec_params->height = global_config::getinstance().get_random().height;
       codec_params->format = AV_PIX_FMT_YUV420P;
       codec_params->bit_rate = 12000000;
       codec_params->profile = FF_PROFILE_H264_BASELINE;
       codec_params->level = 41;

       //return codec_params;
    }

     void get_vedio_codec_context(){
       codec = avcodec_find_encoder(codec_params->codec_id);
       codec_ctx = avcodec_alloc_context3(codec);
       if (!codec_ctx) {
           //std::cerr << "无法分配编码器上下文" << std::endl;
           throw std::runtime_error("无法分配编码器上下文");
       }
       if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0) {
           //std::cerr << "无法将编码器参数复制到编码器上下文" << std::endl;
           avcodec_free_context(&codec_ctx);
           throw std::runtime_error("无法将编码器参数复制到编码器上下文");

       }
       codec_ctx->time_base = { 1, global_config::getinstance().get_frame_rates() };
           if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
               //std::cerr << "无法打开编码器" << std::endl;
               avcodec_free_context(&codec_ctx);
               avcodec_parameters_free(&codec_params);
               avformat_free_context(format_ctx);
               throw std::runtime_error("无法打开编码器");
           }
       //return context;
   }
    void set_vedio_stream(){
       if (avcodec_parameters_from_context(stream->codecpar, codec_ctx) < 0) {
           //std::cerr << "无法设置流的编解码器参数" << std::endl;
           throw std::runtime_error("无法设置流的编解码器参数");
       }
       stream->time_base = { 1, global_config::getinstance().get_frame_rates() };
   }
};


#endif
