#include "jpegcompressor.h"

#include <cstring>
#include <cstdlib>

#include <tmmintrin.h>

// #include <easy/profiler.h>

JpegCompressor::JpegCompressor() : width(JPEG_IMAGE_WIDTH), height(JPEG_IMAGE_HEIGHT) {
    jpegBuffer = static_cast<JpegBuffer*>(aligned_alloc(sizeof(__m128i), sizeof(JpegBuffer)));
}

// Iis called at the end of the compression and prepends the image size into the last 4 bytes.
void JpegCompressor::append_buffer_size_ahead(j_compress_ptr cinfo) {
    uint32_t size = JpegCompressor::JPG_BUF_SIZE - cinfo->dest->free_in_buffer;
    void* posOfSize = cinfo->dest->next_output_byte + cinfo->dest->free_in_buffer - sizeof(size);
    memcpy(posOfSize, &size, sizeof(size));
}

/*
 * See: https://software.intel.com/en-us/node/503876 YCbCr 422
 * Before:
 * Y0 Cb0  Y1 Cr0  Y2 Cb1  Y3 Cr1 Y4 Cb2 Y5 Cr2 Y6 Cb3 Y7 Cr3
 *  0   1   2   3   4   5   6   7  8   9 10  11 12  13 14  15
 *
 * After:
 * Y0 Y1 Y2 Y3 Y4 Y5  Y6  Y7 Cb0 Cb1 Cb2 Cb3 Cr0 Cr1 Cr2 Cr3
 *  0  2  4  6  8 10  12  14   1   5   9  13   3   7  11  15
 */
static const unsigned char cshuffle[]={0,2,4,6,8,10,12,14,1,5,9,13,3,7,11,15};

const __m128i JpegCompressor::shuffle=_mm_loadu_si128((__m128i*)cshuffle);

int JpegCompressor::yuv422_to_jpeg(uint8_t* data422, CamID cam, int quality, bool useC420, int subsample) {
    // EASY_FUNCTION();

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int counter;
    jpegBuffer->cam = cam;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    cinfo.image_width = width/subsample;  /* image width and height, in pixels */
    cinfo.image_height = height/subsample;
    cinfo.input_components = 3;    /* # of color components per pixel */
    cinfo.in_color_space = JCS_YCbCr;  /* colorspace of input image */
    jpeg_set_defaults(&cinfo);
    cinfo.dct_method=JDCT_IFAST;
    cinfo.raw_data_in=TRUE;

    cinfo.jpeg_color_space = JCS_YCbCr;
    if(useC420){
        cinfo.comp_info[0].h_samp_factor = 2;
        cinfo.comp_info[0].v_samp_factor = 2;
        cinfo.comp_info[1].h_samp_factor = 1;
        cinfo.comp_info[1].v_samp_factor = 1;
        cinfo.comp_info[2].h_samp_factor = 1;
        cinfo.comp_info[2].v_samp_factor = 1;
    }else{
        cinfo.comp_info[0].h_samp_factor = 2;
        cinfo.comp_info[0].v_samp_factor = 2;
        cinfo.comp_info[1].h_samp_factor = 1;
        cinfo.comp_info[1].v_samp_factor = 2;
        cinfo.comp_info[2].h_samp_factor = 1;
        cinfo.comp_info[2].v_samp_factor = 2;
    }

    cinfo.dest = (struct jpeg_destination_mgr *)(*cinfo.mem->alloc_small)((j_common_ptr) &cinfo, JPOOL_PERMANENT, sizeof(struct jpeg_destination_mgr));
    cinfo.dest->next_output_byte=jpegBuffer->buffer.data();
    cinfo.dest->free_in_buffer=JpegCompressor::JPG_BUF_SIZE;
    cinfo.dest->init_destination = [](j_compress_ptr) {};
    cinfo.dest->empty_output_buffer = [](j_compress_ptr) { return 0; }; /* should never be called */
    cinfo.dest->term_destination = append_buffer_size_ahead;

    jpeg_set_quality(&cinfo, quality, TRUE );
    jpeg_start_compress(&cinfo, TRUE);

    unsigned char ***buffer=(unsigned char***)malloc(sizeof(unsigned char**)*3);//(*cinfo.mem->alloc_small)((j_common_ptr)&cinfo,JPOOL_IMAGE,3*sizeof(JSAMPARRAY));
    int buf_width[3];
    int buf_height[3];
    for(int band=0;band<3;band++){
        buf_width[band]=cinfo.comp_info[band].width_in_blocks*DCTSIZE;
        buf_height[band]=cinfo.comp_info[band].v_samp_factor*DCTSIZE;
        buffer[band]=(unsigned char**)malloc(sizeof(unsigned char*)*buf_height[band]);//(*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo,JPOOL_IMAGE,buf_width[band],buf_height[band]);
        buffer[band][0] = static_cast<uint8_t*>(aligned_alloc(sizeof(__m128i),sizeof(unsigned char)*buf_width[band]*buf_height[band]));

        for(int i=1;i<buf_height[band];i++)
            buffer[band][i]=buffer[band][0]+buf_width[band]*i;
    }

    const int biw=buf_height[0]*width*subsample;
    unsigned char *by=buffer[0][0];
    unsigned char *bu=buffer[1][0];
    unsigned char *bv=buffer[2][0];
    __m128i *tmpLine;

    tmpLine = static_cast<__m128i*>(aligned_alloc(sizeof(__m128i), width*2/subsample));

    for(counter=0; cinfo.next_scanline < cinfo.image_height; counter++) {
        const size_t cd4i=counter*DCTSIZE*4*width*subsample;
        for(int y=0;y<biw;y+=width*subsample){
            if(useC420){
                switch(subsample){//TODO: make that faster using SSE
                case 1:
                    memcpy(tmpLine,&(data422[(y<<1)+cd4i]),width*2);
                    break;
                default:
                    int32_t *data32=(int32_t*)&(data422[(y<<1)+cd4i]);
                    for(int x=0;x<width/2/subsample;x++){
                        ((int32_t*)tmpLine)[x]=data32[x*subsample];
                    }
                    break;
                }
                for(int x=0;x<width/subsample;x+=32){
                    const size_t addr=(x<<1);
                    /* Result:
                     *  LSB Y00 Y01 Y02 Y03 Y04 Y05 Y06 Y07 Cb00 Cb01 Cb02 Cb03 Cr00 Cr01 Cr02 Cr03
                     *  LSB Y08 Y09 Y10 Y11 Y12 Y13 Y14 Y15 Cb04 Cb05 Cb06 Cb07 Cr04 Cr05 Cr06 Cr07
                     *  LSB Y16 Y17 Y18 Y19 Y20 Y21 Y22 Y23 Cb08 Cb09 Cb10 Cb11 Cr08 Cr09 Cr10 Cr11
                     *  LSB Y24 Y25 Y26 Y27 Y28 Y29 Y30 Y32 Cb12 Cb13 Cb14 Cb15 Cr12 Cr13 Cr14 Cr15
                     */
                    __m128i *in=(__m128i*)&(((char*)tmpLine)[addr]);
                    __m128i im0=_mm_shuffle_epi8(in[0],shuffle);
                    __m128i im1=_mm_shuffle_epi8(in[1],shuffle);
                    __m128i im2=_mm_shuffle_epi8(in[2],shuffle);
                    __m128i im3=_mm_shuffle_epi8(in[3],shuffle);

                    /* Result:
                     *  LSB Y00 Y01 Y02 Y03 Y04 Y05 Y06 Y07 Y08 Y09 Y10 Y11 Y12 Y13 Y14 Y15
                     *  LSB Y16 Y17 Y18 Y19 Y20 Y21 Y22 Y23 Y24 Y25 Y26 Y27 Y28 Y29 Y30 Y32
                     */
                    __m128i *yout=(__m128i*)&(by[x+y/subsample/subsample]);
                    yout[0]=_mm_unpacklo_epi64(im0,im1);
                    yout[1]=_mm_unpacklo_epi64(im2,im3);


                    /* Result:
                     *  LSB Cb00 Cb01 Cb02 Cb03 Cr00 Cr01 Cr02 Cr03 Cb04 Cb05 Cb06 Cb07 Cr04 Cr05 Cr06 Cr07
                     *  LSB Cb08 Cb09 Cb10 Cb11 Cr08 Cr09 Cr10 Cr11 Cb12 Cb13 Cb14 Cb15 Cr12 Cr13 Cr14 Cr15
                     */
                    __m128i ic0=_mm_unpackhi_epi64(im0,im1);
                    __m128i ic1=_mm_unpackhi_epi64(im2,im3);

                    /* Result: _MM_SHUFFLE is MSB to LSB
                     *  0 Cb00 Cb01 Cb02 Cb03
                     *  1 Cr00 Cr01 Cr02 Cr03
                     *  2 Cb04 Cb05 Cb06 Cb07
                     *  3 Cr04 Cr05 Cr06 Cr07
                     *
                     *  LSB Cb00 Cb01 Cb02 Cb03 Cb04 Cb05 Cb06 Cb07 Cr00 Cr01 Cr02 Cr03 Cr04 Cr05 Cr06 Cr07
                     *
                     *  3 Cb08 Cb09 Cb10 Cb11
                     *  2 Cr08 Cr09 Cr10 Cr11
                     *  1 Cb12 Cb13 Cb14 Cb15
                     *  0 Cr12 Cr13 Cr14 Cr15
                     *
                     *  LSB Cb08 Cb09 Cb10 Cb11 Cb12 Cb13 Cb14 Cb15 Cr08 Cr09 Cr10 Cr11 Cr12 Cr13 Cr14 Cr15
                     */
                    __m128i is0=_mm_shuffle_epi32(ic0,_MM_SHUFFLE(3,1,2,0));
                    __m128i is1=_mm_shuffle_epi32(ic1,_MM_SHUFFLE(3,1,2,0));

                    /* Result: Cb00 Cb01 Cb02 Cb03 Cb04 Cb05 Cb06 Cb07 Cb08 Cb09 Cb10 Cb11 Cb12 Cb13 Cb14 Cb15 */
                    __m128i *uout=(__m128i*)&(bu[(x>>1)+(y>>2)/subsample/subsample]);
                    uout[0]=_mm_unpacklo_epi64(is0,is1);

                    /* Result: Cr00 Cr01 Cr02 Cr03 Cr04 Cr05 Cr06 Cr07 Cr08 Cr09 Cr10 Cr11 Cr12 Cr13 Cr14 Cr15 */
                    __m128i *vout=(__m128i*)&(bv[(x>>1)+(y>>2)/subsample/subsample]);
                    vout[0]=_mm_unpackhi_epi64(is0,is1);
                }
                y+=width*subsample;

                switch(subsample){
                case 1:
                    memcpy(tmpLine,&(data422[(y<<1)+cd4i]),width*2);
                    break;
                default:
                    int32_t *data32=(int32_t*)&(data422[(y<<1)+cd4i]);
                    for(int x=0;x<width/2/subsample;x++){
                        ((int32_t*)tmpLine)[x]=data32[x*subsample];
                    }
                    break;
                }

                for(int x=0;x<width/subsample;x+=32){
                    const size_t addr=(x<<1);
                    __m128i *in=(__m128i*)&(((char*)tmpLine)[addr]);
                    __m128i im0=_mm_shuffle_epi8(in[0],shuffle);
                    __m128i im1=_mm_shuffle_epi8(in[1],shuffle);
                    __m128i im2=_mm_shuffle_epi8(in[2],shuffle);
                    __m128i im3=_mm_shuffle_epi8(in[3],shuffle);

                    __m128i *yout=(__m128i*)&(by[x+y/subsample/subsample]);
                    yout[0]=_mm_unpacklo_epi64(im0,im1);
                    yout[1]=_mm_unpacklo_epi64(im2,im3);
                }
            }else{//TODO: implement yuv422 for subsampled images
                for(int x=0;x<width;x+=32){
                    const size_t addr=(x<<1)+(y<<1)+cd4i;
                    __m128i *in=(__m128i*)&(data422[addr]);
                    __m128i im0=_mm_shuffle_epi8(in[0],shuffle);
                    __m128i im1=_mm_shuffle_epi8(in[1],shuffle);
                    __m128i im2=_mm_shuffle_epi8(in[2],shuffle);
                    __m128i im3=_mm_shuffle_epi8(in[3],shuffle);

                    __m128i *yout=(__m128i*)&(by[x+y]);
                    yout[0]=_mm_unpacklo_epi64(im0,im1);
                    yout[1]=_mm_unpacklo_epi64(im2,im3);

                    __m128i ic0=_mm_unpackhi_epi64(im0,im1);
                    __m128i ic1=_mm_unpackhi_epi64(im2,im3);

                    __m128i is0=_mm_shuffle_epi32(ic0,_MM_SHUFFLE(3,1,2,0));
                    __m128i is1=_mm_shuffle_epi32(ic1,_MM_SHUFFLE(3,1,2,0));

                    __m128i *uout=(__m128i*)&(bu[(x>>1)+(y>>1)]);
                    uout[0]=_mm_unpacklo_epi64(is0,is1);

                    __m128i *vout=(__m128i*)&(bv[(x>>1)+(y>>1)]);
                    vout[0]=_mm_unpackhi_epi64(is0,is1);
                }
            }
        }
        jpeg_write_raw_data(&cinfo, buffer,  cinfo.max_v_samp_factor*DCTSIZE);
    }
    free(tmpLine);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    for(int band=0;band<3;band++){
        free(buffer[band][0]);
        free(buffer[band]);
    }
    free(buffer);
    return 0;
}
