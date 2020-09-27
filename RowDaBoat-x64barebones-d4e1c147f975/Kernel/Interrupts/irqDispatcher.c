#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <video_driver.h>
#include <scheduler.h>

static uint64_t int_20(uint64_t rsp);
static void int_21();

uint64_t irqDispatcher(uint64_t irq, uint64_t rsp) {
	switch (irq) {
		case 0:
			return int_20(rsp);
		case 1:
			int_21();
			return rsp;	
	}
	return rsp;
}

uint64_t int_20(uint64_t rsp) {
	timer_handler();
	return scheduler(rsp);
}

void int_21(){
	keyboard_handler();
}