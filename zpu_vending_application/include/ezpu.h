// Library for interfacing a ZPU program with it's vending machine host

typedef unsigned  char  uint8_t;
typedef           char   int8_t;
typedef unsigned short uint16_t;
typedef          short  int16_t;
typedef unsigned   int uint32_t;
typedef            int  int32_t;
typedef unsigned  char     bool;

#define true 1
#define false 0

extern const uint8_t p_hex[ 16 ];

// Receives a packet from communications interface
// Stores the result in buffer pointed to by *p_s
// Packets are always recieved complete
// Returns number of bytes in packet or 0 if none available
// Caller must allocate a minimum of 256 bytes for *p_s
extern uint8_t  recv( int8_t *p_s );

// Sends data to the communications interface
// Data may be sent in fragments, the packet is not delivered until terminated with \x0A
// Data is contained in buffer pointed to by *p_s
// The number of bytes to push is specified by len
// This is a blocking call and will return once data has been received by other end
extern void     send( int8_t *p_s, uint8_t len );

// Engages a coil in the vending machine, presenting a product in the retrieval slot
// The coil is specified by coil and may range from 0-9
// This is a blocking call and will return when the operation is complete
extern void     engage( uint8_t coil );

// Returns the latest event from the vending machine, events include button presses etc.
// MSB specifies that the signal/button is SET: raised/pressed or CLEARED: lowered/released
// Front-side buttons range from 0-9, internal signals range from 10-11 and may be ignored
extern uint8_t  inp( void );

// Reads a configuration parameter from the config file
// The requested value is indicated by a zero-terminated string pointed to by *p_parameter
// A zero terminated value is returned in the buffer pointed to by *p_value
// If the parameter is not available the zero-terminated string pointed to by *p_fallback is returned instead
// Caller must allocate a minimum of 256 bytes for *p_value
extern void     config( int8_t* p_value, int8_t* p_parameter, int8_t* p_fallback );

// Consumes a token from the finite list of tokens in the token file
// The requested token is pointed to by tok[] and must be exactly 16 bytes
// If successful the token is consumed and may never be used again, and the function returns true
// The function returns false when the token cannot be found
extern bool     token( uint8_t tok[ 16 ] );

// Same as send, except takes a zero-terminated string pointed to by *s as input
// The necessary \x0A termination character is automatically added
extern void     print( int8_t *s );

// Loads data from storage
// The entire storage is returned in the buffer pointed to by *p_data
// Caller must allocate a minimum of 32Kbytes for *p_data
// Returns true if the operation was successful (page not out of range)
extern void     retrieve( uint8_t *p_data );

// Saves data to storage
// Data to be stored is contained in a buffer pointed to by *p_s
// The number of bytes to store is specified by len, the rest of the page is cleared to 0xFF
// The maximum value of len is 32768, and as such, no more than 32Kbytes of data may be written
// Returns true if the operation was successful (page and len not out of range, no hardware problems)
// This is a blocking call and will return when all data has been saved to storage
extern bool     store( uint8_t *p_data, uint16_t len );

// Writes data to external peripherals
// Address specifies the id of an external device
// The data in the buffer pointed to by *p_data is with a length of len is written
// This is a blocking call and will return when all data has been written or a failure has occurred
// Returns the number of bytes written (will be less than specified if a failure has occurred)
extern uint16_t  write( uint8_t address, uint8_t *p_data, uint16_t len );

// Reads data from external peripherals
// len specifies the maximum number of bytes to read
// The data will be stored in the buffer pointed to by *p_data
// Upon return with one or more bytes of data *address will contain an id for the external device
// Returns the number of bytes read
extern uint16_t  read( uint8_t *address, uint8_t *p_data, uint16_t len );
