#include <time.h>
#include <video_driver.h>

static unsigned long ticks = 0;

void timer_handler() {
	int color[3]= {255,255,255};
	ticks++;
	//writeWord("timer", 1.5, color);
	//newLine();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
