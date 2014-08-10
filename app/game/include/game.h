#ifndef __APP_GAME_H
#define __APP_GAME_H

#include <common.h>

// UI settings
#define COLOR_BORDER 0x4f2606
#define GRID_WIDTH 12
#define GRID_HEIGHT 24
#define MAP_WIDTH GRID_WIDTH-2
#define MAP_HEIGHT GRID_HEIGHT-2
#define MAP_POSX 1
#define MAP_POSY 1

typedef unsigned map_data[MAP_WIDTH][MAP_HEIGHT];
extern map_data map;

void init(struct fbconfig *config);
void update(double delta);
void render(void);
int isKeyPressed(int code);

#endif
