#include <common.h>

// global
map_data map = {{}};

// private
static struct fbconfig *fb = NULL;
static mutex_t map_lock;

static void fallDown(int index) {
	unsigned x,y;

	for(y=index; y>0; y--) {
		if(y==0) {
			for(x=0; x<MAP_WIDTH; x++)
				map[x][y] = 0;
		}
		else {
			for(x=0; x<MAP_WIDTH; x++)
				map[x][y] = map[x][y-1];
		}
	}
}

static void tryClearMap(void) {
	unsigned i,j;

	for(i=0; i<MAP_HEIGHT; i++) {
		int num=0;
		for(j=0; j<MAP_WIDTH; j++) {
			if(map[j][i]) num++;
			else break;
		}

		if(num==MAP_WIDTH) {
			fallDown(i);
		}
	}
}

void init(struct fbconfig *config) {
	fb = config;
#if 0
	fbSetLandscape(fb, true);
#endif
	fbGridSetSize(fb, GRID_WIDTH, GRID_HEIGHT);

	newStone();
	mutex_init(&map_lock);
	font_init();
}

void update(double delta) {
	static unsigned time = 0, mod, c;
	static const unsigned time_stone_drop = 500; // 70-1000
	time+=(unsigned)delta;

	// input
	if(isKeyPressed(KEY_RIGHT) && stoneCanRight())
		stone_posx++;
	if(isKeyPressed(KEY_LEFT) && stoneCanLeft())
		stone_posx--;
	if(isKeyPressed(KEY_DOWN) && stoneCanDown())
		stone_posy++;
	if(isKeyPressed(KEY_UP))
		stoneDoRotate();

	// auto fall
	mod = time % time_stone_drop;
	c = time / time_stone_drop;
	if(c>0) {
		time = mod;
		mutex_acquire(&map_lock);
		if(!stoneCanDown()) {
			// bottom reached
			// TODO handel c>1

			copyStone2Map();
			tryClearMap();

			newStone();
		}
		else stone_posy+=c;

		if(c>1) printf("WARN: c==%d\n", c);
		mutex_release(&map_lock);
	}

}

void render(void) {
	int i,j;

	// clear
	memset(fb->buf, 0x0, fb->width*fb->height*fb->bpp/8);

	// border
	for(i=0; i<fb->grid_w; i++) {
		fbGridSetColor(fb, i, fb->grid_h-1, COLOR_BORDER);
	}
	for(i=0; i<fb->grid_h; i++) {
		fbGridSetColor(fb, 0, i, COLOR_BORDER);
		fbGridSetColor(fb, fb->grid_w-1, i, COLOR_BORDER);
	}

	mutex_acquire(&map_lock);

	// map
	for(i=0; i<MAP_HEIGHT; i++) {
		for(j=0; j<MAP_WIDTH; j++) {
			if(map[j][i]) {
				fbGridSetColor(fb, j+MAP_POSX, i+MAP_POSY, map[j][i]);
			}
		}
	}

	// active stone
	drawStone(fb);

	mutex_release(&map_lock);

	//font_printf(fb, 0, 0, "Time: %us", current_time()/1000);
	font_printf(fb, 0, 40, "FPS: %ld/%ld", game_get_fps_real(), game_get_fps_limited());

	// flip
	fb_flip();
}
