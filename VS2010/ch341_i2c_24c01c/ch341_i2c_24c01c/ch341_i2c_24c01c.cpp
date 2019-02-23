//
// ch341_i2c_24c01c.cpp : simple example how to write and read I2C EEPROM 24C01C
//

#include "stdafx.h"

#define HPCH_24C01_BYTES 128

// string to write to 24C01C
static const char *TEST_STR = "Hello!";
// address to write TEST_STR into EEPROM
static const int   TEST_ADDR = 5;
// buffer to dump whole EEPROM contents
static UCHAR buffer[HPCH_24C01_BYTES] = {0};
// pattern to clean EEPROM
static const UCHAR CLEAN_CHAR = 0xFF;

// Hex/ASCII dump of memory
void HpCh_DumpBuf(BYTE *buf, int n){
	const int VALUES_PER_LINE = 16;

	int i=0;
	printf("Dump of buffer at 0x%p,  bytes %u\n",buf,n);
	for(i=0;i<n;i++){
		// if it is new line - show address
		if ( i % VALUES_PER_LINE == 0){
			printf("0x%04x",i);
		}
		printf(" %02x",buf[i]);
		// if it is last byte on line dump also ASCII values where possible...
		if ( (i % VALUES_PER_LINE) == VALUES_PER_LINE - 1 || i == n-1 ){
			int start =  i / VALUES_PER_LINE * VALUES_PER_LINE;
			int end   = min( start + VALUES_PER_LINE,n);
			int j=0;
			// XXX: puts(3) always append \n
			putc(' ',stdout);
			for(j=start;j<end;j++){
				BYTE b = buf[j];
				if (b>=32 && b<127){
					putc(b,stdout);
				} else {
					putc('.',stdout);
				}
			}
			puts(""); // new line
		}
	}
	printf("\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	int    i    = 0;
	BOOL   ret  = EXIT_FAILURE;
	HANDLE h341 = NULL;
	ULONG iIndex = 0; // first device
	
	if (sizeof(buffer)!=HPCH_24C01_BYTES){
		fprintf(stderr,"Internal error - sizeof(buffer) %u != HPCH_24C01_BYTES %u",sizeof(buffer),HPCH_24C01_BYTES);
		goto exit0;
	}

	printf("CH341 library version: %lu\n", CH341GetVersion());

	printf("Opening device# %lu\r\n", iIndex);
	h341 = CH341OpenDevice(iIndex);
	if (h341 == NULL){
		fprintf(stderr, "CH341OpenDevice(iDevIndex=%lu) failed\r\n", iIndex);
		goto exit0;
	}

	// NOTE: must be called *after* CH341OpenDevice()
	printf("CH341 driver  version: %lu\n", CH341GetDrvVersion());

	printf("Storing string '%s' (including '\\0') at EEPROM address 0x%x...\n",TEST_STR,TEST_ADDR);
	// NOTE: ID_24C01 will work with 24x01B/B/C only, but NOT with 24C01 (without suffix)!
	// see README.md for more info
	if (!CH341WriteEEPROM(iIndex,ID_24C01,TEST_ADDR,strlen(TEST_STR)+1,(PUCHAR)TEST_STR)){
		fprintf(stderr,"CH341WriteEEPROM() failed\n");
		goto exit1;
	}
	// wait 2ms after write (+ USB delay) - worst case (data sheet maximum is 1.5ms)
	if (!CH341SetDelaymS(iIndex,2)){
		fprintf(stderr,"CH341SetDelaymS() failed\n");
		goto exit1;
	}
	printf("Fetching whole memory content...\n");
	if (!CH341ReadEEPROM(iIndex, ID_24C01, 0, sizeof(buffer), buffer)){
		fprintf(stderr,"CH341ReadEEPROM() failed\n");
		goto exit1;
	}

	printf("Dumping EEPROM contents...\n");
	HpCh_DumpBuf(buffer, sizeof(buffer));

	// integrity check - for TEST_STR - we use memcmp(3) to test also terminating '\0' character
	if ( memcmp(buffer+TEST_ADDR, TEST_STR,strlen(TEST_STR)+1)!=0 ){
		fprintf(stderr,"Read back of '%s' failed - please see dump\n",TEST_STR);
		goto exit1;
	}

	printf("Cleaning EEPROM contents with 0x%02x...\n",CLEAN_CHAR);
	memset(buffer,CLEAN_CHAR,sizeof(buffer));
	if (!CH341WriteEEPROM(iIndex,ID_24C01,0,sizeof(buffer),buffer)){
		fprintf(stderr,"CH341WriteEEPROM() failed\n");
		goto exit1;
	}
	// wait 2ms after write (+ USB delay) - worst case (data sheet maximum is 1.5ms)
	if (!CH341SetDelaymS(iIndex,2)){
		fprintf(stderr,"CH341SetDelaymS() failed\n");
		goto exit1;
	}
	printf("Fetching whole memory content...\n");
	if (!CH341ReadEEPROM(iIndex, ID_24C01, 0, sizeof(buffer), buffer)){
		fprintf(stderr,"CH341ReadEEPROM() failed\n");
		goto exit1;
	}

	printf("Dumping EEPROM contents...\n");
	HpCh_DumpBuf(buffer, sizeof(buffer));

	for(i=0; i<sizeof(buffer);i++){
		if (buffer[i]!=CLEAN_CHAR){
			fprintf(stderr,"Buffer check failed at address=0x%x value 0x%02x != 0x%02x\n",i,buffer[i],CLEAN_CHAR);
			goto exit1;
		}
	}

	ret = EXIT_SUCCESS;
exit1:
	CH341CloseDevice(iIndex);
exit0:
	return ret;
}

