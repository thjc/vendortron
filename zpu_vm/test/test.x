OUTPUT_FORMAT("elf32-zpu", "elf32-zpu", "elf32-zpu")
OUTPUT_ARCH(zpu)
SEARCH_DIR(.);
ENTRY(_start)
SEARCH_DIR("/home/dbetz/zpugcc/toolchain/build/../install/zpu-elf/lib");

RAMSIZE = 64 * 1024;
ROMSIZE = 1 * 1024 * 1024;

MEMORY {
  ram : ORIGIN = 0x00C00000, LENGTH = 64K
  rom : ORIGIN = 0x00010000, LENGTH = 1M
}


SECTIONS {
  .fixed_vectors  : {
	  . = .;
    KEEP (*(.fixed_vectors))
  } >rom =0
  .init           : {
    KEEP (*(.init))
  } >rom =0
  .text : {
    LONG(_start)
    LONG(_data_image)
    LONG(_data_start)
    LONG(_data_end)
    LONG(_bss_start)
    LONG(_bss_end)
    *(.fixed_vectors)
    *(.text)
    *(.text.*)
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(4);
    _data_image = .;
  } >rom
  .fini           : {
    KEEP (*(.fini))
  } >rom =0

  .data BLOCK(4) : {
    _data_start = .;
    *(.data)
    *(.data.*)
    . = ALIGN(4);
    _data_end = .;
  } >ram AT>rom
  .bss BLOCK(4) : {
    _bss_start = .;
    *(.bss)
    *(.bss.*)
    . = ALIGN(4);
    _bss_end = .;
    _end = .;
  } >ram
  .stack RAMSIZE - 0x1000 : {
    _stack = .;
    *(.stack)
  } >ram
}