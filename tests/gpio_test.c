// // //////////////////////////////////////
// # 	externLED.c
// Blinks an external LED wired to P9_14.
// Wiring: P9_14 connects to the plus lead of an LED.  The negative lead of the
// 		LED goes to a 220 Ohm resistor.  The other lead of the resistor goes
// 		to ground
// 	Setup:
// 	See:
// // //////////////////////////////////////
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define	CONSUMER	"internLED.c"

int main(int argc, char **argv)
{
	int chipnumber = 2;
	unsigned int line_num = 24;	// P9_14, run: gpioinfo | grep -i -e chip -e P9_14
	unsigned int val;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int i, ret;

	chip = gpiod_chip_open_by_number(chipnumber);
	line = gpiod_chip_get_line(chip, line_num);
	ret = gpiod_line_request_output(line, CONSUMER, 0);

	/* Blink */
	val = 0;
	while(1) {
		ret = gpiod_line_set_value(line, val);
		// printf("Output %u on line #%u\n", val, line_num);
		usleep(100000);		// Number of microseconds to sleep
		val = !val;
	}
}

// g++ gpio_test.cpp -o gpio_test -lgpiodcxx && ./gpio_test