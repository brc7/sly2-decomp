// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder
void _sceIDC(unsigned int start, unsigned int end)
{
    int i;
    unsigned int tag;
    for (i = 0; i < 0x1000; i += 0x40) {
        __asm__ volatile("sync\n\tcache 0x10,0(%0)\n\tsync" : : "r"(i));
        __asm__ volatile("mfc0 %0,$28" : "=r"(tag));
        tag = (tag & 0xFFFFF000) + i;
        if (tag >= start && tag <= end) {
            __asm__ volatile("sync\n\tcache 0x16,0(%0)\n\tsync" : : "r"(i));
        }
        __asm__ volatile("sync\n\tcache 0x10,1(%0)\n\tsync" : : "r"(i));
        __asm__ volatile("mfc0 %0,$28" : "=r"(tag));
        tag = (tag & 0xFFFFF000) + i;
        if (tag >= start && tag <= end) {
            __asm__ volatile("sync\n\tcache 0x16,1(%0)\n\tsync" : : "r"(i));
        }
        __asm__ volatile("sync");
    }
}
