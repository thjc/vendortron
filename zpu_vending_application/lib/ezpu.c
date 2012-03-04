#include <string.h>

typedef unsigned  char  uint8_t;
typedef           char   int8_t;
typedef unsigned short uint16_t;
typedef          short  int16_t;
typedef unsigned  char     bool;
// must allocate 256 bytes
const uint8_t p_hex[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

uint8_t recv( int8_t *s ) {
  asm( "IM 1" );
  asm( "SYSCALL" );
}

void send( int8_t *s, uint8_t len ) {
  asm( "IM 2" );
  asm( "SYSCALL" );
}

void engage( uint8_t coil ) {
  asm( "IM 3" );
  asm( "SYSCALL" );
}

uint8_t inp( void ) {
  asm( "IM 4" );
  asm( "SYSCALL" );
}

void config( int8_t* value, int8_t* parameter, int8_t* fallback ) {
  asm( "IM 5" );
  asm( "SYSCALL" );
}

bool token( uint8_t tok[] ) {
  asm( "IM 6" );
  asm( "SYSCALL" );
}

void print( int8_t *s ) {
  send( s, strlen( s ) );
}

bool retrieve( uint8_t *p_data ) {
  asm( "IM 7" );
  asm( "SYSCALL" );
}

void store( uint8_t *p_data, uint16_t len ) {
  asm( "IM 8" );
  asm( "SYSCALL" );
}

uint16_t write( uint8_t address, uint8_t *p_data, uint16_t len ) {
  asm( "IM 9" );
  asm( "SYSCALL" );
}

uint16_t read( uint8_t *address, uint8_t *p_data, uint16_t len ) {
  asm( "IM 10" );
  asm( "SYSCALL" );
}

uint16_t uart_write( uint8_t port, uint8_t *p_data, uint8_t len ) {
  asm( "IM 11" );
  asm( "SYSCALL" );
}

uint16_t uart_read( uint8_t port, uint8_t *p_data, uint8_t len ) {
  asm( "IM 12" );
  asm( "SYSCALL" );
}
