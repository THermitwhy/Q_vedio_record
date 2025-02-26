#ifndef Q_WRITE_QUEUE_HPP
#define Q_WRITE_QUEUE_HPP
#pragma once
#include <queue>
#include <mutex>
#include <queue>
#include <mutex>
#include <condition_variable>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}


class SafePacketQueue {
public:
    SafePacketQueue(size_t max_size = 10) : max_size(max_size) {}

    // 从空闲队列中获取一个 AVPacket
    AVPacket* getPacket() {
        std::unique_lock<std::mutex> lock(mutex);
        if (freeQueue.empty()) {
            // 如果空闲队列为空，分配一个新的 AVPacket
            return av_packet_alloc();
        }
        else {
            // 从空闲队列中取出一个 AVPacket
            AVPacket* pkt = freeQueue.front();
            freeQueue.pop();
            return pkt;
        }
    }

    // 将数据包加入数据队列
    bool push(AVPacket* pkt) {
        std::unique_lock<std::mutex> lock(mutex);
        // 如果队列已满，等待直到有空间
        cond_full.wait(lock, [this]() { return dataQueue.size() < max_size; });

        dataQueue.push(pkt);
        lock.unlock();
        cond_empty.notify_one(); // 通知等待的消费者
        return true;
    }

    // 从数据队列中取出数据包
    AVPacket* pop() {
        std::unique_lock<std::mutex> lock(mutex);
        // 如果队列为空，等待直到有数据包
        cond_empty.wait(lock, [this]() { return !dataQueue.empty(); });

        AVPacket* pkt = dataQueue.front();
        dataQueue.pop();
        lock.unlock();
        cond_full.notify_one(); // 通知等待的生产者
        return pkt;
    }

    // 将使用完的 AVPacket 放回空闲队列
    void releasePacket(AVPacket* pkt) {
        std::unique_lock<std::mutex> lock(mutex);
        av_packet_unref(pkt); // 释放内部资源
        freeQueue.push(pkt);   // 放回空闲队列
    }

    // 检查数据队列是否为空
    bool empty() {
        std::lock_guard<std::mutex> lock(mutex);
        return dataQueue.empty();
    }

private:
    std::queue<AVPacket*> dataQueue; // 数据队列
    std::queue<AVPacket*> freeQueue; // 空闲队列
    std::mutex mutex; // 互斥锁
    std::condition_variable cond_empty; // 数据队列为空的条件变量
    std::condition_variable cond_full; // 数据队列满的条件变量
    size_t max_size; // 数据队列的最大容量
};

inline void writeThread(AVFormatContext* fmtContext, SafePacketQueue& videoQueue, SafePacketQueue& audioQueue, std::atomic<bool> &write_flag) {

    while (write_flag) {
        // 从数据队列中取出视频数据包
        if (!videoQueue.empty()) {
            AVPacket* video_pkt = videoQueue.pop();
            int ret = av_interleaved_write_frame(fmtContext, video_pkt);
            //std::cout << "write" <<ret<< std::endl;
            videoQueue.releasePacket(video_pkt); // 释放并放回空闲队列
        }

         //从数据队列中取出音频数据包
        if (!audioQueue.empty()) {
            AVPacket* audio_pkt = audioQueue.pop();
            int ret = av_interleaved_write_frame(fmtContext, audio_pkt);
            //std::cout << "write:" << ret << std::endl;
            audioQueue.releasePacket(audio_pkt); // 释放并放回空闲队列
        }
    }
}
#endif // Q_WRITE_QUEUE_HPP
