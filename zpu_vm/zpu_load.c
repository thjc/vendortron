#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "zpu_load.h"
#include "zpu_memory.h"

void zpu_load() {
  char* fileName = "test.bin";
	FILE* f;
	int bytesRead;
	int address;
	uint8_t inByte;
	uint32_t data, ldata;

	f = fopen(fileName, "r");
	if (f == 0)
	{
		printf("Failed to open %s\n", fileName);
		perror("");
		exit(0);
	}


	for (address = 0; address < ROM_SIZE; address++)
	{
		bytesRead = fread(&inByte, 1, 1, f);
		if (ferror(f))
		{
			printf("Error reading image from %s\n", fileName);
			perror("");
			exit(1);
		}
		if (feof(f))
		{
			break;
		}
		memoryWriteROM(address, inByte);
	}
	data = memoryReadLong( 4 );
	ldata = memoryReadLong( 8 );
	for( address = 0; address < ldata; address++ ) {
	  memoryWriteByte( address + RAM_BASE, memoryReadByte( address + data ) );
	}
	//printf("Loaded %d bytes from RAM image from %s\n", address, fileName);
	fclose(f);
}

