/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-zpu", "elf32-zpu",
	      "elf32-zpu")
OUTPUT_ARCH(zpu)
SEARCH_DIR(.);
ENTRY(_start)
SEARCH_DIR("/home/oyvind/toolchain/toolchain/build/../install/zpu-elf/lib");
/* Do we need any of these for elf?
   __DYNAMIC = 0;    */

MEMORY {
  rom   : ORIGIN = 0x00000020, LENGTH = 960K - 0x20
/*  exec  : ORIGIN = 0x00010020, LENGTH = 960K - 0x20 */
  ram   : ORIGIN = 0x00100000, LENGTH = 448K
  stack : ORIGIN = 0x0018E000, LENGTH = 4K
}

SECTIONS
{
  /* Read-only sections, merged into text segment: */
  PROVIDE (__executable_start = 0x0); . = 0x0;

  .fixed_vectors  : {
    KEEP (*(.fixed_vectors))
  } > rom =0
  .init           : {
    KEEP (*(.init))
  } > rom = 0
  .text           : {
    *(.text .stub .text.* .gnu.linkonce.t.*)
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.glue_7t) *(.glue_7)
/*  } > exec AT>rom*/
    } >rom
  .fini           : {
    KEEP (*(.fini))
  } > rom =0
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) }> rom 
  .rodata1        : { *(.rodata1) }> rom 
  .eh_frame_hdr : { *(.eh_frame_hdr) }> rom 
  /* Adjust the address for the data segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = ALIGN(1) + (. & (1 - 1));
  /* Ensure the __preinit_array_start label is properly aligned.  We
     could instead move the label definition inside the section, but
     the linker would then create the section even if it turns out to
     be empty, which isn't pretty.  */
  . = ALIGN(32 / 8);
  PROVIDE (__preinit_array_start = .);
  .preinit_array     : { *(.preinit_array) }> rom 
  PROVIDE (__preinit_array_end = .);
  PROVIDE (__init_array_start = .);
  .init_array     : { *(.init_array) }> rom 
  PROVIDE (__init_array_end = .);
  PROVIDE (__fini_array_start = .);
  .fini_array     : { *(.fini_array) }> rom 
  PROVIDE (__fini_array_end = .);
  . = ALIGN(32 / 8);
  __data_rom_start = . ;
  .data           :
  {
    __data_start = . ;
    *(.data .data.* .gnu.linkonce.d.*)
    __data_end = . ;
    SORT(CONSTRUCTORS)
  }>ram AT>rom
  .data1          : { *(.data1) }>rom
  .tdata	  : { *(.tdata .tdata.* .gnu.linkonce.td.*) }>rom
  .tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) }>rom
  .eh_frame       : { KEEP (*(.eh_frame)) }>rom
  .gcc_except_table   : { *(.gcc_except_table) }>rom
  .dynamic        : { *(.dynamic) }>rom
  .ctors          :
  {
    ___ctors = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin*.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    ___ctors_end = .;
  }>rom
  .dtors          :
  {
    ___dtors = .;
    KEEP (*crtbegin*.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    ___dtors_end = .;
  }>rom
  .jcr            : { KEEP (*(.jcr)) }>rom
  .got            : { *(.got.plt) *(.got) }>rom
  _edata = .;
  PROVIDE (edata = .);
  __bss_start = .;
  __bss_start__ = .;
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.  */
   . = ALIGN(32 / 8);
  } >ram
  . = ALIGN(32 / 8);
  _end = .;
  _bss_end__ = . ; __bss_end__ = . ; __end__ = . ;
  PROVIDE (end = .);

  .info 0x0 : {
    . = .;
    LONG( 0x655A5055 );
    LONG( __data_rom_start - 2 );
    LONG( __data_end - __data_start );
  }

    .stack         0x0018EFFF-0x0018E000:
  {
    _stack = .;
    *(.stack)
  }
  .note.gnu.zpu.ident 0 : { KEEP (*(.note.gnu.zpu.ident)) }
  /DISCARD/ : { *(.note.GNU-stack) }
}

