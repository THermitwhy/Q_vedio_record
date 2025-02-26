#include "bvedio.h"
#include <fstream>
#include <exception>
#include <chrono>

bvedio::bvedio(int fps, std::atomic<bool>& stop_flag, AVStream* stream, AVFormatContext* format_ctx,SafePacketQueue &queue) :vedio_meta(stream,format_ctx),
    vedio_fps(fps), stop_flag(stop_flag),queue(queue)
{
    hWnd = GetDesktopWindow();
    //initRect();
    auto [x,y,w,h]  = global_config::getinstance().get_random();
    y_stride = w;
    u_stride = (w + 1) / 2;
    v_stride = (w + 1) / 2;
    yuv_data.resize(y_stride * h + u_stride * (h / 2) + v_stride * (h / 2),0);
    initHeader();
    //captureinstance = new capture_windows_instance(params.width, params.height, hWnd);
    sc = new ScreenCapture(w, h);
}

bvedio::~bvedio()
{
    endLoop();

}

void bvedio::initHeader()
{

//    stream->time_base = { 1, global_config::getinstance().get_frame_rates() };

    get_vedio_codec_params();
    get_vedio_codec_context();
    set_vedio_stream();
    //avcodec_parameters_free(params);
    // 设置编解码器参数
//    codec_params = avcodec_parameters_alloc();
//    codec_params->codec_id = AV_CODEC_ID_H264;
//    codec_params->codec_type = AVMEDIA_TYPE_VIDEO;
//    codec_params->width = params.width;
//    codec_params->height = params.height;
//    codec_params->format = AV_PIX_FMT_YUV420P;
//    codec_params->bit_rate = params.bitrate;
//    codec_params->profile = FF_PROFILE_H264_BASELINE;
//    codec_params->level = 41;
//    //codec_params->time_base
   
    



//    //codec = avcodec_find_encoder(codec_params->codec_id);
//    codec = avcodec_find_encoder_by_name("h264_nvenc");//硬件编码
//    if (!codec) {

//        //std::cerr << "无法找到编码器" << std::endl;
//        avcodec_parameters_free(&codec_params);
//        avformat_free_context(format_ctx);
//        throw std::runtime_error("无法找到编码器");
//    }

//    codec_ctx = avcodec_alloc_context3(codec);
//    if (!codec_ctx) {
//        //std::cerr << "无法分配编码器上下文" << std::endl;
//        avcodec_parameters_free(&codec_params);
//        avformat_free_context(format_ctx);
//        throw std::runtime_error("无法分配编码器上下文");
//    }

//    if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0) {
//        //std::cerr << "无法将编码器参数复制到编码器上下文" << std::endl;
//        avcodec_free_context(&codec_ctx);
//        avcodec_parameters_free(&codec_params);
//        avformat_free_context(format_ctx);
//        throw std::runtime_error("无法将编码器参数复制到编码器上下文");

//    }
//    codec_ctx->gop_size = 30; // 每 30 帧一个关键帧
//    codec_ctx->max_b_frames = 0; // 每个 GOP 中最多包含 0 个 B 帧
//    codec_ctx->time_base = { 1, params.frame_rate };
//    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
//        //std::cerr << "无法打开编码器" << std::endl;
//        avcodec_free_context(&codec_ctx);
//        avcodec_parameters_free(&codec_params);
//        avformat_free_context(format_ctx);
//        throw std::runtime_error("无法打开编码器");
//    }
//    //video_stream = avformat_new_stream(format_ctx, codec);
//    // 设置流的编解码器参数
//    if (avcodec_parameters_from_context(stream->codecpar, codec_ctx) < 0) {
//        //std::cerr << "无法设置流的编解码器参数" << std::endl;
//        avcodec_free_context(&codec_ctx);
//        avcodec_parameters_free(&codec_params);
//        avformat_free_context(format_ctx);
//        throw std::runtime_error("无法设置流的编解码器参数");
//    }



}


void writeYUVToFile(const char* filename, uint8_t* yuv420, int width, int height) {
    std::ofstream outfile(filename, std::ios::out | std::ios::binary);
    if (!outfile) {
        std::cerr << "无法打开文件以写入YUV数据" << std::endl;
        return;
    }

    int y_size = width * height;
    int u_size = y_size / 4;
    int v_size = y_size / 4;

    // 写入 Y 平面数据
    outfile.write(reinterpret_cast<char*>(yuv420), y_size);

    // 写入 U 平面数据
    outfile.write(reinterpret_cast<char*>(yuv420 + y_size), u_size);

    // 写入 V 平面数据
    outfile.write(reinterpret_cast<char*>(yuv420 + y_size + u_size), v_size);

    outfile.close();
}
int bvedio::loop()
{
    //counts = counts * params.frame_rate;
    int frame_count = 0;
//    int y_stride = params.width;
//    int u_stride = (params.width + 1) / 2;
//    int v_stride = (params.width + 1) / 2;
    //uint8_t* yuv420 = new uint8_t[y_stride * params.height + u_stride * (params.height / 2) + v_stride * (params.height / 2)];
    auto [x,y,w,h]  = global_config::getinstance().get_random();
    std::vector<BYTE> buffer(w * h * 4);
    //int delay_time = 1000 / params.frame_rate;
    std::chrono::milliseconds delay_time(1000 / global_config::getinstance().get_frame_rates());
    AVPacket* pkt;
    pkt = queue.getPacket();
    //pkt = av_packet_alloc();
    //pkt->stream_index = video_stream->index;
    while (stop_flag.load()) {

        auto start = std::chrono::high_resolution_clock::now();

        

        sc->Capture(buffer.data());
        libyuv::ARGBToI420(
            (const uint8_t*)buffer.data(), w*4,
            yuv_data.data(), y_stride,
            yuv_data.data() + (y_stride * h), u_stride,
            yuv_data.data() + (y_stride * h) + (u_stride * (h / 2)), v_stride,
            w, h
        );

        auto t1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - start);
        //std::cout << duration1.count() << std::endl;
        convertYUVToAVFrame(yuv_data.data(), w, h, y_stride, u_stride, v_stride,current_frame);
        //std::cout << sizeof(yuv420) << std::endl;
        
        if (!current_frame) {
            fprintf(stderr, "Error converting YUV to AVFrame\n");
           
            return -1;
        }

        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        // 设置帧的时间戳
        
        //frame_count++;
        current_frame->pts = frame_count;
        int ret = avcodec_send_frame(codec_ctx, current_frame);
        // 发送帧给编码器
        if (ret < 0) {
            std::cerr << "无法发送帧给编码器" << std::endl;
            av_frame_free(&current_frame);
            return -1;
        }
        //av_frame_unref(current_frame);
        //pkt->data = nullptr;
        //pkt->size = 0;
        //pkt->time_base = codec_ctx->time_base;
        //av_packet_rescale_ts(pkt, this->codec_ctx->time_base, this->video_stream->time_base);
        while (avcodec_receive_packet(codec_ctx, pkt) == 0) {
            av_packet_rescale_ts(pkt, this->codec_ctx->time_base, this->stream->time_base);
            //std::cout << pkt->pts << std::endl;
            queue.push(pkt);
            pkt = queue.getPacket();
        }
        //av_packet_unref(pkt);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        if (duration < delay_time) {
            while (std::chrono::high_resolution_clock::now() < start + delay_time) {

            }
            //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start) << std::endl;
        }
        ++frame_count;
    }
    avcodec_send_frame(codec_ctx, nullptr);

    // 从编码器接收编码数据并写入文件
    //AVPacket pkt;
    //av_init_packet(&pkt);
    //pkt->data = nullptr;
    //pkt->size = 0;
    //pkt->time_base = codec_ctx->time_base;
    //av_packet_rescale_ts(pkt, this->codec_ctx->time_base, this->video_stream->time_base);
    while (avcodec_receive_packet(codec_ctx, pkt) == 0) {
        av_packet_rescale_ts(pkt, this->codec_ctx->time_base, this->stream->time_base);
        queue.push(pkt);
        pkt = queue.getPacket();
    }
    av_frame_free(&current_frame);
    //delete[] yuv420;
    stop_flag.store(true);
    return 1;
}







void bvedio::endLoop()
{
    delete sc;
    avcodec_free_context(&codec_ctx);
    avcodec_parameters_free(&this->codec_params);
    //avformat_free_context(format_ctx);
}
