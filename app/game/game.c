#include <common.h>

static struct fbconfig *fb = NULL;

void setFramebuffer(struct fbconfig *config) {
	fb = config;
}

void update(double delta) {
	static double time = 0;
	time+=delta;

	if(time>1000) {
		static int color = 0xff;
		if(color==0xff) color=0x33;
		else if(color==0x33) color=0xff;

		memset(fb->buf, color, fb->width * fb->bytes_per_pixel * fb->height);
		fb_flip();

		time = 0;
	}
}
