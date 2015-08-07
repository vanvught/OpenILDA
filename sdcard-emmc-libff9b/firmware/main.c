#include <stdio.h>
#include <stdlib.h>

#include "hardware.h"
#include "ff.h"

FATFS Fatfs;		/* File system object */
FIL Fil;			/* File object */
BYTE Buff[128];		/* File read buffer */

void __attribute__((interrupt("IRQ"))) c_irq_handler(void) {}
void __attribute__((interrupt("FIQ"))) c_fiq_handler(void) {}


void die(FRESULT rc) {
	printf("Failed with rc=%d.\n", (int)rc);
	while (1)
		;
}

int notmain (void)
{
	FRESULT rc;				/* Result code */
	// DIR dir;				/* Directory object */
	// FILINFO fno;			/* File information object */
	// UINT br;

	hardware_init();

    printf("Compiled on %s at %s\n", __DATE__, __TIME__);
	printf("SD Card power state: %ld\n", bcm2835_vc_get_power_state(BCM2835_VC_POWER_ID_SDCARD));

	f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

#if 1
	printf("\nOpen an existing file (params.txt).\n");
	rc = f_open(&Fil, "params.txt", FA_READ);
	if (rc)
		die(rc);

	printf("\nType the file content.\n");
	int i = 1;
	for (;;) {
		if (f_gets((TCHAR *)Buff, sizeof Buff, &Fil) == NULL)
			break; /* Error or end of file */
		printf("%d\t:%s\n", i, Buff);
		i++;
	}
	if (rc)
		die(rc);

	printf("\nClose the file.\n");
	rc = f_close(&Fil);
	if (rc)
		die(rc);
#endif

#if 0
	int i;
	printf("\nOpen an existing file (devices.txt).\n");
	rc = f_open(&Fil, "devices.txt", FA_READ);
	if (rc)
		die(rc);

	printf("\nType the file content.\n");
	for (;;) {
		rc = f_read(&Fil, Buff, sizeof Buff, &br); /* Read a chunk of file */
		if (rc || !br)
			break; /* Error or end of file */
		for (i = 0; i < br; i++) /* Type the data */
			putchar(Buff[i]);
	}
	if (rc)
		die(rc);

	printf("\nClose the file.\n");
	rc = f_close(&Fil);
	if (rc)
		die(rc);
#endif

#if 0
	UINT bw;
	printf("\nCreate a new file (hello.txt).\n");
	rc = f_open(&Fil, "hello.txt", FA_WRITE | FA_CREATE_ALWAYS);
	if (rc) die(rc);

	printf("\nWrite a text data. (Hello world!)\n");
	rc = f_write(&Fil, "Hello world!\n", 14, &bw);
	if (rc) die(rc);
	printf("%d bytes written.\n", bw);

	printf("\nClose the file.\n");
	rc = f_close(&Fil);
	if (rc) die(rc);
#endif

#if 0
	printf("\nOpen root directory.\n");
	rc = f_opendir(&dir, "");
	if (rc) die(rc);

	char filename_buf[32];

	printf("\nDirectory listing...\n");
	for (;;) {
		fno.lfname = filename_buf;
		fno.lfsize = sizeof(filename_buf);
		rc = f_readdir(&dir, &fno);		/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
		if (fno.fattrib & AM_DIR)
			printf("   <dir>  %s\n", fno.fname);
		else {
			char * fn = *fno.lfname ? fno.lfname : fno.fname;
			printf("%8lu  %s\n", fno.fsize, (char *) fn);
		}
	}
	if (rc) die(rc);
#endif

	printf("SD Card power state: %ld\n", bcm2835_vc_get_power_state(BCM2835_VC_POWER_ID_SDCARD));
	printf("\nTest completed.\n");

	return 0;
}
