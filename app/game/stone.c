#include <common.h>

// global
int stone_posx = 0, stone_posy = 0;

// private

static struct stone stone1 = {
	{
		{0,0,0},
		{1,1,1},
		{1,0,0},
	}, 3, 0, 1, 0xff00ff
};

static struct stone stone2 = {
	{
		{0,0,0,0},
		{1,1,1,1},
		{0,0,0,0},
		{0,0,0,0},
	}, 4, 0, 1, 0xff0000
};

static struct stone stone3 = {
	{
		{0,0,0},
		{1,1,1},
		{0,1,0},
	}, 3, 0, 0, 0xffff00
};

static struct stone stone4 = {
	{
		{0,1,1},
		{1,1,0},
	}, 3, 0, 0, 0x00ff00
};

static struct stone stone5 = {
	{
		{1,1,0},
		{0,1,1},
	}, 3, 0, 0, 0x00ffff
};

static struct stone stone6 = {
	{
		{1,1},
		{1,1},
	}, 2, 0, 0, 0x0000ff
};

static struct stone stone7 = {
	{
		{0,0,0},
		{1,1,1},
		{0,0,1},
	}, 3, 0, 1, 0xffffff
};
static struct stone active_stone = {{},0,0,0,0};
static struct stone* stones[NUM_STONES] = {
	&stone1,
	&stone2,
	&stone3,
	&stone4,
	&stone5,
	&stone6,
	&stone7,
};

int myRand(int low, int high) {
   srand(current_time());
   return (rand()%10) % (high - low + 1) + low;
}

// http://stackoverflow.com/questions/2509679/how-to-generate-a-random-number-from-within-a-range
static int getRandomStone(void) {
	unsigned long r, n = NUM_STONES-1+1;
	srand(current_time());

	r = n / 2;
	r = (rand() * n + r) / (RAND_MAX + 1UL);
	r = (rand() * n + r) / (RAND_MAX + 1UL);
	r = (rand() * n + r) / (RAND_MAX + 1UL);

	return r;
}

void newStone(void) {
	int stone = myRand(0,6);
	printf("stone%d\n", rand());
	active_stone = *stones[stone];
	stone_posx = 0 - active_stone.offset_x;
	stone_posy = 0 - active_stone.offset_y;
}

void drawStone(struct fbconfig* fb) {
	unsigned i,j;

	for(i=0; i<active_stone.diameter; i++) {
		for(j=0; j<active_stone.diameter; j++) {
			if(active_stone.data[i][j]) {
				fbGridSetColor(fb, MAP_POSX+stone_posx+j, MAP_POSY+stone_posy+i, active_stone.color);
			}
		}
	}
}

void copyStone2Map(void) {
	unsigned i,j;

	for(i=0; i<active_stone.diameter; i++) {
		for(j=0; j<active_stone.diameter; j++) {
			if(active_stone.data[i][j]) {
				map[j+stone_posx][i+stone_posy] = active_stone.color;
			}
		}
	}
}

static int stoneOverlaps(int offset_x, int offset_y) {
	unsigned x,y;
	for(y=0; y<active_stone.diameter; y++) {
		for(x=0; x<active_stone.diameter; x++) {

			if(
				// position x/y <0	
				// TODO check stone parts
				(stone_posx+offset_x<0 || stone_posy+offset_y<0)

				|| (
					// we have a stone part
					active_stone.data[y][x] 
					&& (
						// position is outside map (right or bottom)
						(stone_posx+x+offset_x>=MAP_WIDTH || stone_posy+y+offset_y>=MAP_HEIGHT)

						// there's another stone on the map
						|| map[stone_posx+x+offset_x][stone_posy+y+offset_y]
						
					)
				)
			)
				return 1;
		}
	}
	return 0;
}

int stoneCanDown(void) {
	return !stoneOverlaps(0,1);
}

int stoneCanLeft(void) {
	return !stoneOverlaps(-1, 0);
}

int stoneCanRight(void) {
	return !stoneOverlaps(1, 0);
}

// http://stackoverflow.com/questions/42519/how-do-you-rotate-a-two-dimensional-array
// TODO check collision after rotation
void stoneDoRotate(void) {
	int n=active_stone.diameter;
	int tmp,i,j;
	for (i=0; i<n/2; i++) {
		for (j=i; j<n-i-1; j++) {
			tmp=active_stone.data[i][j];
			active_stone.data[i][j]=active_stone.data[j][n-i-1];
			active_stone.data[j][n-i-1]=active_stone.data[n-i-1][n-j-1];
			active_stone.data[n-i-1][n-j-1]=active_stone.data[n-j-1][i];
			active_stone.data[n-j-1][i]=tmp;
		}
	}	
}
