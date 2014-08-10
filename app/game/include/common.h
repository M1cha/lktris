#ifndef __APP_COMMON_H
#define __APP_COMMON_H

#include <main.h>
#include <game.h>
#include <fb.h>
#include <stone.h>

#include <string.h>
#include <stdlib.h>

#if LK
#include <debug.h>
#include <dev/keys.h>
#include <platform.h>
#define printf _dprintf
#else
#include <stdio.h>
#include <linux/input.h>
#include <sys/time.h>
static inline time_t current_time(void) {return time(NULL);}
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif

#ifndef RAND_MAX
#define RAND_MAX INT_MAX
#endif

#endif
