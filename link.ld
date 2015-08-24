/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/* Correct place.  */
/* Original file taken from Bran's Kernel Development */
/* tutorials: http://www.osdever.net/bkerndev/index.php. */

/*OUTPUT_FORMAT(elf32-i386) */ 
ENTRY(start)
SECTIONS
{
  . = 0x100000;  

   PROVIDE( kern_start = . ); 
  .text : 
  {
    *(.text)
    . = ALIGN(4096);
  }

  .data :
  {
     *(.data)
     *(.rodata)
     . = ALIGN(4096);
  }

  .bss :
  {
    *(.bss)
    . = ALIGN(4096);
  }
  .stab :
  {
    *(.stab)
    . = ALIGN(4096);
  }
  .stabstr : 
  {
  	*(.stabstr) 
	. = ALIGN(4096); 
  }
   PROVIDE( kern_end = . ); 

}
