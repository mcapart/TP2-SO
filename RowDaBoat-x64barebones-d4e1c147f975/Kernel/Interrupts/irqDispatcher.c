#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <video_driver.h>
#include <scheduler.h>

static uint64_t int_20(uint64_t rsp);
static void int_21();

uint64_t irqDispatcher(uint64_t irq, uint64_t rsp) {
	if(irq == 0){
		timer_handler();
		return scheduler(rsp);
	}
	if(irq == 1){
		int_21();
		return rsp;	
	}
	/*
	switch (irq) {
		case 0:
			timer_handler();
			return scheduler(rsp);
		case 1:
			int_21();
			return rsp;	
	}*/
	return rsp;
}

uint64_t int_20(uint64_t rsp) {
	timer_handler();
	uint64_t sp = scheduler(rsp);
	return sp;
}

void int_21(){
	keyboard_handler();
}