unsigned int recv( char *s, unsigned int ls ) {
  asm( "IM 1" );
  asm( "SYSCALL" );
}

void send( char *s ) {
  asm( "IM 2" );
  asm( "SYSCALL" );
}

void engage( unsigned char coil ) {
  asm( "IM 3" );
  asm( "SYSCALL" );
}

unsigned char inp( void ) {
  asm( "IM 4" );
  asm( "SYSCALL" );
}

int main ( void ) {
  send( "Hello World!\n" );
	return( 0 );
}
