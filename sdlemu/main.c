#include "SDL.h"
#include <common.h>
#include <stdbool.h>

static struct fbconfig fb;
static SDL_Surface *window;

static int sdl_init(void) {
	window = 0;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return fprintf(stderr, "Couldn't init SDL: %s", SDL_GetError());

	return 0;
}

static int sdl_finish(void) {
	SDL_Quit();
	window = 0;
	return 0;
}

static int sdl_setup(unsigned int width, unsigned int height, int depth) {
	int flags = 0;

	// create window
	window = SDL_SetVideoMode (width, height, depth, flags | SDL_HWSURFACE);
	if(!window)
		window = SDL_SetVideoMode (width, height, depth, flags | SDL_SWSURFACE);
	if(!window)
		return fprintf(stderr, "Couldn't open window: %s", SDL_GetError());


	// get framebuffer
	fb.width = window->w;
	fb.height = window->h;
	fb.bpp = window->format->BitsPerPixel;
	fb.bytes_per_pixel = window->format->BytesPerPixel;
	fb.pitch = window->pitch;
	fb.buf = window->pixels;

	return 0;
}

void fb_flip(void) {
	SDL_Flip (window);
}

static int keymap[0xfff] = {0};
int isKeyPressed(int code) {
	int ret = keymap[code];
	keymap[code] = 0;
	return ret;
}
int isAnyKeyPressed(void) {
	int i, found = 0;

	for(i=0; i<0xfff; i++) {
		if(keymap[i]) found = 1;
		keymap[i] = 0;
	}

	return found;
}

static int keySdlToLinux(int sdl, int* kc) {
	if(sdl==SDLK_RIGHT) *kc = KEY_RIGHT;
	else if(sdl==SDLK_LEFT) *kc = KEY_LEFT;
	else if(sdl==SDLK_UP) *kc = KEY_UP;
	else if(sdl==SDLK_DOWN) *kc = KEY_DOWN;
	else return 0;

	return 1;
}

static void setKey(int codeSDL, int val) {
	int code = 0;

	if(keySdlToLinux(codeSDL, &code))
		keymap[code] = val;
}

static double elapsedTime = 0;
int main(void) {
	struct timeval t1, t2;

	sdl_init();
	sdl_setup(300, 600, 24);
	init(&fb);

	SDL_Event event;
	while(1){
		// get start time
		gettimeofday(&t1, NULL);

		// sdl event handling
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT)
				break;

			int kc = event.key.keysym.sym;

			switch( event.type ){
                case SDL_KEYDOWN:
					setKey(kc, true);
					break;
                case SDL_KEYUP:
					setKey(kc, false);
                    break;

                default:
                    break;
            }
		}

		// call update loop
		update(elapsedTime);
		render();

		// calculate delta
		gettimeofday(&t2, NULL);
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
	}
 
	sdl_finish();   
	return 0;
}

long game_get_fps_real(void) {
	return 1000/elapsedTime;
}

long game_get_fps_limited(void) {
	return 0;
}
