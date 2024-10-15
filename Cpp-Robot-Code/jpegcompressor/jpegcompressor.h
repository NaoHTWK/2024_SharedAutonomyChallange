#ifndef JPEGCOMPRESSOR_H
#define JPEGCOMPRESSOR_H

#include <array>
#include <cstdint>
#include <cstdio>

#include <emmintrin.h>
#include <jpeglib.h>

#include <cammsg.h>
#include <jpegcompressorconfig.h>

class JpegCompressor {
public:
    static constexpr size_t JPG_BUF_SIZE = JPEG_IMAGE_WIDTH * JPEG_IMAGE_HEIGHT * 2 + /* size */ sizeof(uint32_t);
    struct JpegBuffer {
        alignas(16) std::array<uint8_t, JPG_BUF_SIZE> buffer;
        CamID cam;

        size_t size() {
            return *reinterpret_cast<uint32_t*>(buffer.data() + JPG_BUF_SIZE - sizeof(uint32_t));
       }
    };

    JpegCompressor();
    ~JpegCompressor() = default;
    JpegCompressor(const JpegCompressor&) = delete;
    JpegCompressor(JpegCompressor&&) = delete;
    JpegCompressor& operator=(const JpegCompressor&) = delete;
    JpegCompressor& operator=(JpegCompressor&&) = delete;

    int yuv422_to_jpeg(uint8_t *data422, CamID cam, int quality, bool useC420, int subsample);

    JpegBuffer& getJpeg() {
        return *jpegBuffer;
    }

private:
    JpegBuffer* jpegBuffer;
    static const __m128i shuffle;

    static void append_buffer_size_ahead(j_compress_ptr cinfo);

    const int width;
    const int height;

};

#endif  // JPEGCOMPRESSOR_H
