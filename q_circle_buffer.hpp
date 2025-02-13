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
    // ���캯������ʼ����������С
    explicit CircularBuffer(size_t capacity)
        : buffer_(capacity), capacity_(capacity), read_pos_(0), write_pos_(0), full_(false) {}

    // д������
    bool write(const T* data, size_t size) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (size > capacity_) {
            throw std::runtime_error("д�����ݴ�С��������������");
        }

        // ����Ƿ����㹻�Ŀռ�
        if (size > available_capacity()) {
            return false; // �������ռ䲻��
        }

        // д������
        for (size_t i = 0; i < size; ++i) {
            buffer_[write_pos_] = data[i];
            write_pos_ = (write_pos_ + 1) % capacity_;
        }

        // ���»�����״̬
        if (size > 0) {
            full_ = (write_pos_ == read_pos_);
        }

        lock.unlock();
        cv_.notify_all(); // ֪ͨ�ȴ��Ķ�ȡ�߳�
        return true;
    }

    // ��ȡ����
    bool read(T* data, size_t size) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this, size] { return available_data() >= size || stop_; });

        if (stop_) {
            return false; // ��������ֹͣ
        }

        // ��ȡ����
        for (size_t i = 0; i < size; ++i) {
            data[i] = buffer_[read_pos_];
            read_pos_ = (read_pos_ + 1) % capacity_;
        }

        // ���»�����״̬
        if (size > 0) {
            full_ = false;
        }

        return true;
    }

    // ��ȡ�ɶ�ȡ�����ݴ�С
    size_t available_data() const {
        if (full_) {
            return capacity_;
        }
        return (write_pos_ >= read_pos_) ? (write_pos_ - read_pos_) : (capacity_ - read_pos_ + write_pos_);
    }

    // ��ȡ��д��Ŀռ��С
    size_t available_capacity() const {
        return capacity_ - available_data();
    }

    // ֹͣ������
    void stop() {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
        cv_.notify_all();
    }

private:
    std::vector<T> buffer_;      // ���ݻ�����
    size_t capacity_;            // ����������
    size_t read_pos_;            // ��ȡλ��
    size_t write_pos_;           // д��λ��
    bool full_;                  // �������Ƿ�����
    bool stop_ = false;          // �Ƿ�ֹͣ������
    std::mutex mutex_;           // ������
    std::condition_variable cv_; // ��������
};

#endif // Q_CIRCLE_BUFFER_HPP
