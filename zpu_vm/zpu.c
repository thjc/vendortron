/*
 * ZPU
 *
 * A Virtual Machine for the ZPU architecture as defined by ZyLin Inc.
 *
 * By Michael Rychlik
 *
 * Based on an original idea by Toby Seckshund.
 *
 * Optimizations by Bill Henning.
 *
 * Due to the way the VM is optimized here, in order to minimize ZPU memory access,
 * it is rather hard to follow this code from the ZPU instruction set documentation.
 * Basically the variable "tos" is used to hold the value that is "top of stack" such
 * that it can be accessed quickly without pop/push pairs in many op codes.
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "zpu.h"
#include "zpu_memory.h"
#include "zpu_syscall.h"
#include "zpu_load.h"

// Uncomment this for a simple single step debug functionality
// TG: value specifies max number of steps (default 1000)
//#define SINGLE_STEP 0

uint32_t        pc;
static uint32_t sp;
static uint32_t tos, nos;
static uint8_t  instruction;
static uint32_t cpu;
static bool     touchedPc = false;
static bool     decodeMask = false;

static inline uint32_t pop() {
  sp += 4;
  return( memoryReadLong( sp ) );
}

static inline void push( uint32_t data ) {
  memoryWriteLong( sp, data );
  sp -= 4;
}

uint32_t flip( uint32_t i ) {
  uint32_t t = 0;
  for( int j = 0; j < 32; j++ ) {
    t |= ( ( i >> j ) & 1 ) << ( 31 - j );
  }
  return( t );
}

void zpu_reset() {
  memoryInitialize();
  sysinitialize();
  zpu_load();
  pc = 0x24;
  touchedPc = true;
  sp = 0;
  tos = 0;
}

static void printRegs() {
  printf( "PC=%08x SP=%08x TOS=%08x OP=%02x DM=%02x debug=%08x\n", pc, sp, tos, instruction, decodeMask, 0 );
  fflush( 0 );
}

void zpu_execute() {
#ifdef SINGLE_STEP
  static int step = 0;
#endif

  // Due to the way the VM is optimized here it is required to have
  // a free space at the bottom of the stack, else address zero may get written to
  // during a push on an emty stack.
  //sp = sp - 4;
  sp = STK_TOP - 8;

  while( 1 ) {
    touchedPc = false;
    instruction = memoryReadByte( pc );
#ifdef SINGLE_STEP
    if( step == 0 ) {
      printf( "#pc,opcode,sp,top_of_stack,next_on_stack\n" );
      printf( "#----------\n" );
      printf( "\n" );
    }
    printf( "0x%07x 0x%02x 0x%08x 0x%08x 0x%08x\n", pc, instruction, sp, tos, memoryReadLong( sp + 4 ) );
    fflush( 0 );
    //memoryDisplayLong( sp - 64, 32 );
    //printf( "\n" );
    //getchar();
    if( ++step == SINGLE_STEP ) {
      printf( "Done.\n" );
      fflush( 0 );	
      exit( 0 );
    }
#endif
    if( ( instruction & 0x80 ) == ZPU_IM ) {
      if( decodeMask ) {
        tos = tos << 7;
        tos = tos | ( instruction & 0x7f );
      } else {
        push( tos );
        tos = instruction << 25;
        tos = ( ( int32_t ) tos ) >> 25;
      }
      decodeMask = true;
    } else {
      decodeMask = false;
      if( ( instruction & 0xF0 ) == ZPU_ADDSP ) {
        uint32_t addr;
        addr = instruction & 0x0F;
        addr = sp + ( addr << 2 ); // TG: was * 4
        // Handle case were addr is sp. This DOES happen.
        if( addr == sp ) {
          tos = tos + tos;
        } else {
          tos = tos + memoryReadLong( addr );
        }
      } else if( ( instruction & 0xE0 ) == ZPU_LOADSP ) {
        uint32_t addr;
        //memoryWriteLong( sp, tos );                    // Need this?
        addr = ( instruction & 0x1F ) ^ 0x10;
        addr = sp + ( addr << 2 ); // TG: was * 4
        push(tos);
        tos = memoryReadLong(addr);
      } else if( ( instruction & 0xE0 ) == ZPU_STORESP ) {
        uint32_t addr;
        addr = ( instruction & 0x1F ) ^ 0x10;
        addr = sp + ( addr << 2 ); // TG: was * 4
        memoryWriteLong( addr, tos );
        tos = pop();
      } else {
        switch (instruction) {
          case 0:
            printf ( "\nBreakpoint\n" );
            printRegs();
            exit(1);
            break;
          case ZPU_PUSHPC:
            push( tos );
            tos = pc;
            break;
          case ZPU_OR:
            nos = pop();
            tos = tos | nos;
            break;
          case ZPU_NOT:
            tos = ~tos;
            break;
          case ZPU_LOAD:
            tos = memoryReadLong( tos );
            break;
          case ZPU_PUSHSPADD:
            tos = ( tos << 2 ) + sp; // TG: was * 4
            break;
          case ZPU_STORE:
            nos = pop();
        		memoryWriteLong( tos, nos );
            tos = pop();
            break;
          case ZPU_POPPC:
            pc = tos;
            tos = pop();
            touchedPc = true;
            break;
          case ZPU_POPPCREL:
            pc = pc + tos;
            tos = pop();
            touchedPc = true;
            break;
          case ZPU_FLIP:
            tos = flip( tos );
            break;
          case ZPU_ADD:
            nos = pop();
            tos = tos + nos;
            break;
          case ZPU_SUB:
            nos = pop();
            tos = nos - tos;
            break;
          case ZPU_PUSHSP:
            push( tos );
            tos = sp + 4;
            break;
          case ZPU_POPSP:
            //memoryWriteLong( sp, tos );            // Need this ?
            sp = tos;
            tos = memoryReadLong( sp );
            break;
          case ZPU_NOP:
            break;
          case ZPU_AND:
            nos = pop();
            tos &= nos;
            break;
          case ZPU_XOR:
            nos = pop();
            tos ^= nos;
            break;
          case ZPU_LOADB:
		        //memoryWriteLong( sp, tos );            //Need this ?
            tos = memoryReadByte( tos );
            break;
          case ZPU_STOREB:
            nos = pop();
            memoryWriteByte( tos, nos );
            tos = pop();
            break;
          case ZPU_LOADH:
		        //memoryWriteLong( sp, tos );            //Need this ?
            tos = memoryReadWord( tos );
            break;
          case ZPU_STOREH:
            nos = pop();
            memoryWriteWord( tos, nos );
            tos = pop();
            break;
          case ZPU_LESSTHAN:
            nos = pop();
            tos = ( ( int32_t )tos < ( int32_t )nos ? 1 : 0 );
            break;
          case ZPU_LESSTHANOREQUAL:
            nos = pop();
            tos = ( ( int32_t )tos <= ( int32_t )nos ? 1 : 0 );
            break;
          case ZPU_ULESSTHAN:
            nos = pop();
            tos = ( tos < nos ? 1 : 0 );
            break;
          case ZPU_ULESSTHANOREQUAL:
            nos = pop();
            tos = ( tos <= nos ? 1 : 0 );
            break;
          case ZPU_SWAP:
            tos = ( ( tos >> 16 ) & 0xffff ) | ( tos << 16 );
            break;
          case ZPU_MULT16X16:
            nos = pop();
            tos = ( nos & 0xffff ) * ( tos & 0xffff );
            break;
          case ZPU_EQBRANCH:
            nos = pop();
            if( nos == 0 ) {
              pc = pc + tos;
              touchedPc = true;
            }
            tos = pop();
            break;
          case ZPU_NEQBRANCH:
            nos = pop();
            if( nos != 0 ) {
              pc = pc + tos;
              touchedPc = true;
            }
            tos = pop();
            break;
          case ZPU_MULT:
            nos = pop();
            tos = ( int32_t )tos * ( int32_t )nos;
            break;
          case ZPU_DIV:
            nos = pop();
            if( nos == 0 ) {
              printf( "Divide by zero\n" );
              fflush( 0 );
              exit( 1 );
            }
            tos = ( int32_t )tos / ( int32_t )nos;
            break;
          case ZPU_MOD:
            nos = pop();
            if( nos == 0 ) {
              printf( "Divide by zero\n" );
              fflush( 0 );
              exit( 1 );
            }
            tos = ( int32_t )tos % ( int32_t )nos;
            break;
          case ZPU_LSHIFTRIGHT:
            nos = pop();
            tos = nos >> ( tos & 0x3f );
            break;
          case ZPU_ASHIFTLEFT:
            nos = pop();
            tos = nos << ( tos & 0x3f );
            break;
          case ZPU_ASHIFTRIGHT:
            nos = pop();
            tos = ( int32_t )nos >> ( tos & 0x3f );
            break;
          case ZPU_CALL:
            nos = tos;
            tos = pc + 1;
            pc  = nos;
            touchedPc = true;
            break;
          case ZPU_CALLPCREL:
            nos = tos;
            tos = pc + 1;
            pc  = pc + nos;
            touchedPc = true;
            break;
          case ZPU_EQ:
            nos = pop();
            tos = ( nos == tos ? 1 : 0 );
            break;
          case ZPU_NEQ:
            nos = pop();
            tos = ( nos != tos ? 1 : 0 );
            break;
          case ZPU_NEG:
            tos = -tos;
            break;
          case ZPU_CONFIG:
            cpu = tos;
            tos = pop();
            printf( "CONFIG indicates CPU type is %d\n", cpu );
            break;
          case ZPU_SYSCALL:
            // Flush tos to real stack
            memoryWriteLong( sp, tos );
            syscall( sp );
            break;
          default:
            printf( "Illegal Instruction\n" );
            printRegs();
            exit( 1 );
            break;
        }
      }
    }
    if( !touchedPc ) {
      pc = pc + 1;
      //touchedPc = true; // TG: this is set to false at top of loop
    }
  }
}