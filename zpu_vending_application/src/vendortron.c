#include <string.h>
#include "ezpu.h"

int main ( void ) {
  int8_t cmd[ 256 ];
  uint8_t ret, evt = 0;

  while( 1 ) {
    ret = recv( cmd );
    if( ret ) {
      cmd[ ret ] = 0x0A;
      send( cmd, ret + 1 );
    }
    ret = inp();
    if( ret != evt ) {
      evt = ret;
      if( evt & 0x10 ) engage( evt & 0x0F );
      /*strcpy( cmd, "event: 0x00\n" );
      cmd[ 9 ] = p_hex[ evt >> 4 ];
      cmd[ 10 ] = p_hex[ evt & 0x0F ];
      send( cmd, 12 );
      */
    }
    
  }
  //print( "Hello World!\n" );
	return( 0 );
}
