// UnCompress LZ77 Data to WRAM
void LZ77UnCompWRAM(unsigned long source, unsigned long dest) {
asm("mov r0, %0\n"
"mov r1, %1\n"
"swi 0x11\n"
:
:"r" (source), "r" (dest)
:"r0", "r1" );
}


// UnCompress LZ77 Data to VRAM
void LZ77UnCompVRAM(unsigned long source, unsigned long dest) {
asm("mov r0, %0\n"
"mov r1, %1\n"
"swi 0x12\n"
:
:"r" (source), "r" (dest)
:"r0", "r1" );
}