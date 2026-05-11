ENTRY(Reset_Handler)

MEMORY
{
  FLASH_BOOT (rx)  : ORIGIN = 0x08000000, LENGTH = 16K
  FLASH_USER (rx)  : ORIGIN = 0x08004000, LENGTH = 16K
  FLASH_APP  (rx)  : ORIGIN = 0x08008000, LENGTH = 96K
  RAM        (xrw) : ORIGIN = 0x20000000, LENGTH = 32K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

_user_flash_start = ORIGIN(FLASH_USER);
_user_flash_end   = ORIGIN(FLASH_USER) + LENGTH(FLASH_USER);
_user_flash_size  = LENGTH(FLASH_USER);

SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector))
    KEEP(*(.vectors))
  } > FLASH_BOOT

  .boot_text :
  {
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4);
  } > FLASH_BOOT

  .text :
  {
    . = ALIGN(4);
    *(.text*)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
    . = ALIGN(4);
    _etext = .;
  } > FLASH_APP

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH_APP

  .ARM.exidx :
  {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } > FLASH_APP

  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT > FLASH_APP

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE(end = .);
    PROVIDE(_end = .);
    . = . + 0x400;
    . = . + 0x400;
    . = ALIGN(8);
  } > RAM

  /DISCARD/ :
  {
    *(.note*)
    *(.comment*)
    *(.eh_frame*)
  }
}