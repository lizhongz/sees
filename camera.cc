#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
static int stream_flag = V4L2_BUF_TYPE_VIDEO_CAPTURE;

CCamera::CCamera()
{
	strcpy(m_szDevice, "/dev/video0");
	m_nBuffNum = 4;
	m_nStreamFlag = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	m_nWidth = 640;
	m_nHeight = 480;
}

CCamera::CCamera(char* dev, int buffNum)
{
	if(dev != NULL)
		strcpy(m_szDevice, dev);
	m_nBuffNum = buffNum;
	m_nStreamFlag = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	m_nWidth = 640;
	m_nHeight = 480;
}

CCamera::~CCamera()
{

}

int CCamera::dev_open()
{
	m_nfd = open(m_szDevice, O_RDWR);
	if(m_nfd != -1)
	{
		printf("open camera successfully\n");
		return m_nfd;
	}

	for(int i = 0; i < 10; i++)
	{
		sprintf(m_szDevice, "/dev/video%d", i);
		m_nfd = open(m_szDevice, O_RDWR);
		if(m_nfd != -1)
		{
			printf("open camera successfully\n");
			return m_nfd;
		}
	}
	perror("VIDIOC_OPEN");
	exit(EXIT_FAILURE);
	return -1;
}

int CCamera::dev_close()
{
	if(m_nfd != -1)
	{
		close(m_nfd);
		return 0;
	}
	return -1;
}

int CCamera::dev_capablty()
{
	return 0;
}

int CCamera::cam_init()
{
	dev_open();
	dev_set_format();
	dev_buff_alloc();
	dev_stream_on();
	return 0;
}

int CCamera::cam_quit()
{
	dev_stream_off();
	dev_close();
	dev_buff_free();
	return 0;
}

int CCamera::cam_save_photo(char* file, int photoNum)
{
	for(int i = 0; i < photoNum; i++)
	{
		unsigned char * frame_buff = NULL;
		unsigned char * jpeg_buff = NULL;
		int len = 0;
		char filename[255] = {'\0'};
		dev_buff_dequeue(i);	
		frame_buff = (unsigned char*)m_cVideoBuff.buf[i].start;
		len = m_cVideoBuff.buf[i].length;	
		jpeg_buff = (unsigned char *)malloc(len);
		memset(jpeg_buff, 0, len);
		int size = compress_yuyv_to_jpeg(frame_buff, jpeg_buff, m_nWidth, m_nHeight, len, 80);
		sprintf(filename, "%s_%d.jpeg", file, i);
		FILE* file_fd = fopen(filename, "wb");
		fwrite(jpeg_buff, size, 1, file_fd);
		fclose(file_fd);
		dev_buff_enqueue(i);
	}
	return 0;
}

int CCamera::cam_get_buff(void** buff, int* len, int photoNum)
{
}

int CCamera::cam_reso_config(int width, int height)
{
	m_nWidth = width;
	m_nHeight = height;
	dev_set_format();
	return 0;
}

int CCamera::dev_set_format()
{
	memset(&m_cFmt, 0, sizeof(m_cFmt));
	m_cFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	m_cFmt.fmt.pix.width = m_nWidth;
	m_cFmt.fmt.pix.height = m_nHeight;
	m_cFmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

	if (ioctl(m_nfd, VIDIOC_S_FMT, &m_cFmt) == -1) 
	{
		perror("VIDIOC_S_FORMAT");
		exit(EXIT_FAILURE);
	}
	return 0;
}

int CCamera::dev_stream_on()
{
	if (ioctl(m_nfd, VIDIOC_STREAMON, &stream_flag) == -1) 
	{
		if (errno == EINVAL) 
		{
			perror("streaming i/o is not support");
		}
		else 
		{
			perror("VIDIOC_STREAMON");
		}
		exit(EXIT_FAILURE);
	}
	return 0;
}

int CCamera::dev_stream_off()
{
	if (ioctl(m_nfd, VIDIOC_STREAMOFF, &stream_flag) == -1) 
	{
		if (errno == EINVAL) 
		{
			perror("streaming i/o is not support");
		}

		else 
		{
			perror("VIDIOC_STREAMOFF");
		}

		exit(EXIT_FAILURE);
	}
	return 0;
}


int CCamera::dev_buff_alloc()
{
	int i;
	dev_buff_request();

	for (i = 0; i < m_cVideoBuff.req.count; i++) 
	{
		dev_buff_mmap(i);
		dev_buff_enqueue(i);
	}
	return 0;
}

int CCamera::dev_buff_request()
{
	memset(&m_cVideoBuff.req, 0, sizeof(m_cVideoBuff.req));
	m_cVideoBuff.req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	m_cVideoBuff.req.memory = V4L2_MEMORY_MMAP;

	m_cVideoBuff.req.count = m_nBuffNum;


	if (ioctl(m_nfd, VIDIOC_REQBUFS, &m_cVideoBuff.req) == -1) 
	{
		if (errno == EINVAL) 
		{
			perror("video capturing or mmap-streaming is not support");
		}
		else 
		{
			perror("VIDIOC_REQBUFS");
		}
		exit(EXIT_FAILURE);
	}

	if (m_cVideoBuff.req.count < m_nBuffNum) 
	{
		perror("no enough buffer");
		exit(EXIT_FAILURE);
	}

	m_cVideoBuff.buf = (Buff *)calloc(m_cVideoBuff.req.count, sizeof(*m_cVideoBuff.buf));
	assert(m_cVideoBuff.buf != NULL);
	return 0;
}

int CCamera::dev_buff_free()
{
	int i;
	for (i = 0; i < m_cVideoBuff.req.count; i++) 
	{
		munmap(m_cVideoBuff.buf[i].start, m_cVideoBuff.buf[i].length);
	}
	free(m_cVideoBuff.buf);

	return 0;
}

int CCamera::dev_buff_mmap(int index)
{
	memset(&m_cVideoBuff.query, 0, sizeof(m_cVideoBuff.query));
	m_cVideoBuff.query.type = m_cVideoBuff.req.type;
	m_cVideoBuff.query.memory = V4L2_MEMORY_MMAP;
	m_cVideoBuff.query.index = index;

	if (ioctl(m_nfd, VIDIOC_QUERYBUF, &m_cVideoBuff.query) == -1) 
	{
		perror("VIDIOC_QUERYBUF");
		exit(EXIT_FAILURE);
	}

	m_cVideoBuff.buf[index].length = m_cVideoBuff.query.length;
	m_cVideoBuff.buf[index].start = mmap(NULL,
			m_cVideoBuff.query.length,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			m_nfd,
			m_cVideoBuff.query.m.offset);

	if (m_cVideoBuff.buf[index].start == MAP_FAILED) 
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
}

int CCamera::dev_buff_enqueue(int index)
{
	memset(&m_cVideoBuff.query, 0, sizeof(m_cVideoBuff.query));
	m_cVideoBuff.query.type = m_cVideoBuff.req.type;
	m_cVideoBuff.query.memory = V4L2_MEMORY_MMAP;
	m_cVideoBuff.query.index = index;

	if (ioctl(m_nfd, VIDIOC_QBUF, &m_cVideoBuff.query) == -1) 
	{
		perror("VIDIOC_QBUF");
		exit(EXIT_FAILURE);
	}
}

int CCamera::dev_buff_dequeue(int index)
{
	memset(&m_cVideoBuff.query, 0, sizeof(m_cVideoBuff.query));
	m_cVideoBuff.query.type = m_cVideoBuff.req.type;
	m_cVideoBuff.query.memory = V4L2_MEMORY_MMAP;
	m_cVideoBuff.query.index = index;

	if (ioctl(m_nfd, VIDIOC_DQBUF, &m_cVideoBuff.query) == -1) 
	{
		perror("VIDIOC_DQBUF");
		exit(EXIT_FAILURE);
	}
}

void CCamera::init_destination(j_compress_ptr cinfo)
{
	mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;

	/* Allocate the output buffer --- it will be released when done with image */
	dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_IMAGE, OUTPUT_BUF_SIZE * sizeof(JOCTET));

	*(dest->written) = 0;

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

/******************************************************************************
 *  Description.: called whenever local jpeg buffer fills up
 *  Input Value.:
 *  Return Value:
 *******************************************************************************/
boolean CCamera::empty_output_buffer(j_compress_ptr cinfo)
{
	mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;

	memcpy(dest->outbuffer_cursor, dest->buffer, OUTPUT_BUF_SIZE);
	dest->outbuffer_cursor += OUTPUT_BUF_SIZE;
	*(dest->written) += OUTPUT_BUF_SIZE;

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

	return TRUE;
}

/******************************************************************************
 * Description.: called by jpeg_finish_compress after all data has been written.
 * Usually needs to flush buffer.
 * Input Value.:
 * Return Value:
 *******************************************************************************/
void CCamera::term_destination(j_compress_ptr cinfo)
{
	mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
	size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

	/* Write any data remaining in the buffer */
	memcpy(dest->outbuffer_cursor, dest->buffer, datacount);
	dest->outbuffer_cursor += datacount;
	*(dest->written) += datacount;
}

/******************************************************************************
 * Description.: Prepare for output to a stdio stream.
 * Input Value.: buffer is the already allocated buffer memory that will hold
 * the compressed picture. "size" is the size in bytes.
 * Return Value: -
 *******************************************************************************/
void CCamera::dest_buffer(j_compress_ptr cinfo, unsigned char *buffer, int size, int *written)
{
	mjpg_dest_ptr dest;

	if(cinfo->dest == NULL) {
		cinfo->dest = (struct jpeg_destination_mgr *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(mjpg_destination_mgr));
	}

	dest = (mjpg_dest_ptr) cinfo->dest;
	dest->pub.init_destination = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination = term_destination;
	dest->outbuffer = buffer;
	dest->outbuffer_size = size;
	dest->outbuffer_cursor = buffer;
	dest->written = written;
}

/******************************************************************************
 *  Description.: yuv2jpeg function is based on compress_yuyv_to_jpeg written by
 *  Gabriel A. Devenyi.
 *  It uses the destination manager implemented above to compress
 *  YUYV data to JPEG. Most other implementations use the
 *  "jpeg_stdio_dest" from libjpeg, which can not store compressed
 *  pictures to memory instead of a file.
 *  Input Value.: video structure from v4l2uvc.c/h, destination buffer and buffersize
 *  the buffer must be large enough, no error/size checking is done!
 *  Return Value: the buffer will contain the compressed data
 *  ******************************************************************************/
int CCamera::compress_yuyv_to_jpeg(unsigned char *frame, unsigned char *buffer, int width, int height, int size, int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];
	unsigned char *line_buffer, *yuyv;
	int z;
	static int written;

	line_buffer = (unsigned char *)calloc(width * 3, 1);
	yuyv = frame;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	/* jpeg_stdio_dest (&cinfo, file); */
	dest_buffer(&cinfo, buffer, size, &written);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);

	z = 0;
	while(cinfo.next_scanline < height) {
		int x;
		unsigned char *ptr = line_buffer;

		for(x = 0; x < width; x++) {
			int r, g, b;
			int y, u, v;

			if(!z)
				y = yuyv[0] << 8;
			else
				y = yuyv[2] << 8;
			u = yuyv[1] - 128;
			v = yuyv[3] - 128;

			r = (y + (359 * v)) >> 8;
			g = (y - (88 * u) - (183 * v)) >> 8;
			b = (y + (454 * u)) >> 8;

			*(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
			*(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
			*(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

			if(z++) {
				z = 0;
				yuyv += 4;
			}
		}

		row_pointer[0] = line_buffer;
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	free(line_buffer);

	return (written);
}
