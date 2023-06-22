#include <stdio.h>
#include "pico/stdlib.h"
#include "Ultrasonic.hpp"
#include "hardware/uart.h"
#include <new>

int main(){

	uint led = PICO_DEFAULT_LED_PIN;
	stdio_init_all();

	gpio_init(led);
	gpio_set_dir(led, GPIO_OUT);
	int i=0;
	while(true){
		
	}
}
