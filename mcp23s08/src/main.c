#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <mcp23s08.h>

#define PIN MCP23S08_PIN_GP6

int main(int argc, char **argv) {

	printf("mcp23s08_start\n");
	if (mcp23s08_start(0) != MCP23S08_OK) {
		printf("Cannot start MCP23S08\n");
		exit(EXIT_FAILURE);
	}

	printf("mcp23s08_gpio_fsel\n");
	mcp23s08_gpio_fsel(MCP23S08_PIN_GP0, MCP23S08_FSEL_OUTP);
	mcp23s08_gpio_fsel(MCP23S08_PIN_GP1, MCP23S08_FSEL_OUTP);
	mcp23s08_gpio_fsel(MCP23S08_PIN_GP2, MCP23S08_FSEL_OUTP);
	mcp23s08_gpio_fsel(MCP23S08_PIN_GP3, MCP23S08_FSEL_OUTP);
	mcp23s08_gpio_fsel(MCP23S08_PIN_GP4, MCP23S08_FSEL_OUTP);
	mcp23s08_gpio_fsel(MCP23S08_PIN_GP5, MCP23S08_FSEL_OUTP);
	mcp23s08_gpio_fsel(MCP23S08_PIN_GP6, MCP23S08_FSEL_OUTP);
	mcp23s08_gpio_fsel(MCP23S08_PIN_GP7, MCP23S08_FSEL_OUTP);

	printf("mcp23s08_gpio_clr\n");
	mcp23s08_gpio_clr(MCP23S08_PIN_GP0);
	printf("mcp23s08_gpio_set\n");
	mcp23s08_gpio_set(MCP23S08_PIN_GP1);
	printf("mcp23s08_gpio_clr\n");
	mcp23s08_gpio_clr(MCP23S08_PIN_GP2);
	printf("mcp23s08_gpio_set\n");
	mcp23s08_gpio_set(MCP23S08_PIN_GP3);
	printf("mcp23s08_gpio_clr\n");
	mcp23s08_gpio_clr(MCP23S08_PIN_GP4);
	printf("mcp23s08_gpio_set\n");
	mcp23s08_gpio_set(MCP23S08_PIN_GP5);
	printf("mcp23s08_gpio_clr\n");
	mcp23s08_gpio_clr(MCP23S08_PIN_GP6);
	printf("mcp23s08_gpio_set\n");
	mcp23s08_gpio_set(MCP23S08_PIN_GP7);

	while(1) {
		mcp23s08_gpio_clr(PIN);
		sleep(1);
		mcp23s08_gpio_set(PIN);
		sleep(1);
	}

	printf("mcp23s08_end\n");
	mcp23s08_end();

	return EXIT_SUCCESS;
}
