#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "zpu.h"
#include "zpu_memory.h"
#include "zpu_io.h"
#include "zpu_load.h"

extern uint32_t pc;

static uint8_t ret[ 4 ];
static uint8_t rom[ ROM_SIZE ];
static uint8_t ram[ RAM_SIZE ];
static uint8_t stk[ STK_SIZE ];

void memoryInitialize() {
  memset( rom, 0, sizeof( rom ) );
  memset( ram, 0, sizeof( ram ) );
  memset( stk, 0, sizeof( stk ) );
}

void *translate( uint32_t address, bool write ) {
    if( address < 4 ) {
      //printf( "RET: %i\n", address );
      return( ( void* )&ret[ address ] );
    } else if( address < ROM_SIZE ) {
//      printf( "ROM: %i\n", address & ROM_MASK );
      if( write ) {
        printf( "Memory access violation: ROM write\n" );
        exit( 1 );
      }
      return( ( void* )&rom[ address & ROM_MASK ] );
    } else if( address >= RAM_BASE && address < RAM_TOP ) {
//      printf( "RAM: %i\n", address & RAM_MASK );
      return( ( void* )&ram[ address & RAM_MASK ] );
    } else if( address >= STK_BASE && address < STK_TOP ) {
//      printf( "STK: %i\n", address & STK_MASK );
      return( ( void* )&stk[ address & STK_MASK ] );
    } else {
      printf( "Memory access violation: Out of range\n" );
      exit( 1 );
    }
}

uint32_t memoryReadLong( uint32_t address ) {
  // UART
  if (address == 0x80000024) {
  	return (0x100);
  } else if (address == 0x080A000C) {
  	return (0x100);
  }
  if( ( address & 0x3 ) != 0 ) {
    printf( "Read LONG not aligned: PC=%08x, addr=%08x\n", pc, address );
    //exit( 1 );
  }
  return( *( uint32_t* )translate( address, false ) );
}

void memoryWriteLong( uint32_t address, uint32_t value ) {
  // UART
  if( address == 0x80000024 ) {
	  printf( "%c", ( char )value );
	  return;
  } else if( address == 0x080A000C ) {
  	printf( "%c", ( char )value );
  	return;
  }
  if( ( address & 0x3 ) != 0 ) {
    printf( "Write LONG not aligned\n" );
    exit( 1 );
  }
  *( uint32_t* )translate( address, true ) = value;
}

uint16_t memoryReadWord( uint32_t address ) {  
  if( ( address & 0x1 ) != 0 ) {
    printf( "Read WORD not aligned\n" );
    exit( 1 );
  }
  return( *( uint16_t* )translate( address ^ 2, false ) );
}

void memoryWriteWord( uint32_t address, uint16_t value ) {
  if( ( address & 0x1 ) != 0 ) {
    printf( "Write WORD not aligned\n" );
    exit( 1 );
  }
  *( uint16_t* )translate( address ^ 2, true ) = value;
}

uint8_t memoryReadByte( uint32_t address ) {
  return( *( uint8_t* )translate( address ^ 3, false ) );
}

void memoryWriteByte( uint32_t address, uint8_t value ) {
  *( uint8_t* )translate( address ^ 3, true ) = value;
}

void memoryWriteROM( uint32_t address, uint8_t value ) {
  *( uint8_t* )translate( address ^ 3, false ) = value;
}
