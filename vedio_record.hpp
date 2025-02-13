#ifndef VEDIO_RECORD_HPP
#define VEDIO_RECORD_HPP
#include <QObject>
#include "threadPool.hpp"
#include "bvedio.h"
#include "baudio.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libavutil/log.h"
#include "libswresample/swresample.h"

#include "libavutil/avutil.h"
#include "libavutil/opt.h"
}
enum REC_INFO{
    only_vedio = 0,
    vedio_with_audio
};

class record_start:public QObject{
        Q_OBJECT
public:
    record_start(ThreadPool* thread_pool_){
        thread_pool = thread_pool_;
    };
    void init_vedio_info(REC_INFO rec_info,QString vedio_name,int vedio_fps){
        this->vedio_name = vedio_name;
        this->vedio_fps = vedio_fps;
        recinfo = rec_info;
        avformat_alloc_output_context2(&format_ctx, nullptr, nullptr, vedio_name.toStdString().c_str());
        vedio_stream = avformat_new_stream(format_ctx, NULL);

        if(rec_info){
            audio_stream = avformat_new_stream(format_ctx, NULL);
        }
        init_instance();
        test_vedio_info();
        write_vedio_head();

    }
    void test_vedio_info(){
        if (!(format_ctx->oformat->flags & AVFMT_NOFILE)) {
            if (avio_open(&format_ctx->pb, this->vedio_name.toStdString().c_str(), AVIO_FLAG_WRITE) < 0) {
                //std::cerr << "无法打开输出文件" << std::endl;
                //avcodec_free_context(&codec_ctx);
                //avcodec_parameters_free(&codec_params);
                avformat_free_context(format_ctx);
                throw std::runtime_error("无法打开输出文件");
            }
        }
    }
    void write_vedio_head(){
        av_dict_set_int(&opt, "video_track_timescale", this->vedio_fps, 0);
            int ret = avformat_write_header(format_ctx, &opt);
            if (ret < 0) {
                //std::cerr << "无法写入文件头" << std::endl;
                //avcodec_free_context(&codec_ctx);
                //avcodec_parameters_free(&codec_params);
                avformat_free_context(format_ctx);
                throw std::runtime_error("无法写入文件头");
            }
    }
    void init_instance(){
        video_thread = new bvedio(this->vedio_fps, vedio_stop, vedio_stream, format_ctx);
        if(recinfo){
            audio_thread = new rec_au(audio_stop, format_ctx, audio_stream);
            audio_thread->init_device();
        }
    }
    void start_vedio_loop(){
        thread_pool->submit([&]() {
            video_thread->loop();
            });
        if(recinfo){
        thread_pool->submit([&]() {
            audio_thread->loop();
        });}
    }
    ~record_start(){
        //delete thread_pool;
        //delete audio_thread;
        if(recinfo){
            delete audio_thread;
        }
        delete video_thread;

        avformat_free_context(format_ctx);

    };
    void end_vedio_loop(){
        if(recinfo){
            vedio_stop.store(false);
            audio_stop.store(false);
            while (!vedio_stop.load() || !audio_stop.load()) {}
            if (av_write_trailer(format_ctx) < 0) {
                //std::cerr << "无法写入文件尾" << std::endl;
                throw std::runtime_error("无法写入文件尾");
            }
            //av_write_trailer(format_ctx); // 写入 moov 原子

                // 关闭文件
            if (format_ctx && !(format_ctx->oformat->flags & AVFMT_NOFILE)) {
                avio_closep(&format_ctx->pb);
                }
            return;
        }
        vedio_stop.store(false);
        while (!vedio_stop.load()) {}
        if (av_write_trailer(format_ctx) < 0) {
            //std::cerr << "无法写入文件尾" << std::endl;
            throw std::runtime_error("无法写入文件尾");
        }
        if (format_ctx && !(format_ctx->oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&format_ctx->pb);
            }
    }

private:
    AVFormatContext* format_ctx = nullptr;
    AVStream* vedio_stream = nullptr;
    AVStream* audio_stream = nullptr;
    AVDictionary* opt = nullptr;
    QString vedio_name = "defalut";
    int vedio_fps = 30;
    ThreadPool *thread_pool = nullptr;
    REC_INFO recinfo = REC_INFO::only_vedio;
    bvedio* video_thread;
    rec_au* audio_thread;
    std::atomic<bool> vedio_stop = true;
    std::atomic<bool> audio_stop = true;
};

#endif // VEDIO_RECORD_HPP
