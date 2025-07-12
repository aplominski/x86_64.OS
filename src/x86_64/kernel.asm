BITS 32

section .multiboot_header
align 8

    dd 0xE85250D6
    dd 0
    dd header_end - header_start
    dd -(0xE85250D6 + 0 + (header_end - header_start))

header_start:
    dd 0       ; tag type = end
    dd 8       ; tag size = 8
header_end:

section .text
global _start
global write_string
extern kmain

_start:
    call kmain

.halt:
    hlt
    jmp .halt
