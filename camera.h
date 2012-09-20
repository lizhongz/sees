#ifndef _SSE_VIDEO_H
#define _SSE_VIDEO_H
#include <sys/types.h>
#include <stdio.h>
#include <linux/videodev2.h>
#ifdef __cplusplus
extern "C"{
#endif
#include <jpeglib.h>
#ifdef __cplusplus
}
#endif

typedef struct Buff 
{             
	void *start;
	size_t length;
}Buff;

typedef struct VideoBuffer 
{
  struct v4l2_requestbuffers req; 
  struct v4l2_buffer query;     
  Buff* buf;
}VideoBuffer;

/******** lines below are for jpeg convertion********/
#define OUTPUT_BUF_SIZE  4096

typedef struct {
	struct jpeg_destination_mgr pub; /* public fields */

	JOCTET * buffer;    /* start of buffer */

	unsigned char *outbuffer;
	int outbuffer_size;
	unsigned char *outbuffer_cursor;
	int *written;

} mjpg_destination_mgr;

typedef mjpg_destination_mgr * mjpg_dest_ptr;

//#ifdef __cplusplus
//extern "C"{
//#endif
//jpeg_std_error(jpeg_error_mgr*);
//jpeg_CreateCompress(jpeg_compress_struct*, int, unsigned int);
//jpeg_set_defaults(jpeg_compress_struct*);
//jpeg_set_quality(jpeg_compress_struct*, int, int);
//jpeg_start_compress(jpeg_compress_struct*, int);
//jpeg_write_scanlines(jpeg_compress_struct*, unsigned char**, unsigned int);
//
//#ifdef __cplusplus
//}
//#endif
/********** jpeg convertion structs end*************/


class CCamera
{
	private:
	char m_szDevice[255];
	int m_nfd;
	int m_nBuffNum;
	VideoBuffer m_cVideoBuff;	
	v4l2_format m_cFmt;
	struct v4l2_capability m_cCapblty;
	int m_nStreamFlag;
	int m_nWidth;
	int m_nHeight;

	public:
	CCamera();
	CCamera(char* dev, int buffNum = 4);
	~CCamera();
	int cam_init();
	int cam_quit();
	int cam_reso_config(int width, int height);
	int cam_save_photo(char* file, int photoNum = 1);
	int cam_get_buff(void** buff, int* len, int photoNum = 1);

	private:
	int dev_open();
	int dev_close();
	int dev_capablty();
	int dev_set_format();
	int dev_stream_on();	
	int dev_stream_off();
	int dev_buff_alloc();
	int dev_buff_request();
	int dev_buff_free();
	int dev_buff_mmap(int index);
	int dev_buff_enqueue(int index);
	int dev_buff_dequeue(int index);
	int compress_yuyv_to_jpeg(unsigned char *frame, unsigned char *buffer, int width, int height, int size, int quality);
	void dest_buffer(j_compress_ptr cinfo, unsigned char *buffer, int size, int *written);
	METHODDEF(void) term_destination(j_compress_ptr cinfo);
	METHODDEF(boolean) empty_output_buffer(j_compress_ptr cinfo);
	METHODDEF(void) init_destination(j_compress_ptr cinfo);
};

#endif
