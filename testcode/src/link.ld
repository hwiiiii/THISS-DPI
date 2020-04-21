/* Defined symbols
 * __etext
 * __data_start__
 * __data_end__
 * __bss_start__
 * __bss_end__
 * __end__
 * __HeapLimit
 * __StackTop
 * __StackLimit
 */
/* Output format and endtry */
OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(Reset_Handler)

/* Mem areas */
MEMORY
{
	RAM (wrx)	:	ORIGIN = 0x80000000, LENGTH = 5M /*5M should be enough*/
}

/*
 * Stack 
stack_size		=	1024;

_stack_start	=	ORIGIN(RAM)+LENGTH(RAM);
_stack_end		=	_stack_start - stack_size - 4;
*/

/* Output sections */
SECTIONS
{
	.text :
	{
		. = ALIGN(4);
		KEEP(*(.vectors))
		*(.text*)
		*(.rodata*)
	} > RAM

    __etext = .;

	.data :
	{
		. = ALIGN(4);
        __data_start__ = .;
		*(.data*)
        *(.sdata*)
        . = ALIGN(4);
        __data_end__ = .;
	} > RAM

	.bss :
	{
		. = ALIGN(4);
        __bss_start__ = .;
		*(.bss*)
		*(.sbss*)
		*(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
	} > RAM

    .heap (COPY) :
    {
        __end__ = .;
        *(.heap*)
        __HeapLimit = .;
    } > RAM

    .stack_dummy (COPY) :
    {
        *(.stack*)
    } > RAM

    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);

    ASSERT(__StackLimit >= __HeapLimit, "RAM overflowed w/ stack")
}

