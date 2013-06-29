#include <stdio.h>
#include <stdlib.h>

#include <mcp7941x.h>

int main(int argc, char **argv) {

	struct rtc_time tm;

	printf("rtc_start\n");
	if (mcp7941x_start(0) != MCP7941X_OK) {
		printf("Cannot start I2C_RTC\n");
		exit(EXIT_FAILURE);
	}

	mcp7941x_get_date_time(&tm);

	printf("secs=%d, mins=%d, hours=%d, mday=%d, mon=%d, year=%d, wday=%d\n", tm.tm_sec, tm.tm_min, tm.tm_hour, tm.tm_mday, tm.tm_mon, tm.tm_year, tm.tm_wday);

	printf("rtc_end\n");
	mcp7941x_end();

	return EXIT_SUCCESS;
}
