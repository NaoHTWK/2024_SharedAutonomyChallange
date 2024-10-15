#include "jpegcamimageprocessor.h"

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

//#include <easy/profiler.h>
//#include <flightrecorderlog.h>
#include <jpegcompressor.h>
#include <stl_ext.h>

using namespace boost::asio::ip;
namespace bf = boost::filesystem;

JpegCamImageProcessor::JpegCamImageProcessor() {
    launch_named_thread("JpegSrv", true, [this]() { serverThread(); }).detach();
    launch_named_thread("JpegWorker1", true, [this]() { workerThread(CamID::UPPER); }).detach();
    launch_named_thread("JpegWorker2", true, [this]() { workerThread(CamID::LOWER); }).detach();
}

void JpegCamImageProcessor::process(cam_msg_t& msg) {
    //EASY_FUNCTION();
    if (should_jpeg_log) {
        if (msg.id == CamID::UPPER) {
            std::lock_guard<std::mutex> lck(raw_upper_mtx);
            upperRawImage = msg;
            raw_upper_cv.notify_all();
        } else {
            std::lock_guard<std::mutex> lck(raw_lower_mtx);
            lowerRawImage = msg;
            raw_lower_cv.notify_all();
        }
    }
}

void JpegCamImageProcessor::workerThread(CamID cam_id) {
    const int subsampling = 1;
    int quality = 10;

    JpegCompressor compressor;

    //FlightRecorder::LogPtr jpegLog = FlightRecorder::FlightRecorderLog::instance("JpegLogger");

    while (true) {
        cam_msg_t img;
        if (cam_id == CamID::UPPER) {
            std::unique_lock<std::mutex> lck(raw_upper_mtx);
            if (!upperRawImage) {
                raw_upper_cv.wait(lck, [&]() { return upperRawImage; });
            }
            img = *upperRawImage;
            upperRawImage = std::nullopt;
        } else {
            std::unique_lock<std::mutex> lck(raw_lower_mtx);
            if (!lowerRawImage) {
                raw_lower_cv.wait(lck, [&]() { return lowerRawImage; });
            }
            img = *lowerRawImage;
            lowerRawImage = std::nullopt;
        }
        compressor.yuv422_to_jpeg(img.img.get(), img.id, quality, false, subsampling);

        //EASY_BLOCK("jpegImage::push_back");
        auto jpeg = compressor.getJpeg();
        if (cam_id == CamID::UPPER) {
            std::lock_guard<std::mutex> lck(jpeg_mtx);
            upperJpegImage = jpeg;
            jpeg_cv.notify_all();
        } else {
            std::lock_guard<std::mutex> lck(jpeg_mtx);
            lowerJpegImage = jpeg;
            jpeg_cv.notify_all();
        }
        //EASY_END_BLOCK;
    }
}

std::optional<JpegCompressor::JpegBuffer> JpegCamImageProcessor::getBuffer() {
    if (upper) {
        std::lock_guard<std::mutex> lck(jpeg_mtx);
        if (upperJpegImage) {
            auto tmp = upperJpegImage;
            upperJpegImage = std::nullopt;
            upper = false;
            return tmp;
        }
    } else {
        std::lock_guard<std::mutex> lck(jpeg_mtx);
        if (lowerJpegImage) {
            auto tmp = lowerJpegImage;
            lowerJpegImage = std::nullopt;
            upper = true;
            return tmp;
        }
    }
	std::unique_lock<std::mutex> lck(jpeg_mtx);
    jpeg_cv.wait(lck, [&]() { return upperJpegImage || lowerJpegImage; });
    if (upperJpegImage) {
        auto tmp = upperJpegImage;
        upperJpegImage = std::nullopt;
        upper = false;
        return tmp;
    }
    if (lowerJpegImage) {
        auto tmp = lowerJpegImage;
        lowerJpegImage = std::nullopt;
        upper = true;
        return tmp;
    }
    return {};
}

void JpegCamImageProcessor::serverThread() {
    using namespace std::chrono_literals;

    //FlightRecorder::LogPtr jpegLog = FlightRecorder::FlightRecorderLog::instance("JpegCompressor");

    boost::asio::io_service io_service;
    boost::system::error_code ec;
    
    std::string debug_pc_address = "10.0.13.200";
    bf::path debug_pc_file("/home/nao/firmware/etc/infocast_debug_pc");
    if (bf::exists(debug_pc_file) && bf::is_regular_file(debug_pc_file)) {
        boost::filesystem::ifstream is(debug_pc_file);
        std::string line;
        getline(is, line);
        
        if (line != std::string())
            debug_pc_address = line;
    }

    udp::socket socket(io_service, udp::endpoint(udp::v4(), 9999));
    udp::endpoint remote_endpoint(boost::asio::ip::address::from_string(debug_pc_address), 9999); // Set the destination IP and port

    //jpegLog->debugMsg("UDP server started");
    should_compress = true;

    while (true) {
        std::optional<JpegCompressor::JpegBuffer> buf = getBuffer();
        if (!buf) {
            // this should never happen
            usleep(30'000);
            continue;
        }

        //EASY_BLOCK("Send Image");
        uint8_t camId = static_cast<uint8_t>(as_integer(buf->cam));
        uint32_t size = htonl(buf->size());

        // Prepare the packet
        std::vector<uint8_t> packet;
        packet.reserve(sizeof(size) + sizeof(camId) + buf->size());
        packet.insert(packet.end(), reinterpret_cast<uint8_t*>(&size), reinterpret_cast<uint8_t*>(&size) + sizeof(size));
        packet.push_back(camId);
        packet.insert(packet.end(), buf->buffer.data(), buf->buffer.data() + buf->size());

        // Send the packet
        socket.send_to(boost::asio::buffer(packet), remote_endpoint, 0, ec);

        if (ec) {
            //jpegLog->errMsg("Error sending UDP packet: %s", ec.message().c_str());
        } else {
           //  printf("Sent %zu bytes\n", packet.size());
        }
        //EASY_END_BLOCK;
    }

    should_compress = false;
    //jpegLog->debugMsg("UDP server stopped");
}
