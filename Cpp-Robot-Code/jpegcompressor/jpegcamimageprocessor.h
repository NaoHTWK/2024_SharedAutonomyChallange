#ifndef JPEGCAMIMAGEPROCESSOR_H
#define JPEGCAMIMAGEPROCESSOR_H

#include <array>
#include <atomic>
#include <cstdint>
#include <mutex>

#include <cammsg.h>
#include <jpegcompressor.h>
#include <jpegcompressorconfig.h>

#include <threadsafe_deque.h>

class JpegCamImageProcessor {
public:
    JpegCamImageProcessor();

    void process(cam_msg_t& msg);

    void setShouldJpegLog(bool shouldJpegLog) {
        should_jpeg_log = shouldJpegLog;
    }

private:
    std::optional<cam_msg_t> upperRawImage;
    std::mutex raw_upper_mtx;
    std::condition_variable raw_upper_cv;
    std::optional<cam_msg_t> lowerRawImage;
    std::mutex raw_lower_mtx;
    std::condition_variable raw_lower_cv;

    std::optional<JpegCompressor::JpegBuffer> upperJpegImage;
    std::optional<JpegCompressor::JpegBuffer> lowerJpegImage;
    std::mutex jpeg_mtx;
    std::condition_variable jpeg_cv;
    bool upper = true;

    std::atomic_bool should_compress{false};
    std::atomic_bool should_jpeg_log{false};

    [[noreturn]] void workerThread(CamID cam_id);
    [[noreturn]] void serverThread();
    std::optional<JpegCompressor::JpegBuffer> getBuffer();
};

#endif  // JPEGCAMIMAGEPROCESSOR_H
