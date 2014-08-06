#ifndef __APP_FB_H
#define __APP_FB_H

struct fbconfig {
	unsigned int width;
	unsigned int height;
	unsigned int bpp;
	unsigned int bytes_per_pixel;
	unsigned int pitch;
	void* buf;
};

#endif
