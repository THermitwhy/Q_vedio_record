#ifndef Q_CIRCLE_BUFFER_HPP
#define Q_CIRCLE_BUFFER_HPP
#include <vector>
#include <vector>
#include <atomic>
#include <stdexcept>
#include <condition_variable>
#include <mutex>
template <typename T>
class CircularBuffer {
public:
    // 构造函数，初始化缓冲区大小
    explicit CircularBuffer(size_t capacity)
        : buffer_(capacity), capacity_(capacity), read_pos_(0), write_pos_(0), full_(false) {}

    // 写入数据
    bool write(const T* data, size_t size) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (size > capacity_) {
            throw std::runtime_error("写入数据大小超过缓冲区容量");
        }

        // 检查是否有足够的空间
        if (size > available_capacity()) {
            return false; // 缓冲区空间不足
        }

        // 写入数据
        for (size_t i = 0; i < size; ++i) {
            buffer_[write_pos_] = data[i];
            write_pos_ = (write_pos_ + 1) % capacity_;
        }

        // 更新缓冲区状态
        if (size > 0) {
            full_ = (write_pos_ == read_pos_);
        }

        lock.unlock();
        cv_.notify_all(); // 通知等待的读取线程
        return true;
    }

    // 读取数据
    bool read(T* data, size_t size) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this, size] { return available_data() >= size || stop_; });

        if (stop_) {
            return false; // 缓冲区已停止
        }

        // 读取数据
        for (size_t i = 0; i < size; ++i) {
            data[i] = buffer_[read_pos_];
            read_pos_ = (read_pos_ + 1) % capacity_;
        }

        // 更新缓冲区状态
        if (size > 0) {
            full_ = false;
        }

        return true;
    }

    // 获取可读取的数据大小
    size_t available_data() const {
        if (full_) {
            return capacity_;
        }
        return (write_pos_ >= read_pos_) ? (write_pos_ - read_pos_) : (capacity_ - read_pos_ + write_pos_);
    }

    // 获取可写入的空间大小
    size_t available_capacity() const {
        return capacity_ - available_data();
    }

    // 停止缓冲区
    void stop() {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
        cv_.notify_all();
    }

private:
    std::vector<T> buffer_;      // 数据缓冲区
    size_t capacity_;            // 缓冲区容量
    size_t read_pos_;            // 读取位置
    size_t write_pos_;           // 写入位置
    bool full_;                  // 缓冲区是否已满
    bool stop_ = false;          // 是否停止缓冲区
    std::mutex mutex_;           // 互斥锁
    std::condition_variable cv_; // 条件变量
};

#endif // Q_CIRCLE_BUFFER_HPP
