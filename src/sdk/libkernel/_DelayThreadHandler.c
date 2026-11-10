// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
void iSignalSema(int sema);
int _DelayThreadHandler(int id, unsigned short t, int a2, int sema)
{
    iSignalSema(sema);
    __asm__ volatile("sync");
    __asm__ volatile("ei");
    return 0;
}
