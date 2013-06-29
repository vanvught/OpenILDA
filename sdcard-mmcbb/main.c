/*----------------------------------------------------------------------*/
/* FatFs sample project for generic microcontrollers (C)ChaN, 2012      */
/*----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include "ff.h"

FATFS Fatfs;		/* File system object */
FIL Fil;			/* File object */
BYTE Buff[128];		/* File read buffer */

extern void _start(void);

void c_irq_handler(void) {}
void c_fiq_handler(void) {}

void die (		/* Stop with dying message */
	FRESULT rc	/* FatFs return value */
)
{
	printf("Failed with rc=%u.\n", rc);
	_start();
	while(1);
}


/*-----------------------------------------------------------------------*/
/* Program Main                                                          */
/*-----------------------------------------------------------------------*/

int notmain (void)
{
	FRESULT rc;				/* Result code */
	DIR dir;				/* Directory object */
	FILINFO fno;			/* File information object */
	UINT br ;

	bcm2835_uart_begin();

	f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

#if 1
	printf("\r\nOpen an existing file (rainbowcircle.ild).\r\n");
	rc = f_open(&Fil, "rainbowcircle.ild", FA_READ);
	if (rc) die(rc);


	for (;;) {
		uint32_t i1 = bcm2835_st_read();
		rc = f_read(&Fil, Buff, 20, &br);	/* Read a chunk of file */
		uint32_t i2 = bcm2835_st_read();
		if (rc || !br) break;			/* Error or end of file */
		printf("br [%d] i2-i1 [%d]\n", br, (int)(i2-i1));
	}
	if (rc) die(rc);

	printf("\r\nClose the file.\r\n");
	rc = f_close(&Fil);
	if (rc) die(rc);
#endif
#if 0
	printf("\r\nCreate a new file (hello.txt).\r\n");
	rc = f_open(&Fil, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
	if (rc) die(rc);

	printf("\r\nWrite a text data. (Hello world!)\r\n");
	rc = f_write(&Fil, "Hello world!\r\n", 14, &bw);
	if (rc) die(rc);
	printf("%u bytes written.\r\n", bw);

	printf("\r\nClose the file.\r\n");
	rc = f_close(&Fil);
	if (rc) die(rc);
#endif
	printf("\r\nOpen root directory.\r\n");
	rc = f_opendir(&dir, "");
	if (rc) die(rc);

	char filename_buf[32];

	printf("\r\nDirectory listing...\r\n");
	for (;;) {
		fno.lfname = filename_buf;
		fno.lfsize = sizeof(filename_buf);
		rc = f_readdir(&dir, &fno);		/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
		if (fno.fattrib & AM_DIR)
			printf("   <dir>  %s\r\n", fno.fname);
		else {
			char * fn = *fno.lfname ? fno.lfname : fno.fname;
			printf("%8lu  %s\r\n", fno.fsize, (char *) fn);
		}
	}
	if (rc) die(rc);

	printf("\nTest completed.\n");

	die(0);

	return 0;

}


