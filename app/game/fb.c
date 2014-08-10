#include <common.h>
#include <stdbool.h>

//#define INDEX_TO_COORD
#define COORD_TO_INDEX(x,y,width) (y*width + x)

/*
 * ===================================================================================
 * COMMON
 * ===================================================================================
 */

static void fbSetPixel(struct fbconfig* fb, unsigned index, unsigned color) {
	short r,g,b;
	char* buf = (char*) fb->buf;
	unsigned first = index * fb->bytes_per_pixel;

	if(index>=fb->width*fb->height)
		return;

	r = (color & 0xFF0000) >> 16;
	g = (color & 0x00FF00) >> 8;
	b = (color & 0x0000FF) >> 0;

#ifdef PROJECT_MSM8960
	buf[first] = r;
	buf[first+1] = g;
	buf[first+2] = b;
#else
	// our framebuffers use BGR
	buf[first] = b;
	buf[first+1] = g;
	buf[first+2] = r;
#endif
}

void fbSetPixelXY(struct fbconfig* fb, unsigned x, unsigned  y, unsigned color) {
	unsigned width = fb->width;
	if(fb->landscape) {
		unsigned tmp = x;
		width = fb->height;

		x = fb->height-1 - y;
		y = tmp;
	}
	fbSetPixel(fb, COORD_TO_INDEX(x,y, width), color);
}

void fbFill(struct fbconfig* fb, unsigned color) {
	unsigned i;

	for(i=0; i<fb->width*fb->height; i++)
		fbSetPixel(fb, i, color);
}

void fbFillRect(struct fbconfig* fb, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned color) {
	unsigned x, y;

	for(x=x1; x<x2; x++) {
		for(y=y1; y<y2; y++) {
			fbSetPixelXY(fb, x, y, color);
		}
	}
}

void fbSetLandscape(struct fbconfig* fb, int enabled) {
	unsigned tmp = fb->width;
	fb->width = fb->height;
	fb->height = tmp;
	fb->landscape = !fb->landscape;
}

/*
 * ===================================================================================
 * GRID
 * ===================================================================================
 */

void fbGridSetSize(struct fbconfig* fb, int width, int height) {
	fb->grid_phys_w = fb->width/width;
	fb->grid_phys_h = fb->height/height;
	fb->grid_w = width;
	fb->grid_h = height;
}

void fbGridSetColor(struct fbconfig* fb, int x, int y, int color) {
	int x_real = x*fb->grid_phys_w;
	int y_real = y*fb->grid_phys_h;


	fbFillRect(fb, x_real, y_real, x_real+fb->grid_phys_w, y_real+fb->grid_phys_h, color);
}
