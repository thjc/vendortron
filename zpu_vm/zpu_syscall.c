#include <stdio.h>
//#include <conio.h>

#include "zpu_syscall.h"
#include "zpu_memory.h"

void sysinitialize() {
}

void syscall(uint32_t sp) {

//  int returnAdd    = memoryReadLong( sp +  0 );
//  int errNoAdd     = memoryReadLong( sp +  4 );
  int sysCallId    = memoryReadLong( sp +  4 );
//  int fileNo       = memoryReadLong( sp + 12 );
//  int charIndex    = memoryReadLong( sp + 16 );
//  int stringLength = memoryReadLong( sp + 20 );
  printf( "SYSCALL (%i)\n", sysCallId );
/*  
  switch( sysCallId ) {
    case SYS_WRITE:
      for( int i = 0; i < stringLength; i++ ) {
        putch( memoryReadByte( charIndex++ ) );
      }
      // Return value via R0 (AKA memory address 0)
      memoryWriteLong( 0, stringLength );
      break;
    case SYS_READ:
      for( int i = 0; i < stringLength; i++ ) {
        memoryWriteByte( charIndex++, getch() );
      }
      // Return value via R0 (AKA memory address 0)
      memoryWriteLong( 0, stringLength );
      break;
  }
*/  
}
