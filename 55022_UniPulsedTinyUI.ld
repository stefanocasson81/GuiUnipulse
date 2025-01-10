/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**  Author      : STM32CubeIDE
**
**  Abstract    : Linker script for STM32H7 series
**                128Kbytes FLASH and 560Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2022 STMicroelectronics.
** All rights reserved.
**
** This software is licensed under terms that can be found in the LICENSE file
** in the root directory of this software component.
** If no LICENSE file comes with this software, it is provided AS-IS.
**
****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM_D1) + LENGTH(RAM_D1);    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x0;      /* required amount of heap  */
_Min_Stack_Size = 0x4000; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
  ITCMRAM (xrw)    : ORIGIN = 0x00000000,   LENGTH = 64K
  DTCMRAM (xrw)    : ORIGIN = 0x20000000,   LENGTH = 128K
  /*FLASH    (rx)    : ORIGIN = 0x08000000,   LENGTH = 128K*/
  RAM_D1  (xrw)    : ORIGIN = 0x24000000,   LENGTH = 320K
  RAM_D2  (xrw)    : ORIGIN = 0x30000000,   LENGTH = 32K
  RAM_D3  (xrw)    : ORIGIN = 0x38000000,   LENGTH = 16K

  QSPIFLASH (rx) 	: ORIGIN = 0x90000000, LENGTH = 32M
  SDRAM (rw)		: ORIGIN = 0xd0000000, LENGTH = 8M
}


TARGET(binary) /* specify the file format of binary file */
INPUT (../FixHeaderDebugFirmwareSection.bin)
OUTPUT_FORMAT(default) /* restore the out file format */

/* Define output sections */
SECTIONS
{
	/***** I Primi 512 sono dedicati al info header. Fare riferimento al "Ids2002_Codifica binari firmware" per quanto riguarda la definizione */
	.FixHeaderFirmwareSection (READONLY):
	{
	    . = ALIGN(4);
	    KEEP(../FixHeaderDebugFirmwareSection.bin)
	} >QSPIFLASH


  /* The startup code goes first into QSPIFLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >QSPIFLASH

  /* The program code and other data goes into QSPIFLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >QSPIFLASH

  /* Constant data goes into QSPIFLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >QSPIFLASH

  .ARM.extab (READONLY)  : 
  { 
    *(.ARM.extab* .gnu.linkonce.armextab.*) 
  } >QSPIFLASH
  
  .ARM (READONLY): {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >QSPIFLASH

  .preinit_array (READONLY)   :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >QSPIFLASH

  .init_array (READONLY):
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >QSPIFLASH

  .fini_array (READONLY):
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >QSPIFLASH

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM_D1 AT> QSPIFLASH

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM_D1

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM_D1

   /*******************Sezione aggiunta per utilizzare lo spazio della RAM D2 */
	.ram_d2 :
	{
	   . = ALIGN(4);
	} >RAM_D2
	
	.ram_no_cache (NOLOAD) :
  	{
      *(.ram_no_cache)
  	} >RAM_D2

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
  
	FineFirmwareFlashSection :
	{
	   *(FineFirmwareFlashSection FineFirmwareFlashSection.*)
	   *(.gnu.linkonce.r.*)
	   . = ALIGN(0x4);
	} >QSPIFLASH

 	FontFlashSection :
	{
	   *(FontFlashSection FontFlashSection.*)
	   *(.gnu.linkonce.r.*)
	   . = ALIGN(0x4);
	} >QSPIFLASH
	
	TextFlashSection :
	{
	   *(TextFlashSection TextFlashSection.*)
	   *(.gnu.linkonce.r.*)
	   . = ALIGN(0x4);
	} >QSPIFLASH
	
	ExtFlashSection :
	{
	   *(ExtFlashSection ExtFlashSection.*)
	   *(.gnu.linkonce.r.*)
	   . = ALIGN(0x4);
	} >QSPIFLASH
	
	FineDataFlashSection :
	{
	   *(FineDataFlashSection FineDataFlashSection.*)
	   *(.gnu.linkonce.r.*)
	   . = ALIGN(0x4);
	} >QSPIFLASH

}

